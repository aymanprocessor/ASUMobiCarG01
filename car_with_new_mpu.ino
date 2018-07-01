    #include <L298N.h>
    #include <Wire.h>
    #include <Servo.h>
    //#include <SoftwareSerial.h>
    //SoftwareSerial bluetooth(0, 1); // RX, TX
    
    #define EN1 10
    #define IN1 8
    #define IN2 9
    
    
    #define EN2 11
    #define IN3 12
    #define IN4 13
    
    
    
     Servo right_prop;
     Servo left_prop;
    
    int16_t Acc_rawX, Acc_rawY, Acc_rawZ,Gyr_rawX, Gyr_rawY, Gyr_rawZ;
     
    
    float Acceleration_angle[2];
    float Gyro_angle[2];
    float Total_angle[2];
    
    
    
    
    float elapsedTime, time, timePrev;
    int i;
    float rad_to_deg = 180/3.141592654;
    
      float error,PID, pwmLeft, pwmRight,  previous_error;
    float pid_p=0;
    float pid_i=0;
    float pid_d=0;
    ///////////////////PID CONSTANTS/////////////////
    double kp=3.55;//3.55
    double ki=0.005;//0.003
    double kd=2.05;//2.05
    ///////////////////////////////////////////////
    
    double throttle=1300; //initial value of throttle to the motors
    float desired_angle = 0; //This is the angle in which we whant the
                             //balance to stay steady
    
    
    
    
     
    
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
    
    void ultrasonic() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
    
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
    
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
    
    //  Serial.print(distance);
    //  Serial.print(" cm");
    //  Serial.println();
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
      //Serial.println(ldrValue);
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
    
    Wire.begin(); //begin the wire comunication
      Wire.beginTransmission(0x68);
      Wire.write(0x6B);
      Wire.write(0);
      Wire.endTransmission(true);
      Serial.begin(115200);
      //right_prop.attach(3); //attatch the right motor to pin 3
      //left_prop.attach(5);  //attatch the left motor to pin 5
    
      time = millis(); //Start counting time in milliseconds
      //Serial.print("time 1 =");Serial.print(time);
      
      left_prop.writeMicroseconds(1000); 
      right_prop.writeMicroseconds(1000);
      delay(7000); 
    
      
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
      pinMode(ledPin, OUTPUT);
      pinMode(buzzerPin, OUTPUT);
    //  bluetooth.begin(9600); // start the bluetooth uart at 9600 which is its default
      
      delay(200); // wait for voltage stabilize
      delay(3000); // wait for settings to take affect.
    }
    
    void loop() {
    
    timePrev = time;  // the previous time is stored before the actual time read
        //Serial.print("\t timePrev= ");Serial.print(timePrev);
        time = millis();  // actual time read
       // Serial.print("\t time 2= ");Serial.print(time);
        elapsedTime = (time - timePrev) / 1000; 
      //Serial.print("\t elap= ");Serial.println(elapsedTime);
      
         Wire.beginTransmission(0x68);
         Wire.write(0x3B); //Ask for the 0x3B register- correspond to AcX
         Wire.endTransmission(false);
         Wire.requestFrom(0x68,6,true); 
       
       
         Acc_rawX=Wire.read()<<8|Wire.read(); //each value needs two registres
         Acc_rawY=Wire.read()<<8|Wire.read();
         Acc_rawZ=Wire.read()<<8|Wire.read();
    
     
        
         /*---X---*/
         Acceleration_angle[0] = atan((Acc_rawY/16384.0)/sqrt(pow((Acc_rawX/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
         /*---Y---*/
         Acceleration_angle[1] = atan(-1*(Acc_rawX/16384.0)/sqrt(pow((Acc_rawY/16384.0),2) + pow((Acc_rawZ/16384.0),2)))*rad_to_deg;
     
       Wire.beginTransmission(0x68);
       Wire.write(0x43); //Gyro data first adress
       Wire.endTransmission(false);
       Wire.requestFrom(0x68,4,true); //Just 4 registers
       
       Gyr_rawX=Wire.read()<<8|Wire.read(); //Once again we shif and sum
       Gyr_rawY=Wire.read()<<8|Wire.read();
     
       /*---X---*/
       Gyro_angle[0] = Gyr_rawX/131.0; 
       /*---Y---*/
       Gyro_angle[1] = Gyr_rawY/131.0;
    
       
       /*---X axis angle---*/
       Total_angle[0] = 0.98 *(Total_angle[0] + Gyro_angle[0]*elapsedTime) + 0.02*Acceleration_angle[0];
       //Total_angle[0]-=1.6;
       /*---Y axis angle---*/
       Total_angle[1] = 0.98 *(Total_angle[1] + Gyro_angle[1]*elapsedTime) + 0.02*Acceleration_angle[1];
       //Total_angle[1]+=5.2;
         Serial.print("1=  ");
        Serial.print(Total_angle[0]);
        
        Serial.print("\t\t");
         Serial.print("2=  ");
        Serial.println(Total_angle[1]);
    
       error = Total_angle[1] - desired_angle;
      
      pid_p = kp*error;
    
    if(-3 <error <3)
    {
      pid_i = pid_i+(ki*error);  
    }
    
     pid_d = kd*((error - previous_error)/elapsedTime);
    
    /*The final PID values is the sum of each of this 3 parts*/
      PID = pid_p + pid_i + pid_d;
    
    if(PID < -1000)
    {
      PID=-1000;
    }
    if(PID > 1000)
    {
      PID=1000;
    }
    
    pwmLeft = throttle + PID;
    pwmRight = throttle - PID;
    
    
    //Right
    if(pwmRight < 1000)
    {
      pwmRight= 1000;
    }
    if(pwmRight > 2000)
    {
      pwmRight=2000;
    }
    //Left
    if(pwmLeft < 1000)
    {
      pwmLeft= 1000;
    }
    if(pwmLeft > 2000)
    {
      pwmLeft=2000;
    }
    
    //left_prop.writeMicroseconds(pwmLeft);
    //right_prop.writeMicroseconds(pwmRight);
    ///previous_error = error; //Remember to store the previous error.
    
    if(Total_angle[0]>15 || Total_angle[0]<-16 || Total_angle[1]>15 || Total_angle[1]<-16){
      
      
    digitalWrite (ledPin, HIGH);
   // digitalWrite (buzzerPin, HIGH);
       //stop1();
        motor1.stop();
      motor2.stop();
      
      }


    
    
    
    
      if (Serial.available() > 0) { // check if anything in UART buffer
        in = Serial.read();
        // bluetooth.print("123");
        //bluetooth.println();
        delay(100);
      }
      Serial.println(in);
    
    
      //Serial.println(speed);
    
    
      if      (in == 's') {
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
