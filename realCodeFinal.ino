#include <SPI.h>  
#include <SD.h>

#include <SFE_BMP180.h> 
#include <Wire.h>
SFE_BMP180 pressure;
#define ALTITUDE 1729.7 // Altitude of Electropeak Co. in meters

File geigerTest;
const byte gData = 53;
const byte geiger1 = A0;
const byte geiger2 = A8;


unsigned long time1;
int counter1 = 0;
int counter2 = 0;
int loopCounter = 1;
int a = 1730;

void setup() {
  Serial.begin(9600);
  pinMode(gData,OUTPUT);
  pinMode(geiger1, INPUT);
  pinMode(geiger2, INPUT);
  if (!SD.begin(53)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  //checking altimeter functionality
  if(!pressure.begin()) {
    Serial.println("bmp180 init success");
    while(1);
  }
  geigerTest = SD.open("geigerPost.csv", FILE_WRITE);
  if (geigerTest) {
  geigerTest.print("Time(minutes)");
  geigerTest.print(",");
  geigerTest.print("Altitude(meters)");
  geigerTest.print(",");
  geigerTest.print("Geiger1");
  geigerTest.print(",");
  geigerTest.println("Geiger2");
  geigerTest.close();
  } else {
    Serial.println("error opening test");
}

}

void loop() {
  time1 = millis();
  
  int sensorVal1 = analogRead(geiger1);
  if (sensorVal1 > 10) {
  counter1 ++;
  Serial.println("first: ");
  Serial.print(sensorVal1);
  Serial.println("");
  }

  int sensorVal2 = analogRead(geiger2);
  if (sensorVal2 > 10) {
  counter2 ++;
  Serial.println("second: ");
  Serial.print(sensorVal2);
  Serial.println("");
  }
  
  if (time1 > (60000 * loopCounter)) {
    a = altFunction();
    geigerTest = SD.open("geigerPost.csv", FILE_WRITE);
    geigerTest.print(loopCounter);
    geigerTest.print(",");
    geigerTest.print(a);
    geigerTest.print(",");
    geigerTest.print(counter1);
    geigerTest.print(",");
    geigerTest.println(counter2);
    geigerTest.close(); 
    counter1 = 0;
    counter2 = 0;
    loopCounter ++;
}

}

int altFunction() {
char status;
double P, p0, a, T;
status = pressure.startTemperature();
if (status != 0)
{
delay(status);
status = pressure.getTemperature(T);
}
if (status != 0)
{
status = pressure.startPressure(3);
}
if (status != 0)
{
delay(status);
status = pressure.getPressure(P, T);
}
if (status != 0)
{
p0 = pressure.sealevel(P, ALTITUDE); // we're at 943.7 meters
a = pressure.altitude(P, p0);
return a;
}
}
