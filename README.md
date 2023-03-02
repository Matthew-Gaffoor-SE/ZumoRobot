# ZumoRobot
3 Modes to control the zumo robot around a building

PLEASE NOTE:

* These applications are set up ONLY for Zumo 32U4. Errors may occuring trying to use the code for
any other zumo models.
* The zumo model uses 2 Xbee chips which connect via USB and are operational on XCTU ONLY. Trying to control the zumo through arduino will result in an error.
* For optimal results ensure the black order is not gloss or a light source isnt directly shining
down on the lines. This can cause the zumo to drive over borders where light spots are present.
* If the zumo is struggling to detect ojects try to move the objects slightly as the zumo is searching.
* Ensure the zumo is placed in the centre of the white lines, facing forwards on start-up to avoid zumo crossing black lines straight away.
* Once the zumo detects an object - remove it from the building path to avoid zumo entering a loop
of object detection.
* When changing modes wait until arduino has confirmed upload is complete before unplugging zumo.
* Ensure the Xbee 2.0 is connected to your desktop/laptop at all times to avoid connection break.
* Entering commands into XCTU ARE caps lock sensitive. Ensure caps lock is TURNED OFF.

                            Search and Rescue Zumo 32U4

The zumo robot is tasked with searching through a burning building to find any survivors stuck in rooms. The person (any object) can be placed at any point in the room.

Uploading any of the 3 modes will make the zumo act differently. This can be done by connecting the zumo robot via USB to your desktop/laptop and uploading the pre-build code.

Once a zumo comes into contact with a person (object) it will do one of 2 options depending on the mode:

1. (Manual) - The zumo will make a sound and can be continued with controls
2. (Semi-Automatic, Automatic) - The zumo will make a sound and turn in a direction away from the person to avoid collision.

Manual Controls:

* This mode does not detect black lines *

After placing the robot in the middle of the white line control the zumo by pressing either

w - Forward || s - Backwards || a - Left || d - Right        - Normal Speed
i - Forward || k - Backwards || j - Left || l - Right        - Slow Speed
Enter                                                        - Stop robot

Semi-Automatic:

After placing the robot in the middle of the white line press the 'a' button on the zumo which can be found at the rear. Once the zumo connects with a black line it will stop and wait for a direction to be given (Note - The zumo will only resume once its path is strayed away from the connected line).

a - Left || d - Right                                         - Large Rotation
j - Left || d - Right                                         - Small Rotation

Fully-Automatic:

After placing the robot in the middle of the white line press the 'a' button on the zumo which can be found at the rear. This mode is fully automatic so no instructions need to be given until completion in which the zumo can be turned off.

Source Material:

Zumo following white line: https://forum.pololu.com/t/can-we-use-the-zumo-for-white-line-follower/18763
Using timers with Millis: https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html
Arduino border detection example: https://github.com/pololu/zumo-32u4-arduino-library/blob/master/examples/BorderDetect/BorderDetect.ino 
Object detection using from sensors: https://www.youtube.com/watch?v=ddPo6HQvxzQ&ab_channel=ProfessorCraven

