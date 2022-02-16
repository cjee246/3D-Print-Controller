#include <Adafruit_MPR121.h>
#include <Joystick.h>

//main vars
Adafruit_MPR121 cap = Adafruit_MPR121();

const uint8_t BTNS = 8;
uint8_t button[BTNS];
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  BTNS, 0,               // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

//global vars
uint16_t lastTouched = 0;
uint16_t currTouched = 0;

//setup
void setup() 
{
  cap.begin(0x5A);
  cap.setThresholds(4, 2);
  
  Joystick.begin();
  Joystick.setXAxisRange(0, 2);
  Joystick.setYAxisRange(0, 2);
}

uint8_t ypos = 1;
uint8_t xpos = 1;

//loop
void loop() 
{
  lastTouched = currTouched;
  currTouched = cap.touched();
  updateState();
  delay(10);
}

void updateState()
{
  ypos = 1;
  xpos = 1;
  if (currTouched & bit(0)) { ypos -= 1; }
  if (currTouched & bit(1)) { xpos += 1; }
  if (currTouched & bit(2)) { ypos += 1; }
  if (currTouched & bit(3)) { xpos -= 1; }
  Joystick.setXAxis(xpos);
  Joystick.setYAxis(ypos);

  for (uint8_t i=4; i<BTNS; i++)
  {
    if ((currTouched & bit(i)) != (lastTouched & bit(i)))
    {
      if (currTouched & bit(i)) { Joystick.pressButton(i); }
      else { Joystick.releaseButton(i); }
    }
  }
}
