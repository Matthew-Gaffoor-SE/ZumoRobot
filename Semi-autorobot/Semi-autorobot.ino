/** This example uses the Zumo's line sensors to detect the black
border around a burning building.  When the border is detected, it
backs up and turns. */

#include <Wire.h>
#include <Zumo32U4.h>

// Threshold set to 700 to follow a light path (white)
#define QTR_THRESHOLD     700  // microseconds

// counter indicated how many times the zumo touches the wall - This is to prevent the zumo being stuck bouncing between 2 walls
int Counter = 0;

// declaring the timer and setting it to occur every 10 seconds
uint32_t interval = 10000;
char action;

// Standard speeds the zumo will use to navigate the building
#define REVERSE_SPEED     100  // ms
#define TURN_SPEED        110  // ms
#define FORWARD_SPEED     80   // ms
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     200  // ms

// higher speeds which will break the zumo out of a bounce loop
#define END_REVERSE_SPEED     0    // ms
#define END_TURN_SPEED        250  // ms
#define END_REVERSE_DURATION  50   // ms
#define END_TURN_DURATION     300  // ms

// importing everything the zumo will need.
Zumo32U4OLED display;
PololuBuzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

// Only activates the zumo when the 'a' button is pressed on the zumo hardware
void waitForButtonAndCountDown()
{
  ledYellow(1);
  display.clear();
  display.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0);
  display.clear();
}

void setup()
{
  // turning on all the sensors and connecting to port '9600'
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  waitForButtonAndCountDown();

      Serial1.begin(9600);

    Serial.begin(9600);
}

void loop()
{
    // picking up any results from the front 2 sensors.
    proxSensors.read(); 
    int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
    int center_right_sensor = proxSensors.countsFrontWithRightLeds();
    
    // if anything comes into a 6 inch range from either sensor the zumo will stop, make a sound and turn in the direction opposite
    // from the detected sensor.
    if (center_left_sensor < 6)
    {
      delay(5);
    }
    else
    {
      motors.setSpeeds(0, 0);
      buzzer.play("!L16 V8 cdefgab>cbagfedc");
      while (buzzer.isPlaying());
      motors.setSpeeds(END_TURN_SPEED, -END_TURN_SPEED);
      delay(END_TURN_DURATION);
      delay(100);
    }
    if (center_right_sensor < 6)
    {
      delay(5);
    }
    else
    {
      motors.setSpeeds(0, 0);
      buzzer.play("!L16 V8 cdefgab>cbagfedc");
      while (buzzer.isPlaying());
      motors.setSpeeds(-END_TURN_SPEED, END_TURN_SPEED);
      delay(END_TURN_DURATION);
      delay(100);
    }

  if (buttonA.isPressed())
  {
    // If button is pressed, stop and wait for another press to
    // go again.
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  // reading values from bottom sensors to avoid crossing over black lines.
  lineSensors.read(lineSensorValues);

  if (lineSensorValues[0] > QTR_THRESHOLD or lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
  {
    // auto controls pause until the zumo is rotated off the black line
    motors.setSpeeds(0, 0);
    delay(1000);
  }
  else
  {
    // auto drive continues
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }

      if (Serial1.available() > 0)
    {
      action = Serial1.read();

      // controls
      switch (action)
      {

        case 'a':

          motors.setSpeeds(-END_TURN_SPEED, END_TURN_SPEED);
          delay(TURN_DURATION);
          delay(2);

        break;

        case 'd':

          motors.setSpeeds(END_TURN_SPEED, -END_TURN_SPEED);
          delay(TURN_DURATION);
          delay(2);

        break;

        case 'j':

          motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
          delay(TURN_DURATION);
          delay(2);

        break;

        case 'l':

          motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
          delay(TURN_DURATION);
          delay(2);

        break;
      }
  }
}
