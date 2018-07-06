#include <Wire.h>

int Xaxis = 0; //Signed 16 bit integer for X-axis value
int Yaxis = 0; //Signed 16 bit integer for Y-axis value
int Zaxis = 0; //Signed 16 bit integer for Z-axis value
uint8_t MSBnum = 0; //Used when reading 8-bit registers
uint8_t LSBnum = 0; //Used when reading 8-bit registers

float heading = 0.0;

#define addr 0x0D  //Default address of the QMC5883L chip (or DA 5883 as written on the GY-273 breakout board)

void setup()
{
  Serial.begin(9600);

  //Setup GY273  - How to detect if it's not present?
  Wire.begin();

  //Register 0x0Ah is Soft Reset, Roll Pointer and Interupt Enable.  None of this is required here so set to 0.
  //Bits are:
  //      7               6               5        4        3        2       1            0
  // Soft Reset    pointer roll-over     n/a      n/a      n/a      n/a     n/a    Interupt Enable

  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x0A); //select Status register 0A
  Wire.write(0x00); //set as required (likely always 00h)
  Wire.endTransmission();


  //Register 0x0Bh is Set/Reset Period.  Datasheet recommends setting this to 01h without explanation
  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x0B); //select Set/Reset period register 0B
  Wire.write(0x01); //Recommended for Set/Reset Period by datasheet
  Wire.endTransmission();


  //Register 0x09h is the most important. The byte sets Over Sample Rate (OSR), Range(RNG), Output Data Rate (ODR) and the Mode
  //Eah setting is 2 bits and the values are as follows:
  //
  //        OSR     RNG     ODR       Mode
  //00      512     2G      10Hz      Standby
  //01      256     8G      50Hz      Continuous
  //10      128     n/a     100Hz     n/a
  //11      64      n/a     200Hz     n/a
  //
  //So, for 128 over sample, 8G range, 50Hz output rate and continuous mode
  //the binary number would be 10 01 01 01 = 95 in hex (and 149 in decimal)
  //
  //or for 256 over samplr rate, 2G range, 10Hz data rate and continuous mode
  //the binary number would be 01 00 00 01 = 41 in hex (and 65 in decimal)
  //Set register 09h to whatever hex value you calculate for your desired settings

  Wire.beginTransmission(addr); //open communication with GY273
  Wire.write(0x09); //select status register 09
  Wire.write(0x41); //this is the hex value you calculated above based on your required settings
  Wire.endTransmission();
}

//thats it!  Now get the X-axis, Y-axis and Z-axis values and output to serial port
void loop()
{
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
    Serial.print("x val: "); Serial.print(Xaxis);

    //Get Y-Axis value
    LSBnum = Wire.read() ; //Y-axis LSB byte
    MSBnum = Wire.read() ; //Y-axis MSB byte
    Yaxis = (MSBnum << 8) + (LSBnum) ;
    Serial.print("y val: "); Serial.print(Yaxis);

    //Get Z-Axis value
    LSBnum = Wire.read() ; //Z-axis LSB byte
    MSBnum = Wire.read() ; //Z-axis MSB byte
    Zaxis = (MSBnum << 8) + (LSBnum) ;
    Serial.print("z val: "); Serial.println(Zaxis);

    heading = atan2(Yaxis, Xaxis);
    if (heading < 0)
      heading += 2 * PI;

    // Check for wrap due to addition of declination.
    if (heading > 2 * PI)
      heading -= 2 * PI;

    // Convert radians to degrees for readability.
    float headingDegrees = heading * 180 / M_PI;
    Serial.print("Degree : "); Serial.println(headingDegrees);
  }
  //X, Y and Z are values are between -32768 and 32767
 /*Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");  */
  delay(300);  //not required but skow it down as much as you need.
}
