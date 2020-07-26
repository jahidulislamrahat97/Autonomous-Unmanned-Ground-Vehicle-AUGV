#define  Radian_1  57.2958 // degree 
#define Degree_1  0.0174533 // Radian
#define Earth_radius_km  6371
#define pi 3.1416

//23.7505262,90.3592088

// Position A
float lat_A = 23.7505262;
float lon_A = 90.3592088;
// Position B 
//23.7505615,90.3617057

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

}

void loop() {
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
  Serial.println("meter. ");

  float angle_in_radian = atan2(A_B_lat, A_B_lon);
  float angle_in_degree = angle_in_radian * (180 / pi);

  if (angle_in_degree = angle_in_degree < 0 ? angle_in_degree + 360 : angle_in_degree) {
    Serial.print("Angle: ");
    Serial.println(angle_in_degree);
  }
  else {
    angle_in_degree = fmod(angle_in_degree + 360, 360);
    Serial.print("Angle: ");
    Serial.println(angle_in_degree);
  }




}
