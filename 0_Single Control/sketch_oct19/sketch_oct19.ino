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
  if (digitalRead(MOD_PIN) == HIGH && ctrlMode != 1)
  {
    ctrlMode = 1;
    up_ = 0;
    right_ = 1;
    down_ = 2;
    left_ = 3;
    btn1_ = 4;
    btn2_ = 5;
    
  }
  else if (ctrlMode != 2)
  {
    ctrlMode = 2;
  }

  //update joystick
  ypos = 1;
  xpos = 1;
  if (currTouched & bit(up_)) { ypos -= 1; }
  if (currTouched & bit(right_)) { xpos += 1; }
  if (currTouched & bit(down_)) { ypos += 1; }
  if (currTouched & bit(left_)) { xpos -= 1; }
  Joystick.setXAxis(xpos);
  Joystick.setYAxis(ypos);

  //update main buttons
  if ((currTouched & bit(btn1_)) != (lastTouched & bit(btn1_)))
  {
      if (currTouched & bit(btn1_)) { Joystick.pressButton(4); }
      else { Joystick.releaseButton(4); }
  }
  if ((currTouched & bit(btn2_)) != (lastTouched & bit(btn2_)))
  {
      if (currTouched & bit(btn2_)) { Joystick.pressButton(5); }
      else { Joystick.releaseButton(5); }
  }

  //menu button
  if (btnMenu == 0)
  {
    if ((currTouched & bit(up_)) && (currTouched & bit(right_)) && (currTouched & bit(down_)) && (currTouched & bit(left_)))
    {
      btnMenu = 1;
      Joystick.pressButton(2);
    }
  }
  else
  {
    if (!((currTouched & bit(up_)) && (currTouched & bit(right_)) && (currTouched & bit(down_)) && (currTouched & bit(left_))))
    {
      btnMenu = 0;
      Joystick.releaseButton(2);
    }
  }
}
