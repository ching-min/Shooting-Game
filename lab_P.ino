#include <Servo.h> 

Servo servoRL;
//Servo servoUD;

int A=0;//0
int B=0;//1
int C=0;//2
int D=0;//3
int E=0;//4

// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register 
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

//TODO:
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5

// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define RELEASE 3

int select=0;
int pwm=0;


void setup() {
  servoRL.attach(9);//right & left
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  servoRL.write(80);
}

void loop() {
 
  A=digitalRead(A0);
  B=digitalRead(A1);
  C=digitalRead(A2);
  D=digitalRead(A3);
  E=digitalRead(A4);
  
//  servoRL.write(70);	
  
  if(A==HIGH){
    
//    servoRL.write(90);
//    delay(10);
    servoRL.write(95);
    delay(7000);
//    servoRL.write(85);
//    delay(10);  
 //   servoRL.write(70);
	
  }
  if(B==HIGH){
    servoRL.write(80);
    
    delay(7000);
  }
  if(C==HIGH){
//    servoRL.write(50);
//    delay(10);
 //   servoRL.write(50);
 //   delay(10);
    servoRL.write(45);
    delay(7000);
 //   servoRL.write(65);
//    delay(10);
//    servoRL.write(70);
//    delay(10);
	
  }
  if(D==HIGH){
//    servoRL.write(90);
//    delay(10);
    servoRL.write(95);
    delay(7000);
//    servoRL.write(85);
//    delay(10);  
//    servoRL.write(70);		
	
  }
  if(E==HIGH){
    servoRL.write(80);
    
    delay(7000);
  }
}


void motor(int nMotor, int command, int speed)
{
  int motorA, motorB;

  if (nMotor >= 1 && nMotor <= 4)
  {  
    switch (nMotor)
    {
    case 1:
      motorA   = MOTOR1_A;//2
      motorB   = MOTOR1_B;//3
      break;
    case 2:
      motorA   = MOTOR2_A;//1
      motorB   = MOTOR2_B;//4
      break;
    case 3:
      motorA   = MOTOR3_A;//5
      motorB   = MOTOR3_B;//7
      break;
    case 4:
      motorA   = MOTOR4_A;//0
      motorB   = MOTOR4_B;//6
      break;
    default:
      break;
    }

    switch (command)
    {
    case FORWARD:
      motor_output (motorA, HIGH, speed);//HIGH ->left
      motor_output (motorB, LOW, -1);     // -1: no PWM set
      break;
    case BACKWARD:
      motor_output (motorA, LOW, speed);//LOW -> right
      motor_output (motorB, HIGH, -1);    // -1: no PWM set
      break;
    case RELEASE:
      motor_output (motorA, LOW, 0);  // 0: output floating.
      motor_output (motorB, LOW, -1); // -1: no PWM set
      break;
    default:
      break;
    }
  }
}
// ---------------------------------
// motor_output
void motor_output (int output, int high_low, int speed)
{

  switch (output)
  {
  case MOTOR1_A:
  case MOTOR1_B:
    pwm = MOTOR1_PWM;
    break;
  case MOTOR2_A:
  case MOTOR2_B:
    pwm = MOTOR2_PWM;
    break;
  case MOTOR3_A:
  case MOTOR3_B:
    pwm = MOTOR3_PWM;
    break;
  case MOTOR4_A:
  case MOTOR4_B:
    pwm = MOTOR4_PWM;
    break;
  default:
    // Use speed as error flag, -3333 = invalid output.
    speed = -3333;
    break;
  }

  if (speed != -3333)
  {
    shiftWrite(output, high_low);

    if (speed >= 0 && speed <= 255)    
    {
      //TODO:
      analogWrite(pwm,speed);
    }
  }
}


// ---------------------------------
// shiftWrite
void shiftWrite(int output, int high_low)
{
  static int latch_copy;
  static int shift_register_initialized = false;

  // Do the initialization on the fly, 
  // at the first time it is used.
  if (!shift_register_initialized)
  {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);

    // Set pins for shift register to default value (low);
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin Low.
    digitalWrite(MOTORENABLE, LOW);

    // start with all outputs (of the shift register) low
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.
  bitWrite(latch_copy, output, high_low);

  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, LOW);
}



