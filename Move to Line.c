#include "definitions.h"

#include "lowLevelMoves.c"

task main()
{
  alignFromBackRight();
}

void moveStraight(int stopAtEnc, int stopAtMsec){
	turnOnWheels(MOTOR_SPEED, MOTOR_SPEED);//left wheel will be linked to right wheel
	nMotorEncoder[rightWheel]=10;
	nMotorEncoder[leftWheel]=10;
	int last;
	ClearTimer(T1);
	float newValL=MOTOR_SPEED, newValR=MOTOR_SPEED;
	do{
		last=(nMotorEncoder[rightWheel]+nMotorEncoder[leftWheel]);
		wait1Msec(230);
		newValL=(newValL*((float)nMotorEncoder[rightWheel]))/(dif*(float)nMotorEncoder[leftWheel]);
		if(newValL>100){
		  newValL=100;
		}else{
		  if(newValL<1)
		    newValL=1;
		}
		motor[leftWheel]=((short)newValL);
		//motor[rightWheel]=((short)newValR);

#ifdef DEBUG
		nxtDisplayString(1, "ENC: %d, %d", nMotorEncoder[rightWheel], ((int)(dif*((float)nMotorEncoder[leftWheel]))));
		nxtDisplayString(2, "MOT: %d, %d", ((int)newValR), ((int)newValL));
#endif

	}while((stopAtEnc==0||abs((nMotorEncoder[rightWheel]+nMotorEncoder[leftWheel])/2)<stopAtEnc)&&(stopAtMsec==0||time1[T1]<stopAtMsec));

	motor[rightWheel]=0;
	motor[leftWheel]=0;
}

void moveStraightUntilLine(int threshold, bool slow, short dir, int endAtEnc){
	motor[rightWheel]=motorSpeed;
	motor[leftWheel]=motorSpeed;//this wheel will be linked to wheel 1
	nMotorEncoder[rightWheel]=1;
	nMotorEncoder[leftWheel]=1;
	int last;
	ClearTimer(T1);
	float newVal=motorSpeed, newVal2=motorSpeed, dif=4.4853;
	while((SensorValue[lightSensor1]<threshold)&&(SensorValue[lightSensor2]<threshold)&&abs(nMotorEncoder[leftWheel]-endAtEnc)>50){
		last=(nMotorEncoder[rightWheel]+nMotorEncoder[leftWheel]);
		wait1Msec(100);
		newVal=(newVal*((float)nMotorEncoder[rightWheel]))/(dif*(float)nMotorEncoder[leftWheel]);
		if(newVal>100){
		  newVal=100;
		}else{
		  if(newVal<1)
		    newVal=1;
		}
		motor[leftWheel]=((short)newVal);
		motor[rightWheel]=((short)newVal2);
		nxtDisplayString(1, "%d, %d", nMotorEncoder[rightWheel], ((int)(dif*((float)nMotorEncoder[leftWheel]))));
		nxtDisplayString(2, "%d, %d", ((int)newVal2), ((int)newVal));
	}
	motor[rightWheel]=0;
	motor[leftWheel]=0;
}

void hitLineFromS1(int threshold, bool slow, short dir, int endAtEnc){//right side
	if(!slow)
		motor[rightWheel]=motorSpeed*dir;
	else
		motor[rightWheel]=motorSpeed/2*dir;
	while(SensorValue[lightSensor1]<threshold&&abs(nMotorEncoder[rightWheel]-endAtEnc)>50){//either one will trigger
		wait1Msec(10);
	}
	motor[rightWheel]=0;
}

void hitLineFromS2(int threshold, bool slow, short dir, int endAtEnc){//left side
	if(!slow)
		motor[leftWheel]=motorSpeed*dir;
	else
		motor[leftWheel]=motorSpeed/2*dir;
	while(SensorValue[lightSensor2]<threshold&&abs(nMotorEncoder[leftWheel]-endAtEnc)>50){//either one will trigger stop
		wait1Msec(10);
	}
	motor[leftWheel]=0;
}

void alignFromBackRight(){//line is y-axis and robot is at -45 degree angle going towards origin. S1/rightWheel are on right of robot and positive value for motor moves robot towards origin. Encoder also greater when moving forward
	int defVal=(SensorValue[lightSensor2]+SensorValue[lightSensor1])/2;//avg starting value
	moveStraightUntilLine(defVal+diff, false, 1, 3000);
	nMotorEncoder[rightWheel]=0;
	nMotorEncoder[leftWheel]=0;
	hitLineFromS1(defVal+diff, true, 1, 1000);//hit from right side
	do{
		nMotorEncoder[rightWheel]=0;
		if(nMotorEncoder[leftWheel]>0)//left side is ahead, so swing back
			hitLineFromS2(defVal+diff, true, -1, 1000);
		else//left side moved backward, so swing forward
			hitLineFromS2(defVal+diff, true, 1, 1000);
		if(abs(nMotorEncoder[rightWheel])<100)
			break;//accurate enough
		nMotorEncoder[leftWheel]=0;
		if(nMotorEncoder[rightWheel]>0)//right side moved forward, so swing back
			hitLineFromS1(defVal+diff, false, -1, 0);
		else//left side moved backward, so swing forward
			hitLineFromS1(defVal+diff, false, 1, 0);
	}while(abs(nMotorEncoder[leftWheel])>100);//until little change
}
