#include <Wire.h>
#include<TEA5767N.h>
#include <TEA5767Radio.h>
#include <LiquidCrystal.h>
TEA5767Radio radio = TEA5767Radio();
TEA5767N rad = TEA5767N();
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//const int analogPin = A0;  
int button1= 6;
int button2 = 7; 
int button3 =8;
int button4 =13;
int volume = A0;
int soundsensor = 9;
int led = 10;
int val1,val2,val3,val4;
int y = 0;
static float total=0;
const int threshold = 0; 
static float x;

byte ant[8] = {B00000,B11111,B01010,B00100,B00100,B00100,B01110,B00000};
byte range1[8] = {B00000,B00000,B00000,B00000,B00001,B00101,B10101,B00000};
byte range2[8] = {B00000,B00000,B00100,B10100,B10100,B10100,B10100,B00000};

// Custom characters
#define SCALE_CLEAR   5    // Radio dial scale
byte scaleChar[8] = {
    0B00000,
    0B00000,
    0B00000,
    0B00000,
    0B00000,
    0B00100,
    0B10101,
    0B10101
  };

  byte scaleChar1[8] = {
    0B00000,
    0B00100,
    0B00100,
    0B00100,
    0B00100,
    0B00100,
    0B10101,
    0B10101
  };



const int LED = 0;

int analogsignal=0;
int analogscale=0;
int stereoled=0; 



byte status = 0;

void updateScale() {
  int lcdBase = (x  - 88) * 5;  
  if(lcdBase > 100) lcdBase = 100;
  //lcd.clear();
  
  int lcdMajor = lcdBase / 5;    
  int lcdMinor = lcdBase % 5; 
  
  if(lcdMajor > 0) {
    lcd.setCursor(lcdMajor  - 1, 1);
    lcd.write(SCALE_CLEAR);
  } else
    lcd.setCursor(lcdMajor, 1);
    lcd.write(6);
  
  if(lcdMajor < 20)
    lcd.write(SCALE_CLEAR);

}
void setup()
{ 
  Serial.begin(9600);
  rad.setMonoReception();
  rad.setStereoNoiseCancellingOn();
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);

  pinMode(volume, OUTPUT);
  pinMode (soundsensor, INPUT);
  pinMode(led,OUTPUT);
  
  
  x=95;
  lcd.begin(20, 4);
  lcd.clear();
  Wire.begin();
  radio.setFrequency(x);
  pinMode(5,OUTPUT);
  
  int i;
  byte needleChar[8];  

  lcd.createChar(SCALE_CLEAR, scaleChar);
  lcd.createChar(6, scaleChar1);

  // Create custom character to represent all (5) possible needles position
  for(int j = 0; j < 5; j++) {
    for(i = 0; i < 8; i++)
      needleChar[i] = scaleChar1[i] | (0b10000 >> j);
    lcd.createChar(j, needleChar);
  }

  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 1);
  //lcd.clear();
  
  // Draw the dial scales background
  for(i = 0; i < 20; i++)
    lcd.write(SCALE_CLEAR);
   
  // Initialize the radio module
  Wire.begin();
  
} 

void loop()
{

  int sensorvalue = digitalRead (soundsensor);                                                                   
  if (sensorvalue == 1)                                    
  {
    digitalWrite(led, HIGH);
  }
  
  else
  {
    digitalWrite(led, LOW);
  }
   
  
  val1 = digitalRead(button1);
  val2 = digitalRead(button2);
  val3 = digitalRead(button3);
  val4 = digitalRead(button4);
  
  if(val1==HIGH)
  {
    total++;
    x = x + 0.1;
    radio.setFrequency(x);
  }
  
  if(val2==HIGH)
  {
    total--;
    x = x-0.1;
    radio.setFrequency(x);
  }

  if(val3==HIGH)
  {
    total++;
    x = x+1;
    radio.setFrequency(x); 
  }
  

  if(val4==HIGH)
  {
    total--;
    x = x-1;
    radio.setFrequency(x);
  }
    
  delay(1);

  if(x>108){
    x=87.5;
   }
   if(x<87.5){
    x=108;
   }
   
  // Update the Auto / Manual indicator
 
  lcd.createChar(10, ant);
  lcd.createChar(11, range1);
  lcd.createChar(12, range2);
  lcd.setCursor(0, 0);
  
  lcd.write(10);
  lcd.setCursor(1, 0);
  lcd.write(11);
  lcd.setCursor(2, 0);
  lcd.write(12);        
  lcd.setCursor(0, 2);
  //lcd.print("Date: 09/12/2021");
  
  //lcd.setCursor(0, 2);
  lcd.print("   Freq: " + (String)x + " MHz");
  lcd.setCursor(0, 3);
  lcd.print("     09/12/2021");
  //lcd.print("       FM Mode");
  //lcd.print("Volume: ");
   //lcd.setCursor(0, 1);

  // Update the radio dial
  updateScale();
  delay(100);

}
