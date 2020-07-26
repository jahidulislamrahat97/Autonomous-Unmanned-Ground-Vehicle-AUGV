#include <TinyGPS++.h>


#define  Radian_1  57.2958 // degree 
#define Degree_1  0.0174533 // Radian
#define Earth_radius_km  6371
#define pi 3.1416

static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;




float lat_B = 23.7505615;
float lon_B = 90.3617057;
float In_Degree_lat_A ;
float In_Degree_lat_B ;
float A_B_lat;
float A_B_lon;
float In_Degree_lat;
float In_Degree_lon;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXPin, TXPin);
}

void loop() {

  if (Serial2.available() > 0) {
    gps.encode(Serial2.read());
    if (gps.location.isUpdated()) {

      int lat_ = (gps.location.lat());
      //Serial.print(lat_);
      char coord1[10];
      sprintf(coord1, "%f", (gps.location.lat(), gps.location.isValid()));
      //Serial.print("Lat_A2= ");
      String cod = String(coord1[1]) + String(coord1[2]) + String(coord1[3]) + String(coord1[4]) + String(coord1[5]) + String(coord1[6]) + String(coord1[7]);
      String fin = String(lat_) + cod;
      float lat_A = fin.toFloat();
      Serial.print("Lat_A= ");
      Serial.print(lat_A, 6);


      int lon_ = (gps.location.lng());
      //Serial.print("Lon_A1= ");
      //Serial.print(lon_);
      char coord2[10];
      sprintf(coord2, "%f", (gps.location.lng(), gps.location.isValid()));
      String cod2 = String(coord2[1]) + String(coord2[2]) + String(coord2[3]) + String(coord2[4]) + String(coord2[5]) + String(coord2[6]) + String(coord2[7]);
      String fin2 = String(lon_) + cod2;
      float lon_A = fin2.toFloat();
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

      Serial.print("Distance A to B: ");
      Serial.print(Distance_A_to_B);
      Serial.print("km or ");
      Serial.print( 1000 * Distance_A_to_B);
      Serial.print("meter. ");

      float x  = (sin(lat_B - lat_A) * cos(lon_B));
      float y = (cos(lon_A) * sin(lon_B) - sin(lon_A) * cos(lon_B) * cos(lat_B - lat_A));
      float z = atan2(y, x);
      int bearing = ((z * 180 / pi + 360));
      bearing = bearing  % 360;
      Serial.print("bearing2 :");
      Serial.println(bearing);


    }
  }
}
