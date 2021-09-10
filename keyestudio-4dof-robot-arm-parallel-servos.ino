/***********************************************************
   Servos in Parallel: Demo for keyestudio 4 DOF Robot Arm
   https://wiki.keyestudio.com/Ks0198_keyestudio_4DOF_Robot_Mechanical_Arm_Kit_for_Arduino_DIY
   Modified from the example on millis() function
   https://forum.arduino.cc/index.php?topic=223286.0

   By Kemal Ihsan Kilic - September 2021
***********************************************************/
#include <Servo.h>

const int servoMinDeg[4] = {0, 0, 0, 0}; // Servo min Limit
const int servoMaxDeg[4] = {180, 180, 180, 180}; //Servo max Limit

int servoCurrPos[4] = {90, 90, 90, 90}; // Current Position of servos
int servoSlowInterval[4] = {30, 30, 30, 30}; // millisecs between servo moves
int servoFastInterval[4] = {10, 10, 10, 10};
int servoInterval[4] =  {50, 50, 50, 50}; // initial millisecs between servo moves
int servoStepDeg[4] = {1, 1, 1, 1};   // amount servo moves at each step will be changed to negative value for movement in the other direction
//int servopin[4] = {A0, 6, A1, 9}; //Define servo interfaces for digital interfaces
Servo servo[4];
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long preMillis[4] = {0, 0, 0, 0}; // the time when the servo was last moved



/********************************************************************************************************/
void setup() {

  int i;

  pinMode(A0, OUTPUT); //Set the servo interface as the output interface
  pinMode(6, OUTPUT); //Set the servo interface as the output interface
  pinMode(A1, OUTPUT); //Set the servo interface as the output interface
  pinMode(9, OUTPUT); //Set the servo interface as the output interface

  //right_back_forward.attach(A0);
  //left_up_down.attach(6);
  //base_rotate.attach(A1);
  //claw.attach(9);

  servo[0].attach(A0);
  servo[1].attach(6);
  servo[2].attach(A1);
  servo[3].attach(9);

  for (i = 0; i < 4; i++) {
    servo[i].write(servoCurrPos[i]);
  }

  Serial.begin(9600);
}



/********************************************************************************************************/


void loop()
{
  int i;
  // Notice that none of the action happens in loop() apart from reading millis()
  // it just calls the functions that have the action code

  currentMillis = millis();   // capture the latest value of millis() this is equivalent to noting the time from a clock


  for (i = 0; i < 4; i++) {
    servoLoop(i);
  }


}


/********************************************************************************************************/



void servoLoop(int i) {

  // this is similar to the servo sweep example except that it uses millis() rather than delay()
  // nothing happens unless the interval has expired
  // the value of currentMillis was set in loop()

  if (currentMillis - preMillis[i] >= servoInterval[i]) {
    // its time for another move
    preMillis[i] += servoInterval[i];

    servoCurrPos[i] = servoCurrPos[i] + servoStepDeg[i]; // servoDegrees might be negative

    if (servoCurrPos[i] <= servoMinDeg[i]) {
      // when the servo gets to its minimum position change the interval to change the speed
      if (servoInterval[i] == servoSlowInterval[i]) {
        servoInterval[i] = servoFastInterval[i];
      }
      else {
        servoInterval[i] = servoSlowInterval[i];
      }
    }
    if ((servoCurrPos[i] >= servoMaxDeg[i]) || (servoCurrPos[i] <= servoMinDeg[i]))  {
      // if the servo is at either extreme change the sign of the degrees to make it move the other way
      servoStepDeg[i] = - servoStepDeg[i]; // reverse direction
      // and update the position to ensure it is within range
      servoCurrPos[i] = servoCurrPos[i] + servoStepDeg[i];
    }
    // make the servo move to the next position
    servo[i].write(servoCurrPos[i]);
    // and record the time when the move happened
  }


}


/********************************************************************************************************/
