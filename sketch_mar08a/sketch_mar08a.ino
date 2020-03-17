#include <LiquidCrystal_I2C.h> //引用I2C库
#include <Wire.h>
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
String input;
File myFile;

void setup ()
{
  Serial.begin(9600);
  lcd.init(); // 初始化LCD
  lcd.backlight(); //设置LCD背景等亮
  input = ' ';
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
void loop() {
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
  lcd.setCursor(7, 1);
  lcd.print(input);
  delay(100);
}
//Serial.println("你要输出的内容)
