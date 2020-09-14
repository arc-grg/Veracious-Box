
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
#include<RF24.h>
#include<nRF24L01.h>
#include "printf.h"
#include <Keypad.h>

int box1 = 10;
int box2 = 15;
int box3 = 17;
int box4 = 20;
int t = 0;
char key;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte colPins[ROWS] = {25, 24, 23, 22}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {29, 28, 27, 26}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


int CE_PIN = 9, CSN_PIN = 10;     //   9    10
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t pipeOut = 0xFFBA1B01E1LL;

RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // (rs, e, d4, d5, d6, d7)

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


struct radioData {
  int hr;
} data;

void setup ()
{
  Serial.begin(115200);
  lcd.begin(16, 2);

  if (! rtc.begin())
  {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning())
  {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
  //rtc.adjust(DateTime(19, 4, 2018, 3, 2, 2));// to set the time manualy

  radio.begin();
  printf_begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(57);
  radio.openWritingPipe(pipeOut);
  radio.printDetails();
  delay(2000);



}

void loop ()
{




  DateTime now = rtc.now();
  char key = keypad.getKey();
  // just print the pressed key





  if (key == '#')
  {
    DateTime now = rtc.now();
    while (1)
    {
      DateTime now = rtc.now();
      char key = keypad.getKey();

      int alarm = now.minute() % 2;
      data.hr = alarm;
      radio.write(&data, sizeof(data));
      lcd.clear();
      digitalWrite(14, LOW);
      digitalWrite(15, LOW);
      digitalWrite(16, LOW);
      digitalWrite(17, LOW);
      if (alarm == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);

        if (t == 0)
        {
          Serial.print("BOX1");
          lcd.print("  ----BOX1----  ");
          lcd.setCursor(0, 1);
          lcd.print("NO. of Pills--");
          lcd.print(box1);
          digitalWrite(14, HIGH);
          t = t + 1 ;
          //box1 = box1 - 1;
          Serial.println(t);
          char key = keypad.getKey();
          if (key == '*')
          {
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print(now.hour());
            lcd.print(':');
            lcd.print(now.minute());
            lcd.print(':');
            lcd.print(now.second());
            lcd.print("   ");

            Serial.print(now.hour());
            Serial.print(':');
            Serial.print(now.minute());
            Serial.print(':');
            Serial.print(now.second());
            Serial.print("    ");



            lcd.setCursor(0, 0);
            lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
            lcd.print(" ,");
            lcd.print(now.day());
            lcd.print('/');
            lcd.print(now.month());
            lcd.print('/');
            lcd.print(now.year());


            Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
            Serial.print(" ,");
            Serial.print(now.day());
            Serial.print('/');
            Serial.print(now.month());
            Serial.print('/');
            Serial.print(now.year());
            Serial.print("        ");
            Serial.print(alarm);
            Serial.println("    ");
            delay(500);

            box1 = box1 - 1;
          }


          delay(60000);
        }
        else if (t == 1)
        {
          Serial.println("  ----BOX2----  ");
          lcd.print("  ----BOX2----  ");
          lcd.setCursor(0, 1);
          lcd.print("No. of Pills--");
          lcd.print(box2);
          digitalWrite(15, HIGH);
          lcd.print("BOX2");
          t = t + 1;
          box2 = box2 - 1;

          delay(60000);
        }
        else if (t == 2)
        {
          lcd.print("  ----BOX3----  ");
          lcd.setCursor(0, 1);
          lcd.print("No. of Pills--");
          lcd.print(box3);
          digitalWrite(16, HIGH);
          lcd.print("BOX3");
          t = t + 1;
          box3 = box3 - 1;
          delay(60000);
        }
        else if (t == 3)
        {
          lcd.println("  ----BOX4----  ");
          lcd.setCursor(0, 1);
          lcd.print("No. of Pills--");
          lcd.print(box3);
          digitalWrite(17, HIGH);
          lcd.print("  ----BOX4----  ");
          t = 0;
          box4 = box4 - 1;
          delay(60000);
        }
      }

      else if (key == 'A')
      {
        Serial.println("box 1");
        lcd.print("  ----BOX1----  ");
        lcd.setCursor(0, 1);
        lcd.print("Med. Rem.---");

        lcd.print(box1);


        delay(5000);
      }
      else if (key == 'B')
      { lcd.print("  ----BOX 2----  ");
        lcd.setCursor(0, 1);
        lcd.print("Med. Rem.---");

        lcd.print(box2);

        Serial.println("box2");
        delay(5000);

      }
      else if (key == 'C')
      {
        Serial.println("box3");
        lcd.print("  ----BOX 3----  ");
        lcd.setCursor(0, 1);
        lcd.print("Med. Rem.---");

        lcd.print(box3);
        delay(5000);
      }
      else if (key == 'D')
      {
        Serial.println("box4");
        lcd.print("  ----BOX 4----  ");
        lcd.setCursor(0, 1);
        lcd.print("Med. Rem.---");

        lcd.print(box4);
        delay(5000);
      }



      else
      {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(now.hour());
        lcd.print(':');
        lcd.print(now.minute());
        lcd.print(':');
        lcd.print(now.second());
        lcd.print("   ");

        Serial.print(now.hour());
        Serial.print(':');
        Serial.print(now.minute());
        Serial.print(':');
        Serial.print(now.second());
        Serial.print("    ");



        lcd.setCursor(0, 0);
        lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
        lcd.print(" ,");
        lcd.print(now.day());
        lcd.print('/');
        lcd.print(now.month());
        lcd.print('/');
        lcd.print(now.year());


        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(" ,");
        Serial.print(now.day());
        Serial.print('/');
        Serial.print(now.month());
        Serial.print('/');
        Serial.print(now.year());
        Serial.print("        ");
        Serial.print(alarm);
        Serial.println("    ");
        delay(500);

      }


    }

  }


}
