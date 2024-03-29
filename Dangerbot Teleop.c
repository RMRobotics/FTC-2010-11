#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S2,     BackLight,           sensorLightActive)
#pragma config(Sensor, S3,     FrontLight,          sensorLightActive)
#pragma config(Sensor, S4,     FrontLight2,         sensorLightActive)
#pragma config(Motor,  motorA,          LegoDoor,      tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          LeftHook,      tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorC,          RightHook,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     BackRight,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     BackLeft,      tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     FrontRight,    tmotorNormal, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     FrontLeft,     tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     Belt,          tmotorNormal, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorNormal, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    Bridge,               tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    Box,                  tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    Door,                 tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*

////////////////////////////////////////////////////////////////////////////////////
//                                                 ,w.                            //
//                                               ,YWMMw  ,M  ,                    //
//                          _.---.._   __..---._.'MMMMMw,wMWmW,                   //
//                     _.-""        """           YP"WMMMMMMMMMb,                 //
//                  .-' __.'                   .'     MMMMW^WMMMM;                //
//      _,        .'.-'"; `,       /`     .--""      :MMM[==MWMW^;                //
//   ,mM^"     ,-'.'   /   ;      ;      /   ,       MMMMb_wMW"  @\               //
//  ,MM:.    .'.-'   .'     ;     `\    ;     `,     MMMMMMMW `"=./`-,            //
//  WMMm__,-'.'     /      _.\      F"""-+,,   ;_,_.dMMMMMMMM[,_ / `=_}           //
//  "^MP__.-'    ,-' _.--""   `-,   ;       \  ; ;MMMMMMMMMMW^``; __|             //
//             /   .'            ; ;         )  )`{  \ `"^W^`,   \  :             //
//            /  .'             /  (       .'  /     Ww._     `.  `"              //
//           /  Y,              `,  `-,=,_{   ;      MMMP`""-,  `-._.-,           //
//          (--, )                `,_ / `) \/"")      ^"      `-, -;"\:           //
//           `"""                    `"""   `"'                  `---"            //
////////////////////////////////////////////////////////////////////////////////////

          // Lev Kravtsov, Wednesday, March 02, 2011 //
                                                                                    */


#include "JoystickDriver.c"
                                       //change values before using program
//motor Speed
  #define WheelSpeed 100
  #define BeltSpeed 40
  #define HookSpeed 30
  #define LineupSpeed 50
//Box On Belt open/closed
  #define BeltDoorClosed 75
  #define BeltDoorOpen 20
//Scoring Box
  #define BoxCollect 180
  #define RollingDeposit 130
  #define StationaryDeposit 200
  #define LegoBoxOpen 10       //encoder value
  #define LegoBoxClosed 20     //encoder value
//Bridge arm position
  #define BridgeUp 30
  #define BridgeDown 220
//Hooks closed/opened
  #define HooksOpened 0      //encoder value
  #define HooksClosed -50      //encoder value

