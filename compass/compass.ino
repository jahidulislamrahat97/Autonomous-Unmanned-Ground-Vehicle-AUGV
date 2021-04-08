#include <Wire.h> //Wire Librarey for I2C communication
#include <MechaQMC5883.h>
MechaQMC5883 qmc;
int heading;
void setup() {
  Wire.begin(); //Begin I2C communication
  Serial.begin(9600); //Begin Serial Communication
  qmc.init(); //Initialise the QMC5883 Sensor
}

void loop() {
  int x, y, z;
  qmc.read(&x, &y, &z); //Get the values of X,Y and Z from sensor

  heading = atan2(x, y) / 0.0174532925; //Calculate the degree using X and Y parameters with this formulae
  //Convert result into 0 to 360
  if (heading < 0)
    heading += 360;
  heading = 360 - heading;
  Serial.println(heading);
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
