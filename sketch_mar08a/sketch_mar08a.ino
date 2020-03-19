#include <LiquidCrystal_I2C.h> //引用I2C库
#include <Wire.h>
#include <DS1302.h>
#include <SPI.h>
#include <SD.h>
#include <Keypad.h>  //加载库



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



void setup ()
{
  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setTime(22, 47, 30);

  Serial.begin(9600);
  lcd.init(); // 初始化LCD
  lcd.backlight(); //设置LCD背景等亮
  input = ' ';
  SDcardinit();
  //  fileReBuild();
}
void loop() {
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
          myFile.print(rtc.getTimeStr());
          myFile.println("访问，学号:" + input);
          //myFile.println("testing 1, 2, 3.");
          myFile.close();
          Serial.println("done.");
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
  if (input == " A")
    timeget();
  lcd.clear();
  lcd.print ("PleaseEnterYour");
  lcd.setCursor(0, 1);
  lcd.print ("Nunber:");
  lcd.setCursor(8, 1);
  //lcd.print(t.sec, DEC);
  Serial.println(rtc.getTimeStr());
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
  int years, month, day, hours, minute, second,temp;
  lcd.print("TIME SITTING");
  /*Serial.println ("year");
  years = Sread();
  Serial.println(years);
  delay("1000");
  temp=Sread();
  Serial.println ("month");
  month = Sread();
  Serial.println(month);
  delay("1000");
  temp=Sread();
  Serial.println ("day");
  day = Sread();
  Serial.println(day);
  temp=Sread();
  rtc.setDate(19, 3, 2020);*/
  delay("1000");
  Serial.println ("hours");
  hours = Sread();
  Serial.println(hours);
  delay("1000");
  temp=Sread();
  Serial.println ("minute");
  minute = Sread();
  Serial.println(minute);
  delay("1000");
  temp=Sread();
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
      int a=Serial.parseInt();
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
