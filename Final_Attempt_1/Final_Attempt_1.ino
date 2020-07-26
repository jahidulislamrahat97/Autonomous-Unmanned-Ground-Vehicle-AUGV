#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MechaQMC5883.h>

#define Earth_radius_km  6371
#define pi 3.1416

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);
TinyGPSPlus gps;

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

int LL = 8;
int LM = 9;
int LR = 10;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  ss.begin(GPSBaud);
  qmc.init();
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
      if ((bearing - 5) <= heading && heading <= (bearing + 5)  ) {
        Serial.println("Go Forward");
        digitalWrite(LL, LOW);
        digitalWrite(LM, HIGH);
        digitalWrite(LR, LOW);
      }
      else if (bearing < heading) {
        Serial.println("Turn Right");
        digitalWrite(LL, LOW);
        digitalWrite(LM, LOW);
        digitalWrite(LR, HIGH);
      }
      else if (bearing > heading) {
        digitalWrite(LL, HIGH);
        digitalWrite(LM, LOW);
        digitalWrite(LR, LOW);
        Serial.println("Left");
      }


    }
  }
}
