#include<Arduino.h>
#include<SPI.h>
#include<SD.h>

#define T_PIN 5    //trig pin
#define E_PIN 18     //echo pin
#define SD_CS 19     //chipselect

File logfile;

float distance=0;
float threshold=15.0;
bool object_detected=false;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(T_PIN,OUTPUT);
  pinMode(E_PIN,INPUT);
  SPI.begin(21,22,23,19);
  Serial.println("initializing SD card.....");
  /*if(!SD.begin(SD_CS,SPI,4000000))
  {
    Serial.println("SD Initialization failed....");
    while(1);
  }*/
  Serial.println("SD card initialized successfully ");


  logfile=SD.open("/log_file.txt",FILE_WRITE);
  if(logfile)
  {
    logfile.println("Event log (log_file)");
    logfile.println("--------------------");
    logfile.close();
  }
}

float get_Distance()
{
  digitalWrite(T_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(T_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(T_PIN,LOW);

  long duration=pulseIn(E_PIN,HIGH);
  float distance=(duration * 0.0343)/2.0;
  return distance;
}

void logevent(const char* event)
{
  logfile=SD.open("/log_file.txt",FILE_APPEND);
  if(logfile)
  {
    logfile.print(millis());
    logfile.print(" ms: ");
    logfile.println(event);
    logfile.close();
    Serial.print(" Logged: ");
    Serial.println(event);
  }
  else
  {
    Serial.println("failed to open log file");
  }
}

void loop()
{
  distance=get_Distance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");

  if(distance <threshold && object_detected)
  {
    logevent("Entry Detected");
    object_detected=true;
  }

  if(distance > (threshold +10)&& object_detected)
  {
    logevent("Exit Detected");
    object_detected=false;
  }
  delay(300);
}