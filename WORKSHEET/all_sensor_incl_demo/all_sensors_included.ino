#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ledpin=12;
const int buttonpin=4;
const int potpin=34;
const int ldrpin=35;
const int joyx=32;
const int joyy=33;
const int joysw=25;

LiquidCrystal_I2C lcd(0x27,16,2);

bool ledstate=false;


void setup()
{
  Serial.begin(115200);

  pinMode(ledpin,OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
  pinMode(joysw, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("ESP 32 Demo");

  delay(1000);
  lcd.clear();
  }
  void loop()
  {
    
    //1.led_blink
    digitalWrite(ledpin,HIGH);
    delay(500);
    digitalWrite(ledpin,LOW);
    delay(500);

    //2.pushbutton

    int btnstate =digitalRead(buttonpin);
    if(btnstate==LOW)
    {
      Serial.println("Button pressed");
    }

    //3.potentiomter
    int potval=analogRead(potpin);
    float pot_volt=potval *(3.3/4095);

    //4.photoresistor 

    int ldrvalue =analogRead(ldrpin);
    float ldr_volt=ldrvalue *(3.3/4095.0);

//5.Joystick

int joyxvalue=analogRead(joyx);
int joyyvalue=analogRead(joyy);
int joysw_s=digitalRead(joysw);
    //5.Serial Monitor 

    Serial.print("Pot:");Serial.print(pot_volt,2);
    Serial.print("V | LDR:");Serial.print(ldr_volt,2);
    Serial.print("V |JoyX:");Serial.print(joyxvalue);
    Serial.print(" |JoyY:");Serial.print(joyyvalue);
    Serial.print(" |JoySw:");Serial.println(joysw_s==LOW?"Pressed":"Released");
  
  

  
  lcd.setCursor(0,0);
  lcd.print("P:");
  lcd.print(pot_volt,1);
  lcd.print("L:");
  lcd.print(ldr_volt,1);
  lcd.print(" ");

  lcd.setCursor(0,1);
  lcd.print("JX:");
  lcd.print(map(joyxvalue,0,4095,0,99));
  lcd.print("JY:");
  lcd.print(map(joyyvalue,0,4095,0,99));
  lcd.print(joysw_s==LOW ? "P":" ");

  delay(200);
  
  }



