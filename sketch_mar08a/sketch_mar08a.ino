#include <LiquidCrystal_I2C.h> //引用I2C库
#include <Wire.h>
#include <DS1302.h>
#include <SPI.h>
#include <SD.h>
#include <Keypad.h>  //加载库
#include "SR04.h"
#define TRIG_PIN 22
#define ECHO_PIN 23

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

int guide=2000;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

LiquidCrystal_I2C lcd(0x3F, 16, 2);
byte rowPins[ROWS] = {2, 3, 14, 5}; //定义行引脚
byte colPins[COLS] = {6, 7, 8, 9}; //定义列引脚
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
String input, Filename;
File myFile;
bool started = false;
DS1302 rtc(17, 18, 19);
Time t;
bool safemode = false;
int red = 10, green = 15, bule = 16, SW = 53;
int years, month, day, hours, minute, second, temp;


void setup ()
{
  
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  pinMode(red, OUTPUT);
  digitalWrite(red, LOW);
  pinMode(green, OUTPUT);
  digitalWrite(green, HIGH);
  pinMode(bule, OUTPUT);
  digitalWrite(bule, LOW);
  pinMode(SW, OUTPUT);
  digitalWrite(SW, LOW);
  rtc.halt(false);
  rtc.writeProtect(false);
  Serial.begin(9600);
  lcd.init(); // 初始化LCD
  lcd.backlight(); //设置LCD背景等亮
  input = ' ';
  SDcardinit();
  //  fileReBuild();
  Serial.println(sr04.Distance());
}
void loop() {
  if   (safemode)
      if(guide>sr04.Distance())
      {
        for (int i = 0; i <= 100; i++) {
              digitalWrite(11, HIGH);
              delay(300);
              digitalWrite(bule,LOW);
              digitalWrite(red, HIGH);
              digitalWrite(11, LOW);
              delay(300);
              digitalWrite(red,LOW);
              digitalWrite(bule,HIGH);

            }
        }
  t = rtc.getTime();
  /*while (Serial.available() > 0)
    timeinit();*/
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    
    if (key == '*')
      input = ' ';
    else if (key == '#') {
      if (input != ' ') {
        Serial.println("Nunber:" + input);
        myFile = SD.open("AREA000.txt", FILE_WRITE);
        if (myFile) {
          if (safemode)
          {
            myFile.print(rtc.getTimeStr());
            myFile.println("非法访问！学号:" + input);
            Serial.println("非法访问！学号:" + input);
            Serial.println(rtc.getTimeStr());
            //myFile.println("testing 1, 2, 3.");
            myFile.close();
            Serial.println("done.");
            input = ' ';
            for (int i = 0; i <= 100; i++) {
              digitalWrite(11, HIGH);
              delay(300);
              digitalWrite(bule,LOW);
              digitalWrite(red, HIGH);
              digitalWrite(11, LOW);
              delay(300);
              digitalWrite(red,LOW);
              digitalWrite(bule,HIGH);

            }
          }
          else {
            myFile.print(rtc.getTimeStr());
            myFile.println("访问，学号:" + input);
            //myFile.println("testing 1, 2, 3.");
            myFile.close();
            Serial.println("访问，学号:" + input);
            Serial.println(rtc.getTimeStr());
            input = ' ';
            digitalWrite(11, HIGH);
            delay(500);
            digitalWrite(11, LOW);
            digitalWrite(SW, HIGH);
            delay(3000);
            digitalWrite(SW, LOW);
          }

        }
        return input;
        input = ' ';
      }
    }
    else
    {
      input = input + key;
      //Serial.println(input);
    }
  }
  if (input == " 5555")
  {
    timeget();
    input = ' ';
  }
  if (input == " 5656")
  {
    digitalWrite(green, LOW);

    safemode = true;
    digitalWrite(red, HIGH);
    input = ' ';
  }
  if (input == " 6565")
  {
    digitalWrite(green, HIGH);

    safemode = false;
    digitalWrite(red, LOW);
    input = ' ';
  }
  lcd.clear();
  lcd.print ("PleaseEnterYour");
  lcd.setCursor(0, 1);
  lcd.print ("Nunber:");
  lcd.setCursor(8, 1);
  //lcd.print(t.sec, DEC);
  //Serial.println(rtc.getTimeStr());
  //Serial.println(rtc.getDateStr());
  lcd.setCursor(7, 1);
  lcd.print(input);
  delay(100);
}
void SDcardinit()
{
  Serial.println("Initializing SD card...");
  lcd.print("SD init");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    lcd.clear();
    lcd.print("SD init fail");
    while (1);
  }
  delay (1000);
  lcd.clear();
  Serial.println("initialization done.");
  lcd.print("SD init done");
  delay(2000);
  lcd.clear();
  myFile = SD.open("AREA000.txt", FILE_WRITE);
  Serial.println("CHEAK FILE");
  lcd.print("CHEAK FILE");
  delay(1000);
  if (myFile) {
    Serial.println("done");
    lcd.clear();
    lcd.print("done");
  } else {
    Serial.println("error");
    lcd.clear();
    lcd.print("error");
  }
  delay (1000);
}

void timeget()
{
  rtc.halt(false);
  rtc.writeProtect(false);
  lcd.clear();
  lcd.print("TIME SITTING");
  delay("1000");
  Serial.println ("hours");
  hours = Sread();
  Serial.println(hours);
  delay("1000");
  temp = Sread();
  Serial.println ("minute");
  minute = Sread();
  Serial.println(minute);
  delay("1000");
  temp = Sread();
  Serial.println ("second");
  second = Sread();
  Serial.println(second);
  rtc.setTime(hours, minute, second);
  Serial.println(rtc.getTimeStr());
  return;
  //Serial.println(rtc.getDateStr());

}
int Sread()
{
  while (1)
  {
    while (Serial.available() > 0)
    {
      //Serial.println("got it");
      int a = Serial.parseInt();
      return a;
      break;
    }
  }
}
/* TESTING
  void fileReBuild ()
  {
    lcd.clear();
    char key = keypad.getKey();
    if (key != NO_KEY)
    {

      if (key == '*')
        input = ' ';
      else if (key == '#') {
        if (input != ' ') {
          Serial.println("AREA:" + input);
          myFile = SD.open("AREA"+input+".txt", FILE_WRITE);
          if (myFile) {

            myFile.println("AREA:" + input);
            //myFile.println("testing 1, 2, 3.");
            myFile.close();
            Serial.println("done.");
          }

          input = ' ';
        }
      }
      else
      {
        input = input + key;
        //Serial.println(input);
      }
    }
    lcd.print ("fileReBuild");
    lcd.setCursor(0, 1);
    lcd.print ("AREA");
    lcd.setCursor(5, 1);
    lcd.print(input);
    delay(100);
  }*/
//Serial.println("你要输出的内容)
