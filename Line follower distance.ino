#include <SoftwareSerial.h>
SoftwareSerial bluetooth(0, 1);
void setup() {


  Serial.begin(9600);
  pinMode(7, INPUT);

}
bool flag;
bool firsttime = true;
char in;
void loop() {

  
  if (Serial.available() > 0) {
      in = Serial.read();
      
  }

  if(in == 'c'){
    calib();
  }


}

void calib () {
int resd = digitalRead(7);
  if (resd == 1 && (flag == true || firsttime == true)) {
    Serial.println("1");
    flag = false;
    firsttime = false;
    delay(10);
  }
  else if (resd == 0 && (flag == false || firsttime == true)) {
    Serial.println("0");
    flag = true;
    firsttime = false;
    delay(10);
  }

}

