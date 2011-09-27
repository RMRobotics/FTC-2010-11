#ifndef MOTORS
#include "motorsSensors.h"
#endif
#ifndef CONST
#include "constants.h"
#endif
#ifndef HI
#include "highLevelMoves.c"
#endif

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


void initializeRobot()
{
  return;
}

task main()
{
  initializeRobot();

  moveStraight(DIR_FWD, 0, 0, true);

  waitForStart(); // Wait for the beginning of autonomous phase.


}
