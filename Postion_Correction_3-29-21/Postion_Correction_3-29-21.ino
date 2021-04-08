#include <Wire.h>
#include <MechaQMC5883.h>
#include <SoftPWM.h>
#include <SoftPWM_timer.h>
#include <Wire.h>

#define motorLeftB 5
#define motorLeftF 6
#define motorRightB 9
#define motorRightF 10

MechaQMC5883 qmc;


int Current_Heading;
int Target_Heading = 80;
int Current_Quarter;
int Target_Quarter;
int Error_Heading_P = Target_Heading + 5;
int Error_Heading_N = Target_Heading - 5;

void Turn_Right();
void Turn_Left();
void Stop();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  qmc.init();
  SoftPWMBegin();

  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);
  SoftPWMSet(motorRightF, 0);
  SoftPWMSet(motorRightB, 0);

}

void loop() {
  int x, y, z;
  qmc.read(&x, &y, &z); //Get the values of X,Y and Z from sensor
  //Calculate the degree using X and Y parameters with this formulae
  //Convert result into 0 to 360
  Current_Heading = atan2(x, y) / 0.0174532925;

  if (Current_Heading < 0) {
    Current_Heading += 360;
  }
  Current_Heading = 360 - Current_Heading;
  Serial.print("Current Degree: ");
  Serial.print(Current_Heading);


  //current Quarter Identification.
  if (Current_Heading >= 1 && Current_Heading <= 90) {
    Serial.print("  Current Quarter : 1");
    Current_Quarter = 1;
  }
  else if (Current_Heading >= 91 && Current_Heading <= 180) {
    Serial.print("  Current Quarter : 2");
    Current_Quarter = 2;
  }
  else if (Current_Heading >= 181 && Current_Heading <= 270) {
    Serial.print("  Current Quarter : 3");
    Current_Quarter = 3;
  }
  else if (Current_Heading >= 271 && Current_Heading <= 360) {
    Serial.print("  Current Quarter : 4");
    Current_Quarter = 4;
  }

  //Target Quarter Identification.
  if (Target_Heading >= 1 && Target_Heading <= 90) {
    Serial.println("  Target Quarter : 1");
    Target_Quarter = 1;
  }
  else if (Target_Heading >= 91 && Target_Heading <= 180) {
    Serial.println("  Target Quarter : 2");
    Target_Quarter = 2;
  }
  else if (Target_Heading >= 181 && Target_Heading <= 270) {
    Serial.println("  Target Quarter : 3");
    Target_Quarter = 3;
  }
  else if (Target_Heading >= 271 && Target_Heading <= 360) {
    Serial.println("  Target Quarter : 4");
    Target_Quarter = 4;
  }

  //Quarter & Error Heading Matching
  //if Current heading in error Headinng
  if ((Current_Heading >= Error_Heading_N) && (Current_Heading <= Error_Heading_P)) {
    Stop();
    Serial.println("Stop");
  }

  //if current heading is out of error heading
  else {
    //if curent quarter is equal of target quarter.
    if (Current_Quarter == Target_Quarter) {
      if (Current_Heading < Target_Heading) {
        Turn_Right();
      }
      else if (Current_Heading > Target_Heading) {
        Turn_Left();
      }
    }
    //if current quarter isn't in target quarter
    else {
      if (Target_Quarter == 1) {
        if (Current_Quarter == 2) {
          Turn_Left();
        }
        else if (Current_Quarter == 3) {
          Turn_Left();
        }
        else if (Current_Quarter == 4) {
          Turn_Right();
        }
      }
      else if (Target_Quarter == 2) {
        if (Current_Quarter == 1) {
          Turn_Right();
        }
        else if (Current_Quarter == 3) {
          Turn_Left();
        }
        else if (Current_Quarter == 4) {
          Turn_Left();
        }
      }
      else if (Target_Quarter == 3) {
        if (Current_Quarter == 1) {
          Turn_Left();
        }
        else if (Current_Quarter == 2) {
          Turn_Right();
        }
        else if (Current_Quarter == 4) {
          Turn_Right();
        }
      }
      else if (Target_Quarter == 4) {
        if (Current_Quarter == 1) {
          Turn_Left();
        }
        else if (Current_Quarter == 2) {
          Turn_Left();
        }
        else if (Current_Quarter == 3) {
          Turn_Right();
        }
      }
    }
  }



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
