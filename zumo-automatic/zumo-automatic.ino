/** This example uses the Zumo's line sensors to detect the black
border around a burning building.  When the border is detected, it
backs up and turns. */

#include <Wire.h>
#include <Zumo32U4.h>

// Threshold set to 700 to follow white line avoiding dark colours

#define QTR_THRESHOLD     700  // microseconds

// counter default set to 0 with a timer set to occur every 10 seconds
int Counter = 0;
uint32_t interval = 10000;

// speeds in which the zumo will travel.
// This is the default speed
#define REVERSE_SPEED     100  // ms
#define TURN_SPEED        110  // ms
#define FORWARD_SPEED     90   // ms
#define REVERSE_DURATION  100  // ms
#define TURN_DURATION     200  // ms

// bounce loop break speeds
#define END_REVERSE_SPEED     0     // ms
#define END_TURN_SPEED        250   // ms
#define END_REVERSE_DURATION  50    // ms
#define END_TURN_DURATION     300   // ms

// Speed the zumo will use when turing both directions to look for objects
#define SEARCH_TURN_SPEED        400  // ms
#define SEARCH_REVERSE_DURATION  200  // ms
#define SEARCH_TURN_DURATION     450  // ms

// Importing everything the zumo will need
Zumo32U4OLED display;
PololuBuzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

// Starting the zumo when the 'a' button is pressed on zumo hardware
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
  // Turning on all sensors and waiting for 'a' button pressed
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  waitForButtonAndCountDown();
}

void loop()
{
    // Reading front sensor values to detect objects
    proxSensors.read(); 
    int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
    int center_right_sensor = proxSensors.countsFrontWithRightLeds();
    
    // 6 inch search range, if an object is detected in that range the zumo will make a noise and turn in the direction opposite of the detected sensor
    if (center_left_sensor < 6)
    {
      delay(5);
    }
    else
    {
      motors.setSpeeds(0, 0);
      buzzer.play("!L16 V8 cdefgab>cbagfedc");
      while (buzzer.isPlaying());
      motors.setSpeeds(FOUND_TURN_SPEED, -FOUND_TURN_SPEED);
      delay(FOUND_TURN_DURATION);
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
      motors.setSpeeds(-FOUND_TURN_SPEED, FOUND_TURN_SPEED);
      delay(FOUND_TURN_DURATION);
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

  // reading botton sensor values to detect if the zumo has come into contact with black lines
  lineSensors.read(lineSensorValues);

  if (lineSensorValues[0] > QTR_THRESHOLD)
  {
    // If leftmost sensor detects line, reverse and turn to the
    // right.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    Counter+=1;
  }
  else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
  {
    // If rightmost sensor detects line, reverse and turn to the
    // left.
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
  }
  else
  {

    // Starting the 10 second timer
    static uint32_t nextTime;

    if (millis() - nextTime >= interval)
    {
      // update next time
      nextTime += interval;

      // The zumo will first turn 90 dregrees left to see if any objects are there.
        motors.setSpeeds(-END_REVERSE_SPEED, -END_REVERSE_SPEED);
        delay(END_REVERSE_DURATION);
        motors.setSpeeds(SEARCH_TURN_SPEED, -SEARCH_TURN_SPEED);
        delay(END_TURN_DURATION);

          // zumo first stops to give sensors time to detect object
          motors.setSpeeds(0, 0);
          delay(1000);

          // Reading front sensor values
          proxSensors.read(); 
          int ctr_left_sensor = proxSensors.countsFrontWithLeftLeds();
          int ctr_right_sensor = proxSensors.countsFrontWithRightLeds();

          // Using 5 inch search range. If no object is detected the zumo then turns 180 degrees to see if an object is on the other side.
          // This ensures the zumo can search rooms
          if (ctr_left_sensor < 5 or ctr_right_sensor < 5)
          {
            motors.setSpeeds(-SEARCH_TURN_SPEED, SEARCH_TURN_SPEED);
            delay(END_TURN_DURATION);
            motors.setSpeeds(-SEARCH_TURN_SPEED, SEARCH_TURN_SPEED);
            delay(END_TURN_DURATION);
            motors.setSpeeds(0, 0);
            delay(1000);
            proxSensors.read(); 

            // If no objects are detected the zumo faces forwards again and carries on the path following
              if (ctr_left_sensor < 5 or ctr_right_sensor < 5)
              {
                motors.setSpeeds(SEARCH_TURN_SPEED, -SEARCH_TURN_SPEED);
                delay(END_TURN_DURATION);
              }
              else
              {
                  motors.setSpeeds(0, 0);
                  delay(100);
              }
            }
            else
            {
                motors.setSpeeds(0, 0);
                delay(100);
            }
    }

    // Otherwise, go straight.
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }

    // If the zumo touches a wall 20 times it will make a harsh turn to avoid bounce loop
    if(Counter == 20)
    {
      motors.setSpeeds(-END_REVERSE_SPEED, -END_REVERSE_SPEED);
      delay(END_REVERSE_DURATION);
      motors.setSpeeds(END_TURN_SPEED, -END_TURN_SPEED);
      delay(END_TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);

      // reset counter
      Counter -= 20;
    }
}
