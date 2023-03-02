#include <Wire.h>

#include <Zumo32U4.h>

  // importing everything the zumo will need.
  Zumo32U4Motors motors;
  Zumo32U4LCD lcd;
  Zumo32U4ProximitySensors proxSensors;
  Zumo32U4ButtonA buttonA;


  // declaring values to how fast the zumo will travel
  char action;

  int speed = 200;

  // slow speed ensures the zumo can be rotated without crossing a black line
  int slowSpeed = 100;

  PololuBuzzer buzzer;

  void setup()

  {
    // turning on the sensors and connecting to port '9600'
    proxSensors.initThreeSensors();
    
    Serial1.begin(9600);

    Serial.begin(9600);

    ledGreen(1);

  }

  void loop()
  {
    // reading all sensor values and displaying different LED depending on zumo actions.
    proxSensors.read();    
    int left_sensor = proxSensors.countsLeftWithLeftLeds();
    int center_left_sensor = proxSensors.countsFrontWithLeftLeds();
    int center_right_sensor = proxSensors.countsFrontWithRightLeds();
    int right_sensor = proxSensors.countsRightWithRightLeds();

    lcd.gotoXY(0, 1);
    lcd.print(left_sensor);
    lcd.print(" ");
    lcd.print(center_left_sensor);
    lcd.print(" ");
    lcd.print(center_right_sensor);
    lcd.print(" ");
    lcd.print(right_sensor);

    // using 6 inches to pick up on objects which the zumo will detect and black sound.
    if (center_left_sensor < 6 or center_right_sensor < 6)
    {
      ledRed(1);
      ledYellow(0);
      ledGreen(0);
    }
    else
    {
      ledRed(0);
      ledYellow(0);
      ledGreen(0);
      buzzer.play("!L16 V8 cdefgab>cbagfedc");
      while (buzzer.isPlaying());
    }

    if (Serial1.available() > 0)
    {
      action = Serial1.read();

      // controls with changing LEDs

      switch (action)
      {

        case 'w':

          ledYellow(1);

          motors.setLeftSpeed(speed);
          motors.setRightSpeed(speed);

          delay(2);

        break;

        case 's':

          ledYellow(1);

          motors.setLeftSpeed(speed*-1);
          motors.setRightSpeed(speed*-1);

          delay(2);

        break;

        case 'a':

          ledRed(1);

          motors.setRightSpeed(speed);
          motors.setLeftSpeed(speed*-1);

          delay(2);

        break;

        case 'd':

          ledRed(1);

          motors.setLeftSpeed(speed);
          motors.setRightSpeed(speed*-1);

          delay(2);

        break;

        case 'i':

          ledYellow(1);

          motors.setLeftSpeed(slowSpeed);
          motors.setRightSpeed(slowSpeed);

          delay(2);

        break;

          case 'k':

          ledYellow(1);

          motors.setLeftSpeed(slowSpeed*-1);
          motors.setRightSpeed(slowSpeed*-1);

          delay(2);

        break;

        case 'j':

          ledRed(1);

          motors.setRightSpeed(slowSpeed);
          motors.setLeftSpeed(slowSpeed*-1);

          delay(2);

        break;

        case 'l':

          ledRed(1);

          motors.setLeftSpeed(slowSpeed);
          motors.setRightSpeed(slowSpeed*-1);

          delay(2);

        break;


        default:

          motors.setLeftSpeed(0);

          motors.setRightSpeed(0);

          ledYellow(0);

          ledRed(0);
          
      }
      delay(500);
    }
  }
