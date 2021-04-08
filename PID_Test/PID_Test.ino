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





////////////////////////Variables///////////////////////
int Read = 0;
float distance = 0.0;
float elapsedTime, time, timePrev;        //Variables for time control
float distance_previous_error, distance_error;
int period = 50;  //Refresh rate period of the loop is 50ms
///////////////////////////////////////////////////////


///////////////////PID constants///////////////////////
float kp = 4; //Mine was 4
float ki = 0.3; //Mine was 0.1
float kd = 0; //Mine was 1000
float distance_setpoint = 21;           //Should be the distance from sensor to the middle of the bar in mm
float PID_p, PID_i, PID_d, PID_total;
///////////////////////////////////////////////////////



void setup() {
  Serial.begin(9600);
  Wire.begin();
  qmc.init();
  SoftPWMBegin();

  pinMode(motorLeftF, OUTPUT);
  pinMode(motorLeftB, OUTPUT);
  SoftPWMSet(motorRightF, 0);
  SoftPWMSet(motorRightB, 0);

  time = millis();
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

    //Serial.println("Forward");
    analogWrite(motorLeftF, 130);
    analogWrite(motorLeftB, 0);
    analogWrite(motorRightF, 130);
    analogWrite(motorRightB, 0);


  }

  //if current heading is out of error heading
  else {


    //if curent quarter is equal of target quarter.
    if (Current_Quarter == Target_Quarter) {
      if (Current_Heading < Target_Heading) {
        if (millis() > time + period)
        {
          time = millis();
          distance = Current_Heading;
          distance_error = Error_Heading_N - distance;
          PID_p = kp * distance_error;
          float dist_diference = distance_error - distance_previous_error;
          PID_d = kd * ((distance_error - distance_previous_error) / period);

          if (-3 < distance_error && distance_error < 3)
          {
            PID_i = PID_i + (ki * distance_error);
          }
          else
          {
            PID_i = 0;
          }

          PID_total = PID_p + PID_i + PID_d;
          PID_total = map(PID_total, -150, 150, 0, 150);

          if (PID_total < 20) {
            PID_total = 20;
          }
          if (PID_total > 160) {
            PID_total = 160;
          }
          //Serial.println("Trun Right");
          analogWrite(motorLeftF, (PID_total + 30));
          analogWrite(motorLeftB, 0);
          analogWrite(motorRightF, 0);
          analogWrite(motorRightB, (PID_total - 30));

          distance_previous_error = distance_error;
        }


      }
      else if (Current_Heading > Target_Heading) {
        if (millis() > time + period)
        {
          time = millis();
          distance = Current_Heading;
          distance_error = Error_Heading_P - distance;
          PID_p = kp * distance_error;
          float dist_diference = distance_error - distance_previous_error;
          PID_d = kd * ((distance_error - distance_previous_error) / period);

          if (-3 < distance_error && distance_error < 3)
          {
            PID_i = PID_i + (ki * distance_error);
          }
          else
          {
            PID_i = 0;
          }

          PID_total = PID_p + PID_i + PID_d;
          PID_total = map(PID_total, -150, 150, 0, 150);

          if (PID_total < 20) {
            PID_total = 20;
          }
          if (PID_total > 160) {
            PID_total = 160;
          }

          //Serial.println("Trun Left");
          analogWrite(motorLeftF, 0);
          analogWrite(motorLeftB, (PID_total - 30));
          analogWrite(motorRightF, (PID_total + 30));
          analogWrite(motorRightB, 0);

          distance_previous_error = distance_error;
        }
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
  //Serial.println("Trun Right");
  analogWrite(motorLeftF, 180);
  analogWrite(motorLeftB, 0);
  analogWrite(motorRightF, 0);
  analogWrite(motorRightB, 180);
}
void Turn_Left() {
  //Serial.println("Trun Left");
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
