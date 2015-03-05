/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */
 
#include <Wire.h>

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Wire.begin();
  
  Wire.beginTransmission(0x68);
  Wire.write(0x15);
  int ret = Wire.endTransmission(false);
  int bytes = Wire.requestFrom(0x68, 1, 1);
  
  int div = Wire.read();
  Serial.print("DIV: ");
  Serial.println(div);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x16);
  Wire.write(0x1B);
  ret = Wire.endTransmission(false);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x16);
  ret = Wire.endTransmission(false);
  bytes = Wire.requestFrom(0x68, 1, 1);
  
  int dlpf = Wire.read();
  Serial.print("DLPF: ");
  Serial.println(dlpf,HEX);
}

// the loop routine runs over and over again forever:
void loop() {
  delay(100);        // delay in between reads for stability
  
  Wire.beginTransmission(0x68);
  Wire.write(0x1F);
  int ret = Wire.endTransmission(false);
  
  int bytes = Wire.requestFrom(0x68, 2, 1);
  
  if ( Wire.available() )
  {
    short gyro = Wire.read()<<8 | Wire.read();
    Serial.println(gyro);
  }
  else
  {
    Serial.println("Unavailable");
  }
}



