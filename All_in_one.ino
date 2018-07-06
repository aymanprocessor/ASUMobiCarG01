#include <L298N.h>
#include <Wire.h>

#define EN1 10
#define IN1 9
#define IN2 8

#define EN2 11
#define IN3 13
#define IN4 12

#define addr 0x0D  //Default address of the QMC5883L chip (or DA 5883 as written on the GY-273 breakout board)

const int trigPin = 6;
const int echoPin = 7;
const int ledPin = 5;
const int ldrPin = A0;
const int buzzerPin = A1;
const int RIR = A3;
const int LIR = A2;

bool tone1_flag = true, led_flag = false;
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

const int sr = 2;
const int sc = 3;
const int sl = 4;
int r, l, c;
int flag_left = 0, flag_right = 0, flag_all_white = 0, flag_all_black = 0;

int Xaxis = 0; //Signed 16 bit integer for X-axis value
int Yaxis = 0; //Signed 16 bit integer for Y-axis value
int Zaxis = 0; //Signed 16 bit integer for Z-axis value
uint8_t MSBnum = 0; //Used when reading 8-bit registers
uint8_t LSBnum = 0; //Used when reading 8-bit registers
float headingDegrees;
float heading = 0.777;
float angel = 0, CurrentAngel = 0, DesiredAngel = 0;

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

float Compass() {
  Wire.beginTransmission(addr); //Open a channel to the QMC5883L
  Wire.write(0x00); //select register 0, which is the X-axis MSB register
  Wire.endTransmission();
  Wire.requestFrom(addr, 6); //Read data from each axis, 2 registers per axis, 6 bytes in total
  while (Wire.available())
  {
    //Get X-Axis value
    LSBnum = Wire.read() ; //X-axis LSB byte
    MSBnum = Wire.read() ; //X-axis MSB byte
    Xaxis = (MSBnum << 8) + (LSBnum) ;

    //Get Y-Axis value
    LSBnum = Wire.read() ; //Y-axis LSB byte
    MSBnum = Wire.read() ; //Y-axis MSB byte
    Yaxis = (MSBnum << 8) + (LSBnum) ;

    //Get Z-Axis value
    LSBnum = Wire.read() ; //Z-axis LSB byte
    MSBnum = Wire.read() ; //Z-axis MSB byte
    Zaxis = (MSBnum << 8) + (LSBnum) ;

    heading = atan2(Yaxis, Xaxis);
    if (heading < 0)
      heading += 2 * PI;

    // Check for wrap due to addition of declination.
    if (heading > 2 * PI)
      heading -= 2 * PI;

    // Convert radians to degrees for readability.
    headingDegrees = heading * 180 / M_PI;
    Serial.print("Degree : "); Serial.println(headingDegrees);
  }

  //delay(300);  //not required but skow it down as much as you need.
  return headingDegrees;
}


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(sl, INPUT);
  pinMode(sc, INPUT);
  pinMode(sr, INPUT);
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);

  Wire.begin();

  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x0A); //select Status register 0A
  Wire.write(0x00); //set as required (likely always 00h)
  Wire.endTransmission();

  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x0B); //select Set/Reset period register 0B
  Wire.write(0x01); //Recommended for Set/Reset Period by datasheet
  Wire.endTransmission();

  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x09); //select status register 09
  Wire.write(0x41); //this is the hex value you calculated above based on your required settings
  Wire.endTransmission();

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
Serial.print(digitalRead(LIR));
    Serial.print("      ");
    Serial.print(digitalRead(RIR));
    Serial.print("      ");
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
    Serial.print("l = ");
    Serial.print(digitalRead(sl));
    Serial.print("      ");
    Serial.print("c = ");
    Serial.print(digitalRead(sc));
    Serial.print("      ");
    Serial.print("r = ");
    Serial.print(digitalRead(sr));
    Serial.print("\n");
  }
}

int LineFollowerMode() {
  while (1) {
    r = digitalRead(sr);
    c = digitalRead(sc);
    l = digitalRead(sl);
    if (l == 0 && c == 1 && r == 0 ) {
      if (flag_left != 0 ) {
        Move(80, 0);
      }
      else if (flag_right != 0 ) {
        Move(0, 80);
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
        Move(80, 80);
      }
      else if (flag_all_black != 0) {
        flag_all_black = 0;
        Move(60, 60);
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
    Serial.println(index);
    if (in == '0') {
      return 0;
    }
    if (in == 'a') { //a for posetive angel
      if (Serial.available() > 0) { // check if anything in UART buffer
        angel = Serial.parseInt();
        delay(100);
      }
      while (angel > 360) {
        angel -= 360;
      }
      if (angel != 0) {
        CurrentAngel = Compass();
        DesiredAngel = CurrentAngel + angel - 5;
        while (DesiredAngel > 360) {
          DesiredAngel -= 360;
        }
        Serial.println(DesiredAngel);
        while (CurrentAngel < (DesiredAngel) || CurrentAngel > (DesiredAngel + 5)) {
          while (CurrentAngel < DesiredAngel) {
            Move(70, -80);
            CurrentAngel = Compass();
          }
          Move(-100, 100);
          while (CurrentAngel > (DesiredAngel + 5) ) {
            Move(-70, 80);
            CurrentAngel = Compass();
          }
          Move(100, -100);
          CurrentAngel = Compass();
        }
      }
      stop1();
      angel = 0;
      in = '1';
    }
    else if (in == 'A') { //A for negative angel
      if (Serial.available() > 0) { // check if anything in UART buffer
        angel = Serial.parseInt();
        delay(100);
      }
      while (angel > 360) {
        angel -= 360;
      }
      if (angel != 0) {
        CurrentAngel = Compass();
        DesiredAngel = CurrentAngel - angel + 5;
        while (DesiredAngel > 360) {
          DesiredAngel -= 360;
        }
        Serial.println(DesiredAngel);
        while (CurrentAngel < (DesiredAngel) || CurrentAngel > (DesiredAngel + 5)) {
          while (CurrentAngel < DesiredAngel) {
            Move(70, -80);
            CurrentAngel = Compass();
          }
          Move(-100, 100);
          while (CurrentAngel > (DesiredAngel + 5) ) {
            Move(-70, 80);
            CurrentAngel = Compass();
          }
          Move(100, -100);
          CurrentAngel = Compass();
        }
      }
      stop1();
      angel = 0;
      in = '2';
    }
    else if (in == 'd') { //d for posetive distance
      if (Serial.available() > 0) { // check if anything in UART buffer
        index = Serial.parseInt();
        delay(100);
      }
      in = '3';
    }
    else if (in == 'D') { //D for negative distance
      if (Serial.available() > 0) { // check if anything in UART buffer
        index = Serial.parseInt();
        delay(100);
      }
      in = '4';
    }
    else if (in == ' ') { //for the shapes

    }
  }
}


