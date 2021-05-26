/*
 * Tiny4kOLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x32 displays
 *
 * Based on ssd1306xled, re-written and extended by Stephen Denne
 * from 2017-04-25 at https://github.com/datacute/Tiny4kOLED
 *
 */

// ----------------------------------------------------------------------------

#include "Tiny4kOLED_common.h"

#define SSD1306_COLUMNS 128
#define SSD1306_PAGES 4

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// ----------------------------------------------------------------------------

static uint8_t oledOffsetX = 0, oledOffsetY = 0; // pixels and pages
static uint8_t oledWidth = SSD1306_COLUMNS; // pixels and pages
static uint8_t oledPages = SSD1306_PAGES;
static const DCfont *oledFont = 0;
static uint8_t oledX = 0, oledY = 0;
static uint8_t renderingFrame = 0xB0, drawingFrame = 0x40;
static uint8_t invertedOutput = 0;

static void (*wireBeginFn)(void);
static bool (*wireBeginTransmissionFn)(void);
static bool (*wireWriteFn)(uint8_t byte);
static uint8_t (*wireEndTransmissionFn)(void);

static void ssd1306_begin(void) {
	wireBeginFn();
}

static void ssd1306_send_start(void) {
	wireBeginTransmissionFn();
}

static bool ssd1306_send_byte(uint8_t byte) {
	return wireWriteFn(byte);
}

static void ssd1306_send_stop(void) {
	wireEndTransmissionFn();
}

static void ssd1306_send_command_start(void) {
	ssd1306_send_start();
	ssd1306_send_byte(SSD1306_COMMAND);
}

static void ssd1306_send_data_start(void) {
	ssd1306_send_start();
	ssd1306_send_byte(SSD1306_DATA);
}

static void ssd1306_send_command_byte(uint8_t byte) {
	if (ssd1306_send_byte(byte) == 0) {
		ssd1306_send_stop();
		ssd1306_send_command_start();
		ssd1306_send_byte(byte);
	}
}

static void ssd1306_send_data_byte(uint8_t byte) {
	if (ssd1306_send_byte(byte^invertedOutput) == 0) {
		ssd1306_send_stop();
		ssd1306_send_data_start();
		ssd1306_send_byte(byte^invertedOutput);
	}
}

static void ssd1306_send_command(uint8_t command) {
	ssd1306_send_command_start();
	ssd1306_send_byte(command);
	ssd1306_send_stop();
}

static void ssd1306_send_command2(uint8_t command1, uint8_t command2) {
	ssd1306_send_command_start();
	ssd1306_send_byte(command1);
	ssd1306_send_byte(command2);
	ssd1306_send_stop();
}

static void ssd1306_send_command3(uint8_t command1, uint8_t command2, uint8_t command3) {
	ssd1306_send_command_start();
	ssd1306_send_byte(command1);
	ssd1306_send_byte(command2);
	ssd1306_send_byte(command3);
	ssd1306_send_stop();
}

static void ssd1306_send_command6(uint8_t command1, uint8_t command2, uint8_t command3, uint8_t command4, uint8_t command5, uint8_t command6) {
	ssd1306_send_command_start();
	ssd1306_send_byte(command1);
	ssd1306_send_byte(command2);
	ssd1306_send_byte(command3);
	ssd1306_send_byte(command4);
	ssd1306_send_byte(command5);
	ssd1306_send_byte(command6);
	ssd1306_send_stop();
}

static void ssd1306_send_command7(uint8_t command1, uint8_t command2, uint8_t command3, uint8_t command4, uint8_t command5, uint8_t command6, uint8_t command7) {
	ssd1306_send_command_start();
	ssd1306_send_byte(command1);
	ssd1306_send_byte(command2);
	ssd1306_send_byte(command3);
	ssd1306_send_byte(command4);
	ssd1306_send_byte(command5);
	ssd1306_send_byte(command6);
	ssd1306_send_byte(command7);
	ssd1306_send_stop();
}

