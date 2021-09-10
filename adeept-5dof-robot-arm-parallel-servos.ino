/***********************************************************
 * Servos in Parallel: Demo for Adeept 5 DOF Robot Arm
 * https://www.adeept.com/adeept-arduino-compatible-diy-5-dof-robotic-arm-kit-for-arduino-uno-r3-steam-robot-arm-kit-with-arduino-and-processing-code_p0118.html
 * 
 * Modified from the example on millis() function
 * https://forum.arduino.cc/index.php?topic=223286.0
 * 
 * By Kemal Ihsan Kilic - October 2020
***********************************************************/
#include <Servo.h>

const int servoMinDeg[5] = {10, 20, 20, 20, 20}; // Servo min Limit
const int servoMaxDeg[5] = {170, 150, 150, 150, 150}; //Servo max Limit

int servoCurrPos[5] = {90, 90, 100, 90, 90}; // Current Position of servos
int servoSlowInterval[5] = {30, 30, 30, 30, 30}; // millisecs between servo moves
int servoFastInterval[5] = {10, 10, 10, 10, 10};
int servoInterval[5] =  {50, 50, 50, 50, 50}; // initial millisecs between servo moves
int servoStepDeg[5] = {1, 1, 1, 1, 1};   // amount servo moves at each step will be changed to negative value for movement in the other direction
int servopin[5] = {9, 6, 5, 3, 11}; //Define servo interfaces for digital interfaces
Servo servo[5];
unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long preMillis[5] = {0, 0, 0, 0, 0}; // the time when the servo was last moved



/********************************************************************************************************/
void setup() {

  int i;

  for (i = 0; i < 5; i++) {
    pinMode(servopin[i], OUTPUT); //Set the servo interface as the output interface
    servo[i].attach(servopin[i]);
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


  for (i = 0; i < 5; i++) {
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
