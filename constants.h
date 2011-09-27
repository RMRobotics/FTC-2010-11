#define CONST

#define SMOOTH //in auton, should robot start smoothly?
#define DEBUG //show debug output on screen; this may take away reaction time

#ifdef SMOOTH
  #define SMOOTH_TIME 20 //wait this many msec per unit of power (up to 100) to slow down
  #define SMOOTH_FACTOR 2 //this is how much speedup and slowdown actually change the wheel speed
#endif

#define LIGHT_DIFF 15 //min. difference between readings for black vs. white on the field

#define MOTOR_SPEED 40 //default motor speed for auton
#define TURN_DST_OFFSET 2 //+- 2 cm of expected value when turning left/right
#define CORRECT_MAX_TIME 300 //max time robot has to self-correct per move (ie to move back to a line it overshot)
#define CORRECT_MAX_ATTEMPTS 4 //max number of moves in self-correct routine (each move has two components, of going backward to compensate for overshoot then going forward to de-compensate for previous move if necessary)

#define MODE_MOVE_STRAIGHT 0
#define MODE_PIVOT 1
#define MODE_SWING 2
#define MODE_NOFLAGS 15

#define TURN_LEFT 16
#define TURN_RIGHT 0
#define DIR_FWD 32
#define DIR_BACK 0

#define SERVO_DISPENSER_DOOR_CLOSED 130
#define SERVO_DISPENSER_DOOR_OPEN 250
#define SERVO_BRIDGE_UP 30
#define SERVO_BRIDGE_DOWN 255
#define SERVO_SCORE_BOX_TILT_MID 175
#define SERVO_SCORE_BOX_TILT_STAT 220
#define SERVO_SCORE_BOX_TILT_ROLL 130

#define DISPENSER_ENC_INIT 0
#define DISPENSER_ENC_MID -800
#define DISPENSER_ENC_FLIP -2550
#define DISPENSER_ENC_TOP -1100
