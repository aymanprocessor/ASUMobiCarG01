const byte sensor=0;
float read;
void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read=analogRead(sensor);
  float volt;
  volt=(read/1024)*5000;
  Serial.println(volt);
  
delay(1000);
}
