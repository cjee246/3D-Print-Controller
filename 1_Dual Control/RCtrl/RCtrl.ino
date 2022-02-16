#include <Adafruit_MPR121.h>
#include <Joystick.h>

//main vars
Adafruit_MPR121 cap = Adafruit_MPR121();

const uint8_t BTNS = 6;
uint8_t button[BTNS];
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  BTNS, 0,               // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

//global vars
const uint8_t MOD_PIN = 8;
uint8_t ctrlMode = 0;
uint16_t lastTouched = 0;
uint16_t currTouched = 0;

uint8_t up_, down_, left_, right_, btn1_, btn2_, btn3_, btn4_, btn5_, btn6_;
uint8_t btnMenu = 0;

//setup
void setup() 
{
  if (!cap.begin(0x5A))
  {
    if (!cap.begin(0x5B))
    {
      //no controller connected
    }
    else
    {
      //keep one line high to tell second MCU to stay inactive
    }
  }
  cap.setThresholds(5, 5);
  
  Joystick.begin();
  Joystick.setXAxisRange(0, 2);
  Joystick.setYAxisRange(0, 2);

  Joystick.setXAxis(1);
  Joystick.setYAxis(1);

  pinMode(MOD_PIN, INPUT_PULLUP);
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
  /*
  if (digitalRead(MOD_PIN) == HIGH && ctrlMode != 1)
  {
    ctrlMode = 1;
    btn1_ = 0;
    btn2_ = 1;
    btn3_ = 2;
    btn4_ = 3;
    btn5_ = 4;
    btn6_ = 5;
    
  }
  else if (ctrlMode != 2)
  {
    ctrlMode = 2;
  }
  */
  
  for (uint8_t i=0; i<BTNS; i++)
  {
    if ((currTouched & bit(i)) != (lastTouched & bit(i)))
    {
      if (currTouched & bit(i)) { Joystick.pressButton(i); }
      else { Joystick.releaseButton(i); }
    }
  }
}
