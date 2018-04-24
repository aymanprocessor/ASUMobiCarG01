//#include <L298N.h>

#define EN1 10
#define IN1 8
#define IN2 9

#define EN2 11
#define IN3 12
#define IN4 13

//L298N motor1(EN1, IN1, IN2);
//L298N motor2(EN2, IN3, IN4);

//const int trigPin = 6;
//const int echoPin = 7;
//long duration;
//int distance;
const int sr = 4;
const int sc = 3;
const int sl = 2;
int r, l, c;
int flag_left = 0, flag_right = 0, flag_all_white = 0, flag_all_black = 0;

void Move(int left, int right) {
  // if (ultraMeasure() > 15){
  if (right > 255) {
    right = 255 - 10;
  }
  else if (right < -255) {
    right = -255 + 10;
  }

  if (right > 0) {
    analogWrite(EN2, right);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    //motor2.setSpeed(right + 20);
    //motor2.forward();
  }
  else if (right < 0) {
    analogWrite(EN2, (-right));
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    //motor2.setSpeed((-right) + 15);
    //motor2.backward();
  }
  else {
    analogWrite(EN2, 255);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    //motor2.stop();
  }


  if (left > 255) {
    left = 255;
  }
  else if (left < -255) {
    left = -255;
  }

  if (left > 0) {
    analogWrite(EN1, left);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    //motor1.setSpeed(left);
    //motor1.forward();
  }
  else if (left < 0) {
    analogWrite(EN1, -left);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    //motor1.setSpeed(-left);
    //motor1.backward();
  }
  else {
    analogWrite(EN1, 255);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    //motor1.stop();
  }
  // }
  //  else{
  //    motor1.stop();
  //    motor2.stop();
  //    }
}

//int ultraMeasure() {
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//
//  duration = pulseIn(echoPin, HIGH);
//  distance = duration * 0.034 / 2;
//
//  return distance;
//
//}



void setup() {

  pinMode(sr, INPUT);
  pinMode(sc, INPUT);
  pinMode(sl, INPUT);
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  //Serial.begin(9600);
}

void loop() {

  r = digitalRead(sr);
  c = digitalRead(sc);
  l = digitalRead(sl);
  //  Serial.print("l = ");
  //  Serial.print(l);
  //  Serial.print("      ");
  //  Serial.print("c = ");
  //  Serial.print(c);
  //  Serial.print("      ");
  //  Serial.print("r = ");
  //  Serial.print(r);
  //  Serial.print("\n");
  //Serial.print(distance);

  if (l == 0 && c == 1 && r == 0 ) {
    if (flag_left != 0 ) {
      Move(80, -80);
    }
    else if (flag_right != 0 ) {
      Move(-80, 80);
    }
    else if (flag_all_black == 0) {
      Move(60, 60);
    }
    flag_all_black += 1;
    flag_left = 0;
    flag_right = 0;
    flag_all_white = 0;
    //Move(80, 80);
    //Move(60,60);
  }
  else if (l == 0 && c == 0 && r == 0 ) {
    // Move(100,100);
    if (flag_left != 0 ) {
      Move(0, 60);
    }
    else if (flag_right != 0 ) {
      Move(60, 0);
    }
    else if (flag_all_white == 0) {
      Move(255, 255);
    }
    else if (flag_all_black != 0) {
      flag_all_black = 0;
    }
    else {
      Move(60, 60);
    }
    flag_all_white += 1;
    //flag_left = 0;
    //flag_right = 0;
  }
  else if (l == 1 && c == 0 && r == 0) {
    flag_left += 1;
    flag_all_white = 0;
    //Move(100,100);
    if (flag_left == 1) {
      Move(0, 255);
    }
    else {
      Move(0, 80);
    }
  }
  else if (l == 0 && c == 0 && r == 1) {
    flag_right += 1;
    flag_all_white = 0;
    //Move(100,100);
    if (flag_right == 1) {
      Move(255, 0);
    }
    else {
      Move(80, 0);
    }
  }
  else if (l == 1 && c == 1 && r == 1) {
    Move(50, 50);
  }
}