SSD1306Device::SSD1306Device(void (*wireBeginFunc)(void), bool (*wireBeginTransmissionFunc)(void), bool (*wireWriteFunc)(uint8_t byte), uint8_t (*wireEndTransmissionFunc)(void)) {
	wireBeginFn = wireBeginFunc;
	wireBeginTransmissionFn = wireBeginTransmissionFunc;
	wireWriteFn = wireWriteFunc;
	wireEndTransmissionFn = wireEndTransmissionFunc;
}

void SSD1306Device::begin(void) {
	begin(sizeof(tiny4koled_init_128x32r), tiny4koled_init_128x32r);
}

void SSD1306Device::begin(uint8_t init_sequence_length, const uint8_t init_sequence []) {
	ssd1306_begin();

	ssd1306_send_command_start();
	for (uint8_t i = 0; i < init_sequence_length; i++) {
		ssd1306_send_command_byte(pgm_read_byte(&init_sequence[i]));
	}
	ssd1306_send_stop();
}

void SSD1306Device::begin(uint8_t width, uint8_t height, uint8_t init_sequence_length, const uint8_t init_sequence []) {
	oledOffsetX = (128 - width) >> 1;
	oledOffsetY = 0;
	oledWidth = width;
	oledPages = height >> 3;
	begin(init_sequence_length,init_sequence);
}

void SSD1306Device::begin(uint8_t xOffset, uint8_t yOffset, uint8_t width, uint8_t height, uint8_t init_sequence_length, const uint8_t init_sequence []) {
	oledOffsetX = xOffset;
	oledOffsetY = yOffset >> 3;
	oledWidth = width;
	oledPages = height >> 3;
	begin(init_sequence_length,init_sequence);
}

void SSD1306Device::setPages(uint8_t pages) {
	oledPages = pages;
}

void SSD1306Device::setWidth(uint8_t width) {
	oledWidth = width;
}

void SSD1306Device::setHeight(uint8_t height) {
	oledPages = height >> 3;
}

void SSD1306Device::setOffset(uint8_t xOffset, uint8_t yOffset) {
	oledOffsetX = xOffset;
	oledOffsetY = yOffset >> 3;
}

void SSD1306Device::setRotation(uint8_t rotation) {
	uint8_t rotationBit = (rotation & 0x01);
	ssd1306_send_command2(0xC0 | (rotationBit << 3), 0xA0 | rotationBit);
}

void SSD1306Device::setFont(const DCfont *font) {
	oledFont = font;
}

void SSD1306Device::setCursor(uint8_t x, uint8_t y) {
	ssd1306_send_command3(renderingFrame | ((y + oledOffsetY) & 0x07), 0x10 | (((x + oledOffsetX) & 0xf0) >> 4), (x + oledOffsetX) & 0x0f);
	oledX = x;
	oledY = y;
}

uint8_t SSD1306Device::getCursorY() {
	return oledY;
}

uint8_t SSD1306Device::getCursorX() {
	return oledX;
}

void SSD1306Device::clear(void) {
	fill(0x00);
}

void SSD1306Device::fill(uint8_t fill) {
	for (uint8_t m = 0; m < oledPages; m++) {
		setCursor(0, m);
		fillToEOL(fill);
	}
	setCursor(0, 0);
}

void SSD1306Device::newLine(uint8_t fontHeight) {
	oledY+=fontHeight;
	if (oledY > oledPages - fontHeight) {
		oledY = oledPages - fontHeight;
	}
	setCursor(0, oledY);
}

void SSD1306Device::newLine(void) {
	newLine(oledFont->height);
}

