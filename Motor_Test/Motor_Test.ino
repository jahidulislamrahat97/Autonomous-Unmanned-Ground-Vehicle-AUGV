#include <SoftPWM.h>
#include <SoftPWM_timer.h>
#include <Wire.h>

#define motorLeftB 5
#define motorLeftF 6
#define motorRightB 9
#define motorRightF 10

void setup() {
  Serial.begin(9600);
  SoftPWMBegin();

  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);
  SoftPWMSet(motorRightF, 0);
  SoftPWMSet(motorRightB, 0);
}
void Forward() {
  Serial.println("Forward");
  analogWrite(motorLeftF, 180);
  analogWrite(motorLeftB, 0);
  analogWrite(motorRightF, 180);
  analogWrite(motorRightB, 0);
}

void Backward() {
  Serial.println("Backward");
  analogWrite(motorLeftF, 0);
  analogWrite(motorLeftB, 180);
  analogWrite(motorRightF, 0);
  analogWrite(motorRightB, 180);
}

void Turn_Right() {
  Serial.println("Trun Right");
  analogWrite(motorLeftF, 180);
  analogWrite(motorLeftB, 0);
  analogWrite(motorRightF, 0);
  analogWrite(motorRightB, 180);
}
void Turn_Left() {
  Serial.println("Trun Left");
  analogWrite(motorLeftF, 0);
  analogWrite(motorLeftB, 180);
  analogWrite(motorRightF, 180);
  analogWrite(motorRightB, 0);
}

void Stop() {
  Serial.println("Stop");
  analogWrite(motorLeftF, 0);
  analogWrite(motorLeftB, 0);
  analogWrite(motorRightF, 0);
  analogWrite(motorRightB, 0);
}

void loop() {
  while (Serial.available() > 0) {
    int data = Serial.parseInt();
    if (data == 1) {
      Forward();
    }
    else if (data == 2) {
      Backward();
    }
    else if (data == 3) {
      Turn_Left();
    }
    else if (data == 4) {
      Turn_Right();
    }
    else if (data == 5) {
      Stop();
    }
  }
}
