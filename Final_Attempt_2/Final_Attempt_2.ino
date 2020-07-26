#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MechaQMC5883.h>

#define Earth_radius_km  6371
#define pi 3.1416

int REN1 = 7; // Right Forward EN
int REN2 = 8; // Right Reverse EN
int LEN1 = 12; // Left Forward EN
int LEN2 = 13; // Left Reverse EN

int RPWM1 = 5; // Right Forward PWM
int RPWM2 = 6; // Right Reverse PWM
int LPWM1 = 9; // Left Forward PWM
int LPWM2 = 10; // Left Reverse PWM

int LL = 2;
int LM = 11;
int LR = A0;

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;

int D_speed = 100;
int T_speed = 70;

MechaQMC5883 qmc;

float lat_B = 23.7505615;
float lon_B = 90.3617057;

float In_Degree_lat_A ;
float In_Degree_lat_B ;
float A_B_lat;
float A_B_lon;
float In_Degree_lat;
float In_Degree_lon;
int heading;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  ss.begin(GPSBaud);
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

  pinMode(LL, OUTPUT);
  pinMode(LM, OUTPUT);
  pinMode(LR, OUTPUT);
}


void Compass() {
  int x, y, z;
  qmc.read(&x, &y, &z);

  heading = atan2(x, y) / 0.0174532925;

  if (heading < 0)
    heading += 360;
  heading = 360 - heading;
  if (heading > 338 || heading < 22) {
    Serial.println("NORTH");
  }

  if (heading > 22 && heading < 68) {
    Serial.println("NORTH-EAST");
  }

  if (heading > 68 && heading < 113) {
    Serial.println("EAST");
  }

  if (heading > 113 && heading < 158) {
    Serial.println("SOUTH-EAST");
  }

  if (heading > 158 && heading < 203) {
    Serial.println("SOUTH");
  }

  if (heading > 203 && heading < 248) {
    Serial.println("SOTUH-WEST");
  }

  if (heading > 248 && heading < 293) {
    Serial.println("WEST");
  }

  if (heading > 293 && heading < 338) {
    Serial.println("NORTH-WEST");
  }

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

void Turn_Left() {
  Serial.println("Left ");


  digitalWrite(REN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(LEN2, HIGH);

  analogWrite(RPWM1, 0);
  analogWrite(RPWM2, T_speed);
  analogWrite(LPWM1, T_speed);
  analogWrite(LPWM2, 0);
}

void Turn_Right() {
  Serial.println("Right ");
  digitalWrite(REN1, HIGH);
  digitalWrite(REN2, HIGH);
  digitalWrite(LEN1, HIGH);
  digitalWrite(LEN2, HIGH);

  analogWrite(RPWM1, T_speed);
  analogWrite(RPWM2, 0);
  analogWrite(LPWM1, 0);
  analogWrite(LPWM2, T_speed);
}

void loop() {

  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {

      double lat_A = (gps.location.lat());
      Serial.print("Lat_A= ");
      Serial.print(lat_A, 6);

      double lon_A = (gps.location.lng());
      Serial.print(" Lon_A= ");
      Serial.print(lon_A, 6);

      A_B_lat = lat_A - lat_B;
      A_B_lon = lon_A - lon_B;

      In_Degree_lat = A_B_lat * (pi / 180);
      In_Degree_lon = A_B_lon * (pi / 180);

      In_Degree_lat_A = lat_A * (pi / 180);
      In_Degree_lat_B = lat_B * (pi / 180);

      float a =  sin(In_Degree_lat / 2) * sin(In_Degree_lat / 2) + sin(In_Degree_lon / 2) * sin(In_Degree_lon / 2) * cos(In_Degree_lat_A) * cos(In_Degree_lat_B);
      float b = 2 * atan2(sqrt(a) , sqrt(1 - a));
      float Distance_A_to_B = Earth_radius_km * b;

      Serial.print("Dist A to B: ");
      Serial.print(Distance_A_to_B);
      Serial.print("km ");
      Serial.print( 1000 * Distance_A_to_B);
      Serial.print("meter ");

      float x  = (sin(lat_B - lat_A) * cos(lon_B));
      float y = (cos(lon_A) * sin(lon_B) - sin(lon_A) * cos(lon_B) * cos(lat_B - lat_A));
      float z = atan2(y, x);
      int bearing = ((z * 180 / pi + 360));
      bearing = bearing  % 360;
      Serial.print("bearing :");
      Serial.println(bearing);
      Compass();
      if ((bearing - 10) <= heading && heading <= (bearing + 10)  ) {
        Forward();
        digitalWrite(LL, LOW);
        digitalWrite(LM, HIGH);
        analogWrite(LR, 0);
      }
      else if (bearing < heading) {
        Turn_Left();

        digitalWrite(LL, LOW);
        digitalWrite(LM, LOW);
        analogWrite(LR, 255);
      }
      else if (bearing > heading) {
        Turn_Right();
        digitalWrite(LL, HIGH);
        digitalWrite(LM, LOW);
        analogWrite(LR, 0);

      }




    }
  }
}