size_t SSD1306Device::write(byte c) {
	if (!oledFont)
		return 1;

	if (c == '\r')
		return 1;
	
	uint8_t h = oledFont->height;

	if (c == '\n') {
		newLine(h);
		return 1;
	}

	uint8_t w = oledFont->width;

	if (oledX > ((uint8_t)oledWidth - w)) {
		newLine(h);
	}

	uint16_t offset = ((uint16_t)c - oledFont->first) * w * h;
	uint8_t line = h;
	do
	{
		ssd1306_send_data_start();
		for (uint8_t i = 0; i < w; i++) {
			ssd1306_send_data_byte(pgm_read_byte(&(oledFont->bitmap[offset++])));
		}
		ssd1306_send_stop();
		if (h == 1) {
			oledX+=w;
		}
		else {
			if (line > 1) {
				setCursor(oledX, oledY + 1);
			}
			else {
				setCursor(oledX + w, oledY - (h - 1));
			}
		}
	}
	while (--line);
	return 1;
}

void SSD1306Device::bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) {
	uint16_t j = 0;
 	for (uint8_t y = y0; y < y1; y++) {
		setCursor(x0,y);
		ssd1306_send_data_start();
		for (uint8_t x = x0; x < x1; x++) {
			ssd1306_send_data_byte(pgm_read_byte(&bitmap[j++]));
		}
		ssd1306_send_stop();
	}
	setCursor(0, 0);
}

void SSD1306Device::clearToEOL(void) {
	fillToEOL(0x00);
}

void SSD1306Device::fillToEOL(uint8_t fill) {
	fillLength(fill, oledWidth - oledX);
}

void SSD1306Device::fillLength(uint8_t fill, uint8_t length) {
	if (length == 0) return;
	oledX += length;
	ssd1306_send_data_start();
	repeatData(fill, length);
	ssd1306_send_stop();
}

void SSD1306Device::startData(void) {
	ssd1306_send_data_start();
}

void SSD1306Device::sendData(const uint8_t data) {
	ssd1306_send_data_byte(data);
}

void SSD1306Device::repeatData(uint8_t data, uint8_t length) {
	for (uint8_t x = 0; x < length; x++) {
		ssd1306_send_data_byte(data);
	}
}

void SSD1306Device::clearData(uint8_t length) {
	repeatData(0, length);
}

void SSD1306Device::endData(void) {
	ssd1306_send_stop();
}

void SSD1306Device::invertOutput(bool enable) {
	invertedOutput = enable ? 0xff : 0;
}	

void SSD1306Device::clipText(uint16_t startPixel, uint8_t width, DATACUTE_F_MACRO_T *text) {
	uint8_t h = oledFont->height;
	uint8_t w = oledFont->width;
	PGM_P p = reinterpret_cast<PGM_P>(text);
	uint8_t drawnColumns = 0;
	// It is currently up to the caller to make sure that the startPixel is still within the text.
	// This method needs to change to read each character in the string up to startPixel, to check for end of string
	// If beyond the end of the string, write 0s/spaces
	// However, it probably takes less bytes currently to simply put spaces at the beginning or end of the text.
	uint16_t charactersToSkip = startPixel / w;
	uint8_t initialSkip = startPixel % w;
	p += charactersToSkip;
	while (drawnColumns < width) {
		unsigned char c = pgm_read_byte(p++);
		if (c == 0) break;
		uint16_t offset = ((uint16_t)c - oledFont->first) * w * h;
		uint8_t line = h;
		do
		{
			offset += initialSkip;
			ssd1306_send_data_start();
			for (uint8_t i = 0; (i < w - initialSkip) && ((drawnColumns + i) < width); i++) {
				ssd1306_send_data_byte(pgm_read_byte(&(oledFont->bitmap[offset + i])));
			}
			offset += w - initialSkip;
			ssd1306_send_stop();
			if (h > 1) {
				if (line > 1) {
					setCursor(oledX, oledY + 1);
				}
				else {
					setCursor(oledX + w, oledY - (h - 1));
				}
			}
		} while (--line);
		drawnColumns += w - initialSkip;
		initialSkip = 0;
	}
}

