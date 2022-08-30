#include<Arduino.h>
#include <string.h>
#include <Stepper.h>

#define STEPS         200
#define BAUD          115200
#define TOTAL_DEGREE  2040

void  configStepper();
void configSerial();
void turnLeft(int steps);
void turnRight(int steps);
int16_t calcDegree(int16_t degree);
void setCoordinates(String com);

Stepper stepper(STEPS, 2, 3, 4, 5);


void setup () {
  configSerial();
  configStepper();

}
void loop() {
  String command;
  if (Serial.available() > 0) {
    command = Serial.readString();
    setCoordinates(command);
    Serial.flush();
  }
  delay(1000);

}

void  configStepper() {
  stepper.setSpeed(100);

}

void configSerial() {
  Serial.begin(BAUD);
}

void turnLeft(int steps) {
  Serial.println("Turn left");
  stepper.step(steps);
}

void turnRight(int steps) {
  Serial.println("Turn Right");
  stepper.step(steps);
}

int16_t calcDegree(int16_t degree) {
  // // 520 = 360°
  int32_t fc = 4 * (((TOTAL_DEGREE / 4) * (degree / 4)) / (360 / 4));
  Serial.print("FC = ");
  Serial.println(fc);
  return fc;
}

void setCoordinates(String com) {

  int16_t send_degree;
  int16_t set_height;
  String input_degree = com.substring(0, com.indexOf(","));
  String input_height = com.substring(com.indexOf(",") + 1);
//
//  // Serial.println("Angulo");
//  // Serial.println(input_degree);
//  // Serial.println("Altura");
//  // Serial.println(input_height);

  send_degree = calcDegree(input_degree.toInt());
  Serial.print("send_degree = ");
  Serial.println(send_degree);

  if (send_degree > 0) {
    //    for ( int i = 0; i < send_degree; i++) {
    turnLeft(send_degree);
    //    }
  }
  else {
    send_degree = (send_degree * -1);
    //    for ( int i = 0; i < send_degree; i++) {
    turnRight(send_degree);
    //    }
  }

}