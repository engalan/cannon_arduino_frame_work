/*
  Arduino     Motor
    2    -    IN1  
    3    -    IN3
    4    -    IN2
    5    -    IN4
*/

#include<Arduino.h>
#include <string.h>
#include <Stepper.h>
#include <Servo.h>

#define STEPS             200
#define BAUD              115200
#define TOTAL_DEGREE      2040

#define SERVO_PIN          6
#define TRIGGER            7
#define TRIGGER_MOTOR_1    9
#define TRIGGER_MOTOR_2    10


void configStepper();
void configSerial(uint32_t baud_rate);
void turnLeft(int steps);
void turnRight(int steps);
int32_t calcDegree(int32_t degree);
void setCoordinates(String com);
void configServo();
void shoot();
void configPins();
void enableTriggerMotors();
void disableTriggerMotors();

Stepper stepper(STEPS, 2, 3, 4, 5);
Servo servo_motor;
Servo trigger;

void setup () {
  configSerial(BAUD);
  configStepper();
  configServo();
  configPins();

}
void loop() {
  String command;
  if (Serial.available() > 0) {
    command = Serial.readString();
    setCoordinates(command);
    shoot();
    Serial.flush();
  }
  delay(1000);
}

void configPins(){
  pinMode(TRIGGER_MOTOR_1,OUTPUT);
  pinMode(TRIGGER_MOTOR_2,OUTPUT);
}

void configStepper() {
  stepper.setSpeed(60);
}

void configServo(){
  servo_motor.attach(SERVO_PIN);
  trigger.attach(TRIGGER);
}

void configSerial(uint32_t baud_rate) {
  Serial.begin(baud_rate);
}

void turnLeft(int steps) {
  Serial.println("Turn left");
  stepper.step(steps);
}

void turnRight(int steps) {
  Serial.println("Turn Right");
  stepper.step(steps);
}

int32_t calcDegree(int32_t degree) {
  // // 520 = 360°
  int32_t fc = 4 * (((TOTAL_DEGREE / 4) * (degree / 4)) / (360 / 4));
  return fc;
}

void setCoordinates(String com) {

  int16_t send_degree;
  int16_t set_height;
  String input_degree = com.substring(0, com.indexOf(","));
  String input_height = com.substring(com.indexOf(",") + 1);

  send_degree = calcDegree(input_degree.toInt());
  set_height = input_height.toInt();

  if (send_degree > 0) {
    turnLeft(send_degree);
  }else {
    send_degree = (send_degree );
    turnRight(send_degree);
  }
  if(set_height > 0 && set_height <=90){
    servo_motor.write(set_height);
  }else {
    Serial.println("Invalid height");
  }

}

void shoot(){
  enableTriggerMotors();
  delay(2000);
  trigger.write(40);
  delay(200);
  trigger.write(90);
  delay(500);
  disableTriggerMotors();
}

void enableTriggerMotors(){
  digitalWrite(TRIGGER_MOTOR_1, HIGH);
  digitalWrite(TRIGGER_MOTOR_2, HIGH);
}
void disableTriggerMotors(){
  digitalWrite(TRIGGER_MOTOR_1, LOW);
  digitalWrite(TRIGGER_MOTOR_2, LOW);
}
