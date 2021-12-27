#include <DigiMouse.h>
const int SHIFT_PX = 5;
const int SHIFT_PY = 5;

void setup()
{
  DigiMouse.begin();
}

uint8_t seq = 0;

void loop()
{
  switch (seq++ % 4)
  {
  case 0:
    DigiMouse.moveX(SHIFT_PX);
    DigiMouse.moveY(SHIFT_PY);
    break;

  case 1:
    DigiMouse.moveX(-SHIFT_PX);
    DigiMouse.moveY(SHIFT_PY);
    break;

  case 2:
    DigiMouse.moveX(SHIFT_PX);
    DigiMouse.moveY(-SHIFT_PY);
    break;

  case 3:
    DigiMouse.moveX(-SHIFT_PX);
    DigiMouse.moveY(-SHIFT_PY);
    break;
  }

  DigiMouse.delay(150);
}