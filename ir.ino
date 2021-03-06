#include <L298N.h>

#define EN1 10
#define IN1 9
#define IN2 8

#define EN2 11
#define IN3 13
#define IN4 12

int speed = 0;
L298N motor1(EN1, IN1, IN2);
L298N motor2(EN2, IN3, IN4);


const int LIR = A2;
const int RIR = A3;
int LastReading, CurrentReading, counter = 0;
float distance, DesiredDistance, CurrentDistance;
const float delta = .53;
float in;
void setup() {


  Serial.begin(9600);
  pinMode(LIR, INPUT);
  CurrentReading = calibL();
  LastReading = calibL();
}
bool flag;
bool firsttime = true;
void loop() {
  if (Serial.available() > 0) { // check if anything in UART buffer
    in = Serial.parseInt();
    delay(100);
  }
  Serial.println(in);
  if (in != 0) {
    CurrentDistance = Distance();
    DesiredDistance = CurrentDistance + in;
    Serial.println(DesiredDistance);
//    while (CurrentDistance < (DesiredDistance) || CurrentDistance > (DesiredDistance + 5)) {
      while (CurrentDistance < DesiredDistance) {
        Move(70, 70);
        CurrentReading = calibL();
        if (CurrentReading != LastReading) {
          counter += 1;
          LastReading = CurrentReading;
          distance = counter * delta;
          Serial.println(Distance());
        }
        
        CurrentDistance = distance;
        //Serial.println(CurrentDistance);
        
      }
//      Move(-100, -100);
//      while (CurrentDistance > (DesiredDistance + 5) ) {
//        Move(-70, -70);
//        CurrentDistance = Distance();
//      }
//      Move(100, 100);
//      CurrentDistance = Distance();
//    }
  Serial.println(48);
  in = '1';
  stop1();
  }
   
}

int calibL () {
  int resd = digitalRead(Distance);
  if (resd == 1 && (flag == true || firsttime == true)) {
    Serial.println("1");
    flag = false;
    firsttime = false;
    delay(10);
    return 1;
  }
  else if (resd == 0 && (flag == false || firsttime == true)) {
    Serial.println("0");
    flag = true;
    firsttime = false;
    delay(10);
    return 0;
  }

}

float Distance() {
  CurrentReading = calibL();
  if (CurrentReading != LastReading) {
    counter += 1;
    LastReading = CurrentReading;
    distance = counter * delta;
    Serial.println(distance);
    return distance;
  }
  return distance;
}
void Move(int left, int right) {
  if (left > 0) {
    motor1.setSpeed(left);
    motor1.forward();
  }
  else if (left < 0) {
    motor1.setSpeed((-left));
    motor1.backward();
  }
  else {
    motor1.stop();
  }
  if (right > 0) {
    motor2.setSpeed(right);
    motor2.forward();
  }
  else if (right < 0) {
    motor2.setSpeed((-right));
    motor2.backward();
  }
  else {
    motor2.stop();
  }
}

void stop1() {
  motor1.stop();
  motor2.stop();
}