//Add Dispensor Lineup

  task main()
{
  int forwardRight, forwardLeft, reverse=1, BeltAdjusted;

  nMotorEncoder[LeftHook]=0;
  servo[Box]=BoxCollect;
  servo[Bridge]=BridgeUp;
  servo[Door]=BeltDoorClosed;


 while(true)
  {
  getJoystickSettings(joystick);

 //line up robot so that it's centered for the 7" dispenser
   /* if(joy1Btn(9)==1){                       //side 1 (turns left)
      motor[BackRight]=LineupSpeed;
	    motor[BackLeft]=LineupSpeed;
	    motor[FrontRight]=LineupSpeed;
	    motor[FrontLeft]=LineupSpeed;
	  }
        if(SensorValue(BackLight)>60){
          motor[BackRight]=0;
	        motor[BackLeft]=0;
	        motor[FrontRight]=0;
	        motor[FrontLeft]=0;
          wait10Msec(5);
          motor[BackRight]=LineupSpeed;
          motor[BackLeft]=-LineupSpeed;
      }
          if(SensorValue(FrontLight)>90){
            motor[BackRight]=0;
            motor[BackLeft]=0;
         }
     if(joy1Btn(10)==1){                       //side 2 (turns right)
      motor[BackRight]=LineupSpeed;
	    motor[BackLeft]=LineupSpeed;
	    motor[FrontRight]=LineupSpeed;
	    motor[FrontLeft]=LineupSpeed;
	  }
        if(SensorValue(BackLight)>90){
          motor[BackRight]=0;
	        motor[BackLeft]=0;
	        motor[FrontRight]=0;
	        motor[FrontLeft]=0;
          wait10Msec(5);
          motor[BackRight]=-LineupSpeed;
          motor[BackLeft]=LineupSpeed;
      }
          if(SensorValue(FrontLight)>60){
            motor[BackRight]=0;
            motor[BackLeft]=0;
         }
*/
 //movement (needs to be edited)
    forwardLeft=(reverse*joystick.joy1_y1+joystick.joy1_x1)/2;
    forwardRight=(reverse*joystick.joy1_y1-joystick.joy1_x1)/2;
    if(abs(forwardLeft)<10){
      forwardLeft=0;}
    if(abs(forwardRight)<10){
      forwardRight=0;}

    if(joy1Btn(7)==1){                      //slows down movement of robot when button 7 is pressed
      forwardRight/=4;
      forwardLeft/=4;
    }
    motor[BackRight]=forwardRight;          //robot moves front wheels and back wheels at same time/speed
	  motor[BackLeft]=forwardLeft;           //therefor center of turning is in the middle
	  motor[FrontRight]=forwardRight;
	  motor[FrontLeft]=forwardLeft;


 //bridge arm
   if(joystick.joy1_TopHat==0){              //Move Bridge Arm Up/Down
      servoChangeRate[Bridge]=10;
      servo[Bridge]=BridgeUp;        //how to make servos move slowly, and stop when button released? now they just go to their value
    }else{
      if(joystick.joy1_TopHat==4){
        servo[Bridge]=BridgeDown;
      }else{
          servo[Bridge]=BridgeUp;
    }servoChangeRate[Bridge]=0;
   }
   if(joy1Btn(4)==1){                        //move bridge arm to lower bridge
		  servo[Bridge]=BridgeDown;
		}else{
		  servo[Bridge]=BridgeUp;
		}


 //Lego Hooks for rolling goal
		if(joy1Btn(3)==1){                       //move hooks to hold the goal if they r opened, if closed, opens them
		  if (nMotorEncoder[LeftHook]>HooksOpened){   //how to make it less than or equal to?
		    motor[LeftHook]=-HookSpeed;              //also check the greater than/or less than signs after the variables r defined
		    motor[RightHook]=-HookSpeed;
		     if(nMotorEncoder[LeftHook]==HooksClosed){
	        motor[LeftHook]=0;
	        motor[RightHook]=0;
	      }
		  }else{
		  motor[LeftHook]=HookSpeed;
		  motor[RightHook]=HookSpeed;
		    if(nMotorEncoder[LeftHook]==HooksOpened){
	        motor[LeftHook]=0;
	        motor[RightHook]=0;
	      }
	     }
	    }                                      //move hooks
   	if(joystick.joy1_TopHat==6){ //which value is left arrow?
   	  motor[LeftHook]=HookSpeed;
   	  motor[RightHook]=HookSpeed;
   	}else{
      if(joystick.joy1_TopHat==2){ //which value is right arrow?
        motor[LeftHook]=-HookSpeed;
        motor[RightHook]=-HookSpeed;
     }else{
        motor[LeftHook]=0;
        motor[RightHook]=0;
      }
	  }


  //Scoring Box
	  if(joy1Btn(8)==1){                     //open scoring box
	    motor[LegoDoor]=100;
	    while(nMotorEncoder[LegoDoor]<LegoBoxOpen){
	      wait1Msec(5);
	     }
	   }else{
	      motor[LegoDoor]=-50;
	      while(nMotorEncoder[LegoDoor]>LegoBoxClosed){
	        wait1Msec(5);
	      }
	     }
	  if(joy1Btn(2)==1){                     //move box to the rolling goal deposit position, press again to move back
	    servo[Box]=RollingDeposit;
	  }
	  if(joy1Btn(1)==1){                     //move box to the stationary deposit from the bridge, release to go back
	    servoChangeRate[Box]=5;
	    servo[Box]=StationaryDeposit;
	    }else{
	     servoChangeRate[Box]=0;
	      servo[Box]=BoxCollect;
	   }


	//belt+box controlls
	  if(joy1Btn(5)==1){                      //slow down belt speed
	    BeltAdjusted=BeltSpeed;
	    BeltAdjusted/=4;
	  }

	  if(abs(joystick.joy1_y2)>10){           //move belt using the right joystick
      motor[Belt]=BeltAdjusted*sgn(joystick.joy1_y2);
  }else{
      motor[Belt]=0;
	}
	  if(joy1Btn(6)==1){                      //open door to the box on belt, let go to close
	    servo[Door]=BeltDoorOpen;
	  }else{
	    servo[Door]=BeltDoorClosed;
	  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Joystick 2 controls                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	  //scoring box
	  if(joy2Btn(8)==1){                     //open scoring box
	    motor[LegoDoor]=100;
	    while(nMotorEncoder[LegoDoor]<LegoBoxOpen){
	      wait1Msec(5);
	     }
	   }else{
	      motor[LegoDoor]=-50;
	      while(nMotorEncoder[LegoDoor]>LegoBoxClosed){
	        wait1Msec(5);
	      }
	     }
	  if(joy2Btn(2)==1){                     //move box to the rolling goal deposit position, press again to move back
	    servo[Box]=RollingDeposit;
	  }
	  if(joy2Btn(1)==1){                     //move box to the stationary deposit from the bridge, release to go back
	    servoChangeRate[Box]=5;
	    servo[Box]=StationaryDeposit;
	    }else{
	     servoChangeRate[Box]=0;
	      servo[Box]=BoxCollect;
	   }

	//belt+box controlls
	  if(joy2Btn(5)==1){                      //slow down belt speed
	    BeltAdjusted=BeltSpeed;
	    BeltAdjusted/=4;
	  }
	  if(abs(joystick.joy2_y2)>10){           //move belt using the right joystick
      motor[Belt]=BeltAdjusted*sgn(joystick.joy1_y2);
  }else{
      motor[Belt]=0;
	}
	  if(joy2Btn(6)==1){                      //open door to the box on belt, let go to close
	    servo[Door]=BeltDoorOpen;
	  }else{
	    servo[Door]=BeltDoorClosed;
	  }



	wait10Msec(2);
  }
}
