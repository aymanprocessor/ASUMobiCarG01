#include <L298N.h>

#define EN1 10
#define IN1 9
#define IN2 8

#define EN2 11
#define IN3 13
#define IN4 12

const int trigPin = 6;
const int echoPin = 7;
const int ledPin = 5;
const int ldrPin = A0;
const int buzzerPin = A1;

bool tone1_flag = true,led_flag = false;
long duration;
int distance;
int ldrValue;

unsigned long previousMillis = 0;        // will store last time
const long interval = 500;           // interval at which to delay
static uint32_t tmp; // increment this
int brightness = 0;
int fadeAmount = 5;
char in; long index = 0;
int speed = 0;
L298N motor1(EN1, IN1, IN2);
L298N motor2(EN2, IN3, IN4);

const int sr = 4;
const int sc = 3;
const int sl = 2;
int r, l, c;
int flag_left = 0, flag_right = 0, flag_all_white = 0, flag_all_black = 0;

void Move(int left, int right) {
  if (left > 0) {
    motor1.setSpeed(left);
    motor1.forward();
  }
  else if (left < 0) {
    motor1.setSpeed(left);
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
    motor2.setSpeed(right);
    motor2.backward();
  }
  else {
    motor2.stop();
  }
}

void forward1(int speed1) {
  if (ultraMeasure() > 15) {
    motor1.setSpeed(speed1);
    motor2.setSpeed(speed1);
    motor1.forward();
    motor2.forward();
  } else {
    stop1();
  }

}
void backward1(int speed1) {

  motor1.setSpeed(speed1);
  motor2.setSpeed(speed1);
  motor1.backward();
  motor2.backward();



}
void stop1() {


  motor1.stop();
  motor2.stop();


}

void turnRightFromCenter(int speed1) {

  motor1.setSpeed(speed1);
  motor2.setSpeed(speed1);
  motor1.forward();
  motor2.backward();

}
void turnLeftFromCenter(int speed1) {

  motor1.setSpeed(speed1);
  motor2.setSpeed(speed1);
  motor1.backward();
  motor2.forward();

}

void forwardTurn(int right, int left) {
  if (ultraMeasure() > 15) {
    motor1.setSpeed(right);
    motor2.setSpeed(left);
    motor1.forward();
    motor2.forward();
  } else {
    stop1();
  }
}
void backwardTurn(int right, int left) {

  motor1.setSpeed(right);
  motor2.setSpeed(left);
  motor1.backward();
  motor2.backward();

}

int ultraMeasure() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;

}
int ldrMeasure() {
  ldrValue = analogRead(ldrPin);
  return ldrValue;
}
void led(bool flag) {
  digitalWrite(ledPin, flag);
}
void tone1() {
  if (tone1_flag) {
    tone(buzzerPin, 1000, 100);


    //noTone(buzzerPin);

  }
}


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(sl, INPUT);
  pinMode(sc, INPUT);
  pinMode(sr, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(3000); // wait for settings to take affect and voltage stabilize.
}

void loop() {//Selection mode
  if (Serial.available() > 0) { // check if anything in UART buffer
    in = Serial.read();
    delay(100);
  }
  Serial.println(in);
  switch (in) {
    case '1' :
      ControlMode();
      break;
    case '2' :
      LineFollowerMode();
      break;
    case '3':
      AccuracyMode();
      break;
  }
  stop1();

}


int ControlMode() {
  while (1) {
    if (Serial.available() > 0) { // check if anything in UART buffer
      in = Serial.read();
      delay(100);
    }
    Serial.println(in);

    if (in == '0') {
      return 0;
    }
    if      (in == 'S') {
      stop1();
    }
    else if (in == 'l') {
      turnLeftFromCenter(70);
    }
    else if (in == 'L') {
      turnLeftFromCenter(90);
    }
    else if (in == 'r') {
      turnRightFromCenter(70);
    }
    else if (in == 'R') {
      turnRightFromCenter(90);
    }
    else if (in == 'f') {
      forward1(80);
    }
    else if (in == 'F') {
      forward1(120);
    }
    else if (in == 'b') {
      backward1(80);
    }
    else if (in == 'B') {
      backward1(120);
    }
    else if (in == 'e') {
      forwardTurn(60, 80);
    }
    else if (in == 'E') {
      forwardTurn(60, 100);
    }
    else if (in == 'h') {
      forwardTurn(80, 60);
    }
    else if (in == 'H') {
      forwardTurn(100, 60);
    }
    else if (in == 'A') {
      backwardTurn(60, 100);
    }
    else if (in == 'a') {
      backwardTurn(60, 80);
    }
    else if (in == 'K') {
      backwardTurn(100, 60);
    }
    else if (in == 'k') {
      backwardTurn(80, 60);
    }

    if (in == 'z') {
      led(LOW);
      led_flag = false;
    } else if (in == 'Z') {
      led(HIGH);
      led_flag = true;
    }
    if (ldrMeasure() > 1010) {
      led(HIGH);
      tone1();
      tone1_flag = false;
    } else {
      if (led_flag) {
        led(HIGH);
        tone1_flag = false;
      } else {
        led(LOW);
        tone1_flag = true;
      }
    }
    Serial.print(ultraMeasure());
    Serial.println(" cm");
    Serial.println(ldrMeasure());
  }
}

int LineFollowerMode() {
  while (1) {
    r = digitalRead(sr);
    c = digitalRead(sc);
    l = digitalRead(sl);
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
    if (Serial.available() > 0) { // check if anything in UART buffer
      in = Serial.read();
      if (in == '0') {
        return 0;
      }
    }
  }
}

int AccuracyMode() {
  while (1) {
    if (Serial.available() > 0) { // check if anything in UART buffer
      in = Serial.read();
      delay(100);
    }
    Serial.println(in);
    if (in == '0') {
      return 0;
    }
    if (in == 'a') { //a for posetive angel
      index = Serial.parseInt();

    }
    else if (in == 'A') { //A for negative angel
      index = Serial.parseInt();

    }
    else if (in == 'd') { //d for posetive distance
      index = Serial.parseInt();

    }
    else if (in == 'D') { //D for negative distance
      index = Serial.parseInt();

    }
    else if (in == ' ') { //for the shapes

    }
  }
}


