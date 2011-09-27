#ifndef MOTORS
#include "motorsSensors.h"
#endif
#ifndef LO
#include "lowLevelMoves.c"
#endif
#ifndef CONST
#include "constants.h"
#endif

#define HI

//functions to be implemented:
void moveStraight(byte dir, short stopAtDist, short stopAtMsec, bool stopAtAbsInc);
void turn(byte turn, short stopAtDist, short stopAtMsec);
void swing(byte turn_dir, short stopAtDist, short stopAtMsec);
void moveStraightUntilLine(byte dir, short stopAtDist, short stopAtMsec, bool stopAtAbsInc=false);
void hitLineTurningLeft(short stopAtMsec);
void hitLineTurningRight(short stopAtMsec);
void sensorsDebug(bool wait=false);
void putDownBridge(bool down=true);

//implementations:
void moveStraight(byte dir, short stopAtDist, short stopAtMsec, bool stopAtAbsInc){//if either of these are 0, they are ignored
	turnOnWheels(MODE_MOVE_STRAIGHT | dir, false);//start at low power
	StartTask(speedUp);
	ClearTimer(T1);
	int lastD, temp=255;
	bool slow=false;
	do{
	  lastD=temp;
	  sensorsDebug();
	  wait1Msec(50);
	  temp=abs(SensorValue[distanceSensor]-stopAtDist);
#ifdef SMOOTH
    if(SensorValue[distanceSensor]!=255&&stopAtDist!=0){
      if(temp<20){
        StopTask(speedUp);
        StartTask(slowDown);
        slow=true;
      }
    }
#endif
	}while((!stopAtAbsInc||temp<=lastD||SensorValue[distanceSensor]==255)&&(stopAtDist==0||temp>2)&&(stopAtMsec==0||time1[T1]<stopAtMsec));

  if(slow)
    StopTask(slowDown);
  else
    StopTask(speedUp);

	turnOffWheels();
}

void moveStraightUntilLine(byte dir, short stopAtDist, short stopAtMsec, bool stopAtAbsInc){//if either of these are 0, they are ignored
	turnOnWheels(MODE_MOVE_STRAIGHT | dir, false);
	StartTask(speedUp);
	ClearTimer(T1);
	int lastD, temp=255, change=0, line=0, maxVal=0, attempts=0;
	short frontDef=SensorValue[frontLightSensor], backDef=SensorValue[backLightSensor];

	do{
	  lastD=temp;
	  sensorsDebug();
	  //wait1Msec(50);
	  temp=abs(SensorValue[distanceSensor]-stopAtDist);
#ifdef SMOOTH
    if(SensorValue[distanceSensor]!=255&&stopAtDist!=0){
      if(temp<40){
        if(temp<10&&change==2){
          changeWheelSpeed(0.5);
          change=3;
        }else
          if(change==1){
            changeWheelSpeed(0.5);
            change=2;
          }
      }else
        if(temp<60&&change==0){
          changeWheelSpeed(0.5);
          change=1;
        }
    }
#endif

    if(SensorValue[backLightSensor]>=maxVal){
      //hitting from back; this is centering sensor so stop
      maxVal=SensorValue[backLightSensor];
    }else{
      if(maxVal>(backDef+LIGHT_DIFF)){
        if(line==1)
          StopTask(slowDown);
        line=2;
      }
    }

    if((SensorValue[frontLightSensor]>frontDef+LIGHT_DIFF)&&line==0){
      //hitting from front; this is first pass
#ifdef SMOOTH
       StopTask(speedUp);//from beginning it speeds up
       StartTask(slowDown);
       line=1;
#endif
    }

	}while((line!=2)&&(!stopAtAbsInc||temp<=lastD)&&(stopAtDist==0||temp>2)&&(stopAtMsec==0||time1[T1]<stopAtMsec));

	StopTask(speedUp);//from beginning
	turnOffWheels();

	while(line==2&&attempts<CORRECT_MAX_ATTEMPTS){
	  wait10Msec(50);
		if(line==2&&SensorValue[backLightSensor]<maxVal){
	    line=1;
		  turnOnWheels(MODE_MOVE_STRAIGHT | (dir^DIR_FWD), false);
		  StartTask(speedUp);
		  ClearTimer(T2);
	    while((SensorValue[backLightSensor]<maxVal)&&(time1[T2]<CORRECT_MAX_TIME)){
	      if(SensorValue[backLightSensor]>=maxVal)
	        line=2;
	      wait10Msec(1);
	    }
	    StopTask(speedUp);
	    turnOffWheels();
	  }
	  wait10Msec(50);
		if(line==2&&SensorValue[backLightSensor]<maxVal){
	    line=1;
		  turnOnWheels(MODE_MOVE_STRAIGHT | dir, false);
		  StartTask(speedUp);
		  ClearTimer(T2);
	    while((SensorValue[backLightSensor]<maxVal)&&(time1[T2]<CORRECT_MAX_TIME)){
	      if(SensorValue[backLightSensor]>=maxVal)
	        line=2;
	      wait10Msec(1);
	    }
	    StopTask(speedUp);
	    turnOffWheels();
	  }
  }
}

