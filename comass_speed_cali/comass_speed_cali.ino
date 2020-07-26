
#include <Wire.h>
#include <MechaQMC5883.h>
int REN1 = 7; // Right Forward EN
int REN2 = 8; // Right Reverse EN
int LEN1 = 12; // Left Forward EN
int LEN2 = 13; // Left Reverse EN

int RPWM1 = 5; // Right Forward PWM
int RPWM2 = 6; // Right Reverse PWM
int LPWM1 = 9; // Left Forward PWM
int LPWM2 = 10; // Left Reverse PWM


MechaQMC5883 qmc;
int heading;

int Speed = 80;
int CD = 130;
int error_head;
int error_volt;
int RMS;
int LMS;
int D_speed = 100;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  qmc.init();
  pinMode(REN1, OUTPUT);
  pinMode(REN2, OUTPUT);
  pinMode(LEN1, OUTPUT);
  pinMode(LEN2, OUTPUT);
  pinMode(RPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(LPWM2, OUTPUT);

  digitalWrite(REN1, LOW);
  digitalWrite(REN2, LOW);
  digitalWrite(LEN1, LOW);
  digitalWrite(LEN2, LOW);
  analogWrite(RPWM1, 0);
  analogWrite(RPWM2, 0);
  analogWrite(LPWM1, 0);
  analogWrite(LPWM2, 0);

}

void Forward() {

  Serial.println("Forward");
  digitalWrite(REN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(LEN2, HIGH);

  analogWrite(RPWM1, 0);
  analogWrite(RPWM2, D_speed);
  analogWrite(LPWM1, 0);
  analogWrite(LPWM2, D_speed);
}

void Error() {


  digitalWrite(REN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(LEN2, HIGH);

  analogWrite(RPWM1, RMS);
  analogWrite(RPWM2, 0);
  analogWrite(LPWM1, LMS);
  analogWrite(LPWM2, 0);
}


void loop() {
  int x, y, z;
  qmc.read(&x, &y, &z);

  heading = atan2(x, y) / 0.0174532925;

  if (heading < 0)
    heading += 360;
  heading = 360 - heading;

  Serial.print("Heading: ");
  Serial.print(heading);

  int bx = CD - 10;
  int by = CD + 10;

  if ((CD - 10) <= heading && heading <= (CD + 10)) {
    //Forward();
  }
  else if (heading > by) {
    error_head = heading - by;
    error_volt = error_head * 1;
    RMS = Speed + error_volt ;
    LMS = Speed - error_volt ;
    Serial.print("  LMS: ");
    Serial.print(LMS);
    Serial.print("  RMS: ");
    Serial.println(RMS);
    //Error();

  }
  else if (heading  < bx) {
    error_head = bx - heading ;
    error_volt = error_head * 1;
    RMS = Speed - error_volt ;
    LMS = Speed + error_volt ;
    Serial.print("LMS: ");
    Serial.print(LMS);
    Serial.print("  RMS: ");
    Serial.println(RMS);
    //Error();
  }

}
