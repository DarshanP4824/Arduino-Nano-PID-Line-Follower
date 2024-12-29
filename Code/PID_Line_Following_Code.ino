//Line Follower Code with TB6612FNG Motor Driver

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include <SparkFun_TB6612.h>

//Enter Line Details
bool isBlackLine = 1;          //keep 1 in case of black line. In case of white line change this to 0
unsigned int lineThickness = 25;  //Enter line thickness in mm. Works best for thickness between 10 & 35
unsigned int numSensors = 5;      // Enter number of sensors as 5 or 7
bool brakeEnabled = 0;

#define AIN1 4
#define BIN1 6
#define AIN2 3
#define BIN2 7
#define PWMA 9
#define PWMB 10
#define STBY 5 // This PIN is given to 3v3 Pin of the Arduino Nano (You can give it to the D5 Pin too).

// these constants are used to allow you to make your motor configuration
// line up with function names like forward. Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors. The library will allow you to initialize as many
// motors as you have memory for. If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);


int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfSpeed = 150;
int currentSpeed = 100;

float Kp = 0.05;
float Kd = 1.5;
float Ki = 0;

int onLine = 1;
int minValues[5], maxValues[5], threshold[5], sensorValue[5], sensorArray[5];
bool brakeFlag = 0;

void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  lineThickness = constrain(lineThickness, 10, 35);
}


void loop() {
  while (digitalRead(11)) {}
  delay(1000);
  calibrate();
  while (digitalRead(12)) {}
  delay(1000);

  while (1) {
    readLine();
    if (currentSpeed < lfSpeed) currentSpeed++;
    if (onLine == 1) {  //PID LINE FOLLOW
      linefollow();
      digitalWrite(13, HIGH);
      brakeFlag = 0;
    } else {
      digitalWrite(13, LOW);
      if (error > 0) {
        if (brakeEnabled == 1 && brakeFlag == 0) {
          motor1.drive(0);
          motor2.drive(0);
          delay(30);
        }
        motor1.drive(-100);
        motor2.drive(150);
        brakeFlag = 1;
      } else {
        if (brakeEnabled == 1 && brakeFlag == 0) {
          motor1.drive(0);
          motor2.drive(0);
          delay(30);
        }
        motor1.drive(150);
        motor2.drive(-100);
        brakeFlag = 1;
      }
    }
  }
}

void linefollow() {
  if (numSensors == 7) {
    error = (3 * sensorValue[0] + 2 * sensorValue[1] + sensorValue[2] - sensorValue[4] - 2 * sensorValue[5] - 3 * sensorValue[6]);
  }
  if (numSensors == 5) {
    error = (3 * sensorValue[1] + sensorValue[2] - sensorValue[4] - 3 * sensorValue[5]);
  }
  if (lineThickness > 22) {
    error = error * -1;
  }
  if (isBlackLine) {
    error = error * -1;
  }

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = currentSpeed - PIDvalue;
  rsp = currentSpeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  motor1.drive(lsp);
  motor2.drive(rsp);
}



void calibrate() {
  for (int i = 0; i < 5; i++) {
    minValues[i] = analogRead(i);
    maxValues[i] = analogRead(i);
  }

  for (int i = 0; i < 10000; i++) {
    motor1.drive(120);
    motor2.drive(-120);

    for (int i = 0; i < 5; i++) {
      if (analogRead(i) < minValues[i]) {
        minValues[i] = analogRead(i);
      }
      if (analogRead(i) > maxValues[i]) {
        maxValues[i] = analogRead(i);
      }
    }
  }

  for (int i = 0; i < 5; i++) {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print(" ");
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);
}

void readLine() {
  onLine = 0;
  if (numSensors == 7) {
    for (int i = 0; i < 7; i++) {
      sensorValue[i] = map(analogRead(i), minValues[i], maxValues[i], 0, 1000);
      sensorValue[i] = constrain(sensorValue[i], 0, 1000);
      if (isBlackLine==1 && sensorValue[i] > 700) onLine = 1;
      if (isBlackLine==0 && sensorValue[i] < 700) onLine = 1;
    }
  }
  if (numSensors == 5) {
    for (int i = 1; i < 6; i++) {
      sensorValue[i] = map(analogRead(i), minValues[i], maxValues[i], 0, 1000);
      sensorValue[i] = constrain(sensorValue[i], 0, 1000);
      if (isBlackLine == 1 && sensorValue[i] > 700) onLine = 1;
      if (isBlackLine == 0 && sensorValue[i] < 700) onLine = 1;
    }
  }
}