// Double Buffering Commands

void SSD1306Device::switchRenderFrame(void) {
	renderingFrame ^= 0x04;
}

void SSD1306Device::switchDisplayFrame(void) {
	drawingFrame ^= 0x20;
	ssd1306_send_command(drawingFrame);
}

void SSD1306Device::switchFrame(void) {
	switchDisplayFrame();
	switchRenderFrame();
}

uint8_t SSD1306Device::currentRenderFrame(void) {
	return (renderingFrame >> 2) & 0x01;
}

uint8_t SSD1306Device::currentDisplayFrame(void) {
	return (drawingFrame >> 5) & 0x01;
}

// 1. Fundamental Command Table

void SSD1306Device::setContrast(uint8_t contrast) {
	ssd1306_send_command2(0x81,contrast);
}

void SSD1306Device::setEntireDisplayOn(bool enable) {
	if (enable)
		ssd1306_send_command(0xA5);
	else
		ssd1306_send_command(0xA4);
}

void SSD1306Device::setInverse(bool enable) {
	if (enable)
		ssd1306_send_command(0xA7);
	else
		ssd1306_send_command(0xA6);
}

void SSD1306Device::setExternalIref(void) {
	ssd1306_send_command2(0xAD, 0x00);
}

void SSD1306Device::setInternalIref(bool bright) {
	ssd1306_send_command2(0xAD, ((bright & 0x01) << 5) | 0x10);
}

void SSD1306Device::off(void) {
	ssd1306_send_command(0xAE);
}

void SSD1306Device::on(void) {
	ssd1306_send_command(0xAF);
}

// 2. Scrolling Command Table

void SSD1306Device::scrollRight(uint8_t startPage, uint8_t interval, uint8_t endPage, uint8_t startColumn, uint8_t endColumn) {
	ssd1306_send_command7(0x26, 0x00, startPage + oledOffsetY, interval, endPage + oledOffsetY, startColumn, endColumn);
}

void SSD1306Device::scrollLeft(uint8_t startPage, uint8_t interval, uint8_t endPage, uint8_t startColumn, uint8_t endColumn) {
	ssd1306_send_command7(0x27, 0x00, startPage + oledOffsetY, interval, endPage + oledOffsetY, startColumn, endColumn);
}

void SSD1306Device::scrollRightOffset(uint8_t startPage, uint8_t interval, uint8_t endPage, uint8_t offset) {
	ssd1306_send_command6(0x29, 0x00, startPage + oledOffsetY, interval, endPage + oledOffsetY, offset);
}

void SSD1306Device::scrollLeftOffset(uint8_t startPage, uint8_t interval, uint8_t endPage, uint8_t offset) {
	ssd1306_send_command6(0x2A, 0x00, startPage + oledOffsetY, interval, endPage + oledOffsetY, offset);
}

void SSD1306Device::scrollContentRight(uint8_t startPage, uint8_t endPage, uint8_t startColumn, uint8_t endColumn) {
	ssd1306_send_command7(0x2C, 0x00, startPage + oledOffsetY, 0x01, endPage + oledOffsetY, startColumn + oledOffsetX, endColumn + oledOffsetX);
}

void SSD1306Device::scrollContentLeft(uint8_t startPage, uint8_t endPage, uint8_t startColumn, uint8_t endColumn) {
	ssd1306_send_command7(0x2D, 0x00, startPage + oledOffsetY, 0x01, endPage + oledOffsetY, startColumn + oledOffsetX, endColumn + oledOffsetX);
}

void SSD1306Device::deactivateScroll(void) {
	ssd1306_send_command(0x2E);
}

void SSD1306Device::activateScroll(void) {
	ssd1306_send_command(0x2F);
}

void SSD1306Device::setVerticalScrollArea(uint8_t top, uint8_t rows) {
	ssd1306_send_command3(0xA3, top, rows);
}

