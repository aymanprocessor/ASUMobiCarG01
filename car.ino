#include <L298N.h>

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(0, 1); // RX, TX

#define EN1 10
#define IN1 8
#define IN2 9


#define EN2 11
#define IN3 12
#define IN4 13



const int trigPin = 6;
const int echoPin = 7;
const int ledPin = 5;
const int ldrPin = A0;
const int buzzerPin = 3;
bool tone1_flag = true,led_flag = false;
long duration;
int distance;
int ldrValue;
unsigned long previousMillis = 0;        // will store last time
const long interval = 500;           // interval at which to delay
static uint32_t tmp; // increment this
int brightness = 0;
int fadeAmount = 5;
char in;
int speed = 0;
L298N motor1(EN1, IN1, IN2);
L298N motor2(EN2, IN3, IN4);

void forward1(int speed1) {
  if (ultraMeasure() > 5) {
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
  if (ultraMeasure() > 5) {
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

void ultrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print(distance);
  Serial.print(" cm");
  Serial.println();
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


void ldr() {
  ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);
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
  bluetooth.begin(9600); // start the bluetooth uart at 9600 which is its default
  Serial.begin(9600);
  delay(200); // wait for voltage stabilize
  delay(3000); // wait for settings to take affect.
}

void loop() {

  if (bluetooth.available() > 0) { // check if anything in UART buffer
    in = bluetooth.read();
    // bluetooth.print("123");
    //bluetooth.println();
    delay(100);
  }
  Serial.println(in);


  //Serial.println(speed);


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
  else if (in == 'g') {
    forwardTurn(60, 80);
  }
  else if (in == 'E') {
    forwardTurn(60, 100);
  }
  else if (in == 'G') {
    forwardTurn(60, 80);
  }
  else if (in == 'h') {
    forwardTurn(80, 60);
  }
  else if (in == 'H') {
    forwardTurn(100, 60);
  }
  else if (in == 'i') {
    forwardTurn(80, 60);
  }
  else if (in == 'I') {
    forwardTurn(100, 60);
  }
  else if (in == 'A') {
    backwardTurn(60, 100);
  }
  else if (in == 'a') {
    backwardTurn(60, 80);
  }
  else if (in == 'C') {
    backwardTurn(60, 100);
  }
  else if (in == 'c') {
    backwardTurn(60, 80);
  }
  else if (in == 'K') {
    backwardTurn(100, 60);
  }
  else if (in == 'k') {
    backwardTurn(80, 60);
  }
  else if (in == 'J') {
    backwardTurn(100, 60);
  }
  else if (in == 'j') {
    backwardTurn(80, 60);
  }
  //Serial.println(speed);

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
    }else{
      led(LOW);
      tone1_flag = true;
    }
  }

  ultrasonic();
  ldr();
}


void forward(int speed) {

  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor1.forward();
  motor2.forward();
  Serial.print("motor speed = ");
  Serial.println(  motor1.getSpeed());
  Serial.print("speed = ");
  Serial.println(speed);
  Serial.println("-----------------------------------------------");
  if (speed < 0) {
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    speed = 0;
  }
  if (speed > 255) {
    motor1.setSpeed(255);
    motor2.setSpeed(255);
    speed = 255;
  }
  delay(500);
}