void moveStraightUntilLine2(byte dir, short stopAtDist, short stopAtMsec, bool stopAtAbsInc){//if either of these are 0, they are ignored
	turnOnWheels(MODE_MOVE_STRAIGHT | dir, false);
	StartTask(speedUp);
	ClearTimer(T1);
	int lastD, temp=255, change=0, line=0, maxVal=0, attempts=0;
	short frontDef=SensorValue[frontLightSensor], backDef=SensorValue[backLightSensor];

	do{
	  lastD=temp;
	  sensorsDebug();
	  //wait1Msec(50);
	  temp=abs(SensorValue[distanceSensor]-stopAtDist);
#ifdef SMOOTH
    if(SensorValue[distanceSensor]!=255&&stopAtDist!=0){
      if(temp<40){
        if(temp<10&&change==2){
          changeWheelSpeed(0.5);
          change=3;
        }else
          if(change==1){
            changeWheelSpeed(0.5);
            change=2;
          }
      }else
        if(temp<60&&change==0){
          changeWheelSpeed(0.5);
          change=1;
        }
    }
#endif

    if(SensorValue[frontLightSensor]>=maxVal){
      //hitting from back; this is centering sensor so stop
      maxVal=SensorValue[frontLightSensor];
    }else{
      if(maxVal>(frontDef+LIGHT_DIFF)){
        if(line==1)
          StopTask(slowDown);
        line=2;
      }
    }

    if((SensorValue[backLightSensor]>backDef+LIGHT_DIFF)&&line==0){
      //hitting from front; this is first pass
#ifdef SMOOTH
       StopTask(speedUp);//from beginning it speeds up
       StartTask(slowDown);
       line=1;
#endif
    }

	}while((line!=2)&&(!stopAtAbsInc||temp<=lastD)&&(stopAtDist==0||temp>2)&&(stopAtMsec==0||time1[T1]<stopAtMsec));

	StopTask(speedUp);//from beginning
	turnOffWheels();

	while(line==2&&attempts<CORRECT_MAX_ATTEMPTS){
	  wait10Msec(50);
		if(line==2&&SensorValue[frontLightSensor]<maxVal){
	    line=1;
		  turnOnWheels(MODE_MOVE_STRAIGHT | (dir^DIR_FWD), false);
		  StartTask(speedUp);
		  ClearTimer(T2);
	    while((SensorValue[frontLightSensor]<maxVal)&&(time1[T2]<CORRECT_MAX_TIME)){
	      if(SensorValue[frontLightSensor]>=maxVal)
	        line=2;
	      wait10Msec(1);
	    }
	    StopTask(speedUp);
	    turnOffWheels();
	  }
	  wait10Msec(50);
		if(line==2&&SensorValue[frontLightSensor]<maxVal){
	    line=1;
		  turnOnWheels(MODE_MOVE_STRAIGHT | dir, false);
		  StartTask(speedUp);
		  ClearTimer(T2);
	    while((SensorValue[frontLightSensor]<maxVal)&&(time1[T2]<CORRECT_MAX_TIME)){
	      if(SensorValue[frontLightSensor]>=maxVal)
	        line=2;
	      wait10Msec(1);
	    }
	    StopTask(speedUp);
	    turnOffWheels();
	  }
  }
}

