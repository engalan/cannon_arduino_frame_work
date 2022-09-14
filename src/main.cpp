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

#define STEPS               200
#define BAUD                115200
#define TOTAL_DEGREE        2040   //2040 = 360°

#define SERVO_HEIGHT        8
#define SERVO_TRIGGER       7
#define DC_MOTOR_1          6  //DC motor
#define DC_MOTOR_2          11 //DC motor

#define FULL_SPEED          128 // 128 = 6V | 255 = 12V
#define STOP_DC_MOTORS      0


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
  pinMode(DC_MOTOR_1,OUTPUT);
  pinMode(DC_MOTOR_2,OUTPUT);
}

void configStepper() {
  stepper.setSpeed(60);
}

void configServo(){
  servo_motor.attach(SERVO_HEIGHT);
  trigger.attach(SERVO_TRIGGER);
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
  analogWrite(DC_MOTOR_1, FULL_SPEED);
  analogWrite(DC_MOTOR_2, FULL_SPEED);
}
void disableTriggerMotors(){
  // digitalWrite(DC_MOTOR_1, LOW);
  analogWrite(DC_MOTOR_1, STOP_DC_MOTORS);
  digitalWrite(DC_MOTOR_2, STOP_DC_MOTORS);
}