// 3. Addressing Setting Command Table

void SSD1306Device::setColumnStartAddress(uint8_t startAddress) {
	ssd1306_send_command2(startAddress & 0x0F, startAddress >> 4);
}

void SSD1306Device::setMemoryAddressingMode(uint8_t mode) {
	ssd1306_send_command2(0x20, mode & 0x03);
}

void SSD1306Device::setColumnAddress(uint8_t startAddress, uint8_t endAddress) {
	ssd1306_send_command3(0x21, startAddress & 0x7F, endAddress & 0x7F);
}

void SSD1306Device::setPageAddress(uint8_t startPage, uint8_t endPage) {
	ssd1306_send_command3(0x22, startPage & 0x07, endPage & 0x07);
}

void SSD1306Device::setPageStartAddress(uint8_t startPage) {
	ssd1306_send_command(0xB0 | (startPage & 0x07));
}

// 4. Hardware Configuration (Panel resolution and layout related) Command Table

void SSD1306Device::setDisplayStartLine(uint8_t startLine) {
	ssd1306_send_command(0x40 | (startLine & 0x3F));
}

void SSD1306Device::setSegmentRemap(uint8_t remap) {
	ssd1306_send_command(0xA0 | (remap & 0x01));
}

void SSD1306Device::setMultiplexRatio(uint8_t mux) {
	ssd1306_send_command2(0xA8, (mux - 1) & 0x3F);
}

void SSD1306Device::setComOutputDirection(uint8_t direction) {
	ssd1306_send_command(0xC0 | ((direction & 0x01) << 3));
}

void SSD1306Device::setDisplayOffset(uint8_t offset) {
	ssd1306_send_command2(0xD3, offset & 0x3F);
}

void SSD1306Device::setComPinsHardwareConfiguration(uint8_t alternative, uint8_t enableLeftRightRemap) {
	ssd1306_send_command2(0xDA, ((enableLeftRightRemap & 0x01) << 5) | ((alternative & 0x01) << 4) | 0x02 );
}

// 5. Timing and Driving Scheme Setting Command table

void SSD1306Device::setDisplayClock(uint8_t divideRatio, uint8_t oscillatorFrequency) {
	ssd1306_send_command2(0xD5, ((oscillatorFrequency & 0x0F) << 4) | ((divideRatio -1) & 0x0F));
}

void SSD1306Device::setPrechargePeriod(uint8_t phaseOnePeriod, uint8_t phaseTwoPeriod) {
	ssd1306_send_command2(0xD9, ((phaseTwoPeriod & 0x0F) << 4) | (phaseOnePeriod & 0x0F));
}

void SSD1306Device::setVcomhDeselectLevel(uint8_t level) {
	ssd1306_send_command2(0xDB, (level & 0x07) << 4);
}

void SSD1306Device::nop(void) {
	ssd1306_send_command(0xE3);
}

// 6. Advance Graphic Command table

void SSD1306Device::fadeOut(uint8_t interval) {
	ssd1306_send_command2(0x23, (0x20 | (interval & 0x0F)));
}

void SSD1306Device::blink(uint8_t interval) {
	ssd1306_send_command2(0x23, (0x30 | (interval & 0x0F)));
}

void SSD1306Device::disableFadeOutAndBlinking(void) {
	ssd1306_send_command2(0x23, 0x00);
}

void SSD1306Device::enableZoomIn(void) {
	ssd1306_send_command2(0xD6, 0x01);
}

void SSD1306Device::disableZoomIn(void) {
	ssd1306_send_command2(0xD6, 0x00);
}

// Charge Pump Settings

void SSD1306Device::enableChargePump(uint8_t voltage) {
	ssd1306_send_command2(0x8D, ((voltage | 0x14) & 0xD5));
}

void SSD1306Device::disableChargePump(void) {
	ssd1306_send_command2(0x8D, 0x10);
}

// ----------------------------------------------------------------------------