void turn(byte turn, short stopAtDist, short stopAtMsec){//robot turns around center point; encoder value from rightWheel equiv is given
  turnOnWheels(MODE_PIVOT | turn, true);
  ClearTimer(T1);
  do{
    wait10Msec(1);
  }while((stopAtDist==0||abs(SensorValue[distanceSensor]-stopAtDist)>TURN_DST_OFFSET)&&(stopAtMsec==0||time1[T1]<stopAtMsec));
  turnOffWheels();
}

void swing(byte turn_dir, short stopAtDist, short stopAtMsec){//robot swings right side over, right wheel encoder used
  turnOnWheels(MODE_SWING | turn_dir, true);
  ClearTimer(T1);
  do{
    wait10Msec(1);
  }while((stopAtDist==0||abs(SensorValue[distanceSensor]-stopAtDist)>TURN_DST_OFFSET)&&(stopAtMsec==0||time1[T1]<stopAtMsec));
  turnOffWheels();
}

void hitLineTurningLeft(short stopAtMsec){//based on front light sensor
  turnOnWheels(MODE_PIVOT | TURN_LEFT, true);
  int defVal=SensorValue[frontLightSensor], maxVal=0, attempts=0;
  bool cont=false;
  ClearTimer(T2);
  while((!cont)&&(stopAtMsec==0||time1[T2]<stopAtMsec)){//either one will trigger
    if(SensorValue[frontLightSensor]>=maxVal)
      maxVal=SensorValue[frontLightSensor];
    else
      if(maxVal>=(defVal+LIGHT_DIFF))
        cont=true;
		wait1Msec(10);
	}
  turnOffWheels();

  while(attempts<CORRECT_MAX_ATTEMPTS&&cont){//attempt to self-correct, with timeout
    wait10Msec(50);//wait to actually stop
    if(cont&&(SensorValue[frontLightSensor]<maxVal)){//overshoot
      cont=false;
	    turnOnWheels(MODE_PIVOT | TURN_RIGHT, false);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[frontLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	  wait10Msec(50);
	  if(cont&&(SensorValue[frontLightSensor]<maxVal)){//overshoot again
	    cont=false;
	    turnOnWheels(MODE_PIVOT | TURN_LEFT, false);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[frontLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	}
}

void hitLineTurningRight(short stopAtMsec){//based on front light sensor
  turnOnWheels(MODE_PIVOT | TURN_RIGHT, true);
  int defVal=SensorValue[frontLightSensor], maxVal=0, attempts=0;
  bool cont=false;
  ClearTimer(T2);
  while((!cont)&&(stopAtMsec==0||time1[T2]<stopAtMsec)){//either one will trigger
    if(SensorValue[frontLightSensor]>=maxVal)
      maxVal=SensorValue[frontLightSensor];
    else
      if(maxVal>=(defVal+LIGHT_DIFF))
        cont=true;
		wait1Msec(10);
	}
  turnOffWheels();

  while(attempts<CORRECT_MAX_ATTEMPTS&&cont){//attempt to self-correct, with timeout
    wait10Msec(50);
    if(cont&&(SensorValue[frontLightSensor]<maxVal)){//overshoot
      cont=false;
	    turnOnWheels(MODE_PIVOT | TURN_LEFT, false);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[frontLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	  wait10Msec(50);
	  if(cont&&(SensorValue[frontLightSensor]<maxVal)){//overshoot again
	    cont=false;
	    turnOnWheels(MODE_PIVOT | TURN_RIGHT, false);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[frontLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	}
}

void hitLineTurningRight2(short stopAtMsec){//based on front light sensor
  setWheelPwr(MOTOR_SPEED/3, -MOTOR_SPEED/3, MOTOR_SPEED, -MOTOR_SPEED);
  int defVal=SensorValue[backLightSensor], maxVal=0, attempts=0;
  bool cont=false;
  ClearTimer(T2);
  while((!cont)&&(stopAtMsec==0||time1[T2]<stopAtMsec)){//either one will trigger
    if(SensorValue[backLightSensor]>=maxVal)
      maxVal=SensorValue[backLightSensor];
    else
      if(maxVal>=(defVal+LIGHT_DIFF))
        cont=true;
		wait1Msec(10);
	}
  turnOffWheels();

  while(attempts<CORRECT_MAX_ATTEMPTS&&cont){//attempt to self-correct, with timeout
    wait10Msec(50);
    if(cont&&(SensorValue[backLightSensor]<maxVal)){//overshoot
      cont=false;
	    setWheelPwr(-MOTOR_SPEED/3, MOTOR_SPEED/3, -MOTOR_SPEED, MOTOR_SPEED);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[backLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	  wait10Msec(50);
	  if(cont&&(SensorValue[backLightSensor]<maxVal)){//overshoot again
	    cont=false;
	    setWheelPwr(MOTOR_SPEED/3, -MOTOR_SPEED/3, MOTOR_SPEED, -MOTOR_SPEED);
	    ClearTimer(T2);
	    while((!cont)&&(time1[T2]<CORRECT_MAX_TIME)){//either one will trigger
				wait1Msec(10);
				if(SensorValue[backLightSensor]>=maxVal)
				  cont=true;
			}
		  turnOffWheels();
		  attempts++;
	  }
	}
}

void sensorsDebug(bool wait){
  do{
  	nxtDisplayString(1, "ENC: %d, %d", nMotorEncoder[belt], nMotorEncoder[scoringBox]);
		nxtDisplayString(2, "DST: %d", SensorValue[distanceSensor]);
  	nxtDisplayString(3, "LGT: %d, %d", SensorValue[frontLightSensor], SensorValue[backLightSensor]);
 	  if(wait)
 	    wait10Msec(10);
	}while(wait&&nNxtButtonPressed!=3);
}

void putDownBridge(bool down){
  if(down)
    moveBridgeTo(SERVO_BRIDGE_DOWN);
  else
    moveBridgeTo(SERVO_BRIDGE_UP);
}

void dispenseFromMiddle(){
  moveDispenserTo(DISPENSER_ENC_MID, 500);
#ifdef DEBUG
  sensorsDebug();
#endif
  wait1Msec(500);
  moveDispenserTo(DISPENSER_ENC_INIT, 500);
#ifdef DEBUG
  sensorsDebug();
#endif
}

void dispenseFromTop(){
  moveDispenserTo(DISPENSER_ENC_TOP, 700);
#ifdef DEBUG
  sensorsDebug();
#endif
  wait1Msec(500);
  moveDispenserTo(DISPENSER_ENC_INIT+160, 700);
#ifdef DEBUG
  sensorsDebug();
#endif
}

void flipDispenserBox(){
  moveDispenserTo(DISPENSER_ENC_FLIP, 1500);
#ifdef DEBUG
  sensorsDebug();
#endif
  wait1Msec(1000);
  moveDispenserTo(DISPENSER_ENC_INIT, 1500);
#ifdef DEBUG
  sensorsDebug();
#endif
}

void scoreOnStat(){
  servo[scoreBoxTilt]=SERVO_SCORE_BOX_TILT_STAT;
  wait10Msec(100);
  servo[scoreBoxTilt]=SERVO_SCORE_BOX_TILT_MID;
}

void scoreOnRoll(){
  moveScoreBoxTo(SERVO_SCORE_BOX_TILT_ROLL);
  openScoreBoxDoor();
  wait10Msec(100);
  closeScoreBoxDoor();
  moveScoreBoxTo(SERVO_SCORE_BOX_TILT_MID);
}
