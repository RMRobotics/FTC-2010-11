#ifndef MOTORS
#include "motorsSensors.h" //motors references
#endif
#ifndef CONST
#include "constants.h" //constants for smoothness
#endif

#define LO

/*some macros:
#define getLeftPwr() motor[leftWheel]
#define getRightPwr() motor[rightWheel]
#define setLeftPwr(x) motor[leftWheel]=x
#define setRightPwr(x) motor[rightWheel]=x
//#define turnOffWheels() turnOnWheels(0, 0)*/

//functions that are implemented:
void setWheelPwr(short leftPwr, short rightPwr, short leftOmniPwr, short rightOmniPwr);
void changeWheelSpeed(float factor);
void turnOnWheels(byte mode, bool startAtFullPwr, int speed=MOTOR_SPEED);
void turnOffWheels();
void turnOnBelt();
task slowDown();
task speedUp();
void initializeServos();
void moveBridgeTo(short value);
void moveDispenserTo(short encoderValue);
void moveScoreBoxTo(short value);
void openScoreBoxDoor();
void closeScoreBoxDoor();
void openDispenserBox();
void closeDispenserBox();

//implementations:
void turnOnWheels(byte mode, bool startAtFullPwr, int speed){
  int dir, turn;
  if(!startAtFullPwr)
    speed/=SMOOTH_FACTOR;
  if((mode & DIR_FWD)!=0)
    dir=1;
  else
    dir=-1;

  if((mode & TURN_LEFT)!=0)
    turn=-1;
  else
    turn=1;

  switch(mode&MODE_NOFLAGS){
    case MODE_MOVE_STRAIGHT:
      setWheelPwr(dir*speed, dir*speed, dir*speed, dir*speed);
    break;
    case MODE_PIVOT:
      setWheelPwr(turn*speed, -turn*speed, turn*speed/2, -turn*speed/2);
    break;
    case MODE_SWING:
      if(dir==1){//forward
        if(turn==1){//swing right (left side moves)
          setWheelPwr(speed, 0, speed, 0);
        }else{//swing left (right side moves)
          setWheelPwr(0, speed, 0, speed);
        }
      }else{
        if(turn==1){//swing right (right side moves)
          setWheelPwr(0, -speed, 0, -speed);
        }else{//swing left (left side moves)
          setWheelPwr(-speed, 0, -speed, 0);
        }
      }
    break;
  }
  return;
}

void setWheelPwr(short leftPwr, short rightPwr, short leftOmniPwr, short rightOmniPwr){
  motor[leftWheel]=leftPwr;
  motor[rightWheel]=rightPwr;
  motor[leftOmniWheel]=leftOmniPwr;
  motor[rightOmniWheel]=rightOmniPwr;
}

void changeWheelSpeed(float factor){
  short lp=motor[leftWheel];
  short rp=motor[rightWheel];
  short lop=motor[rightOmniWheel];
  short rop=motor[rightOmniWheel];
  short leftPwr=((short)(lp*factor));
  short rightPwr=((short)(rp*factor));
  short leftOmniPwr=((short)(lop*factor));
  short rightOmniPwr=((short)(rop*factor));

#ifdef SMOOTH
  while(lp!=leftPwr||rp!=rightPwr||lop!=leftOmniPwr||rop!=rightOmniPwr){
    if(lp<leftPwr)
      motor[leftWheel]=++lp;
    else
      if(lp>leftPwr)
        motor[leftWheel]=--lp;
    if(rp<rightPwr)
      motor[rightWheel]=++rp;
    else
      if(rp>rightPwr)
        motor[rightWheel]=--rp;

    if(lop<leftOmniPwr)
      motor[leftOmniWheel]=++lop;
    else
      if(lop>leftOmniPwr)
        motor[leftOmniWheel]=--lop;
    if(rop<rightOmniPwr)
      motor[rightOmniWheel]=++rop;
    else
      if(rop>rightOmniPwr)
        motor[rightOmniWheel]=--rop;

    wait1Msec(SMOOTH_TIME);
  }
#endif
  motor[leftWheel]=leftPwr;
  motor[rightWheel]=rightPwr;
  motor[leftOmniWheel]=leftOmniPwr;
  motor[rightOmniWheel]=rightOmniPwr;

  return;
}

void turnOffWheels(){
  setWheelPwr(0, 0, 0, 0);
}

task slowDown(){
  changeWheelSpeed(1.0/SMOOTH_FACTOR);
}

task speedUp(){
  changeWheelSpeed(SMOOTH_FACTOR);
}

void initializeServos(){
  servo[bridgeServo]=SERVO_BRIDGE_UP;
  servo[dispenserDoor]=SERVO_DISPENSER_DOOR_CLOSED;
  servo[scoreBoxTilt]=SERVO_SCORE_BOX_TILT_MID;
}

void moveBridgeTo(short value){
  servo[bridgeServo]=value;
}

void moveDispenserTo(short encoderValue, short wait){
  nMotorEncoderTarget[belt]=abs(encoderValue-nMotorEncoder[belt]);//positive value to stop at position
  if(encoderValue<0)
    motor[belt]=-100;
  else
    motor[belt]=100;
  wait1Msec(wait);
  motor[belt]=0;
  nMotorEncoderTarget[belt]=0;
}

void openScoreBoxDoor(){
  nMotorEncoderTarget[boxDoor]=100;
  motor[boxDoor]=50;
  wait10Msec(100);
  motor[boxDoor]=0;
}

void closeScoreBoxDoor(){
  nMotorEncoderTarget[boxDoor]=100;
  motor[boxDoor]=-50;
  wait10Msec(100);
  motor[boxDoor]=0;
}

void openDispenserBox(){
  servo[dispenserDoor]=SERVO_DISPENSER_DOOR_OPEN;
}

void closeDispenserBox(){
  servo[dispenserDoor]=SERVO_DISPENSER_DOOR_CLOSED;
}

void moveScoreBoxTo(short value){
#ifdef SMOOTH
  short cur=ServoValue[scoreBoxTilt];
  while(cur!=value){
    if(cur<value)
      cur++;
    if(cur>value)
      cur--;
    servo[scoreBoxTilt]=cur;
    wait1Msec(SMOOTH_TIME);
  }
#endif
  servo[scoreBoxTilt]=value;
}
