#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     backLightSensor,     sensorLightActive)
#pragma config(Sensor, S3,     frontLightSensor,    sensorLightActive)
#pragma config(Sensor, S4,     distanceSensor,      sensorSONAR)
#pragma config(Motor,  motorA,          boxDoor,       tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     rightWheel,    tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     leftWheel,     tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     rightOmniWheel, tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     leftOmniWheel, tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     belt,          tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     scoringBox,    tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    bridgeServo,          tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    scoreBoxTilt,         tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    dispenserDoor,        tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

void gotoMiddleDispenserFromRight();

void initializeRobot()
{
  bFloatDuringInactiveMotorPWM = false;  // brake
  initializeServos();
  disableDiagnosticsDisplay();
  return;
}

task main()
{
  initializeRobot();

  sensorsDebug(true);

  //waitForStart();

  //balanceOnBridgeFromLeft();

  gotoMiddleDispenserFromRight();
}

void gotoMiddleDispenserFromRight(){

  moveStraightUntilLine2(DIR_FWD, 190, 0, false);

  moveStraight(DIR_FWD, 0, 150, false);

  hitLineTurningRight2(1600);

  //attempts at self-correcting; rather hackish and should not be used

  //swing(DIR_FWD | TURN_LEFT, 0, 300);

  //turn(TURN_LEFT, 0, 100);

  moveStraight(DIR_FWD, 0, 1500, false);

  moveStraight(DIR_BACK, 0, 150, false);//align with dispenser
}
