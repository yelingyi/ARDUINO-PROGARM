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
DS1302 rtc(2, 3, 4);
void setup ()
{
    rtc.setTime(22, 47, 30);

  Serial.begin(9600);
  lcd.init(); // 初始化LCD
  lcd.backlight(); //设置LCD背景等亮
  input = ' ';
  SDcardinit();
//  fileReBuild();
}
void loop() {
    while (Serial.available() > 0)
      timeinit();
    lcd.clear();
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

            myFile.println("Nunber:" + input);
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
    lcd.print ("PleaseEnterYour");
    lcd.setCursor(0, 1);
    lcd.print ("Nunber:");
    lcd.setCursor(8, 1);
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(7, 1);
    lcd.print(input);
    delay(100);
}
void SDcardinit()
{
  Serial.print("Initializing SD card...");
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
void timeinit ()
{
  int year,month,day,hour,minute,second;
  Serial.println("let's sit time");
  Serial.println("year");
  year=Serial.read();
  Serial.println("month");
  month=Serial.read();
  Serial.println("day");
  day=Serial.read();
  rtc.setDate(day,month,year);
  Serial.println("hour");
  hour=Serial.read();
  Serial.println("minute");
  minute=Serial.read();
  Serial.println("second");
  second=Serial.read();
  rtc.setTime(22, 47, 30);
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
