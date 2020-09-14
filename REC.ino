#include<SPI.h>
#include<RF24.h>
#include<nRF24L01.h>
#include "printf.h"

int CE_PIN = 9, CSN_PIN = 10;
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t pipeIn = 0xFFBA1B01E1LL;
int radio_led = 13;

struct radioData {
  int hr;
} data;

void setup()
{
  Serial.begin(115200);
  pinMode(radio_led, OUTPUT);
  digitalWrite(radio_led, LOW);
  start_nrf();
  delay(2000);
}

void loop()
{
  if (radio.available())
  {
    radio.read(&data, sizeof(data));
    digitalWrite(radio_led, HIGH);

    Serial.println(data.hr);
  }
  else
  {
    digitalWrite(radio_led, LOW);
  }

  if (data.hr == 0)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(5,HIGH);
    delay(500);

    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(5,HIGH);
    delay(500);

  }
  else
  {
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(5,HIGH); 


  }
}

void start_nrf()
{
  radio.begin();
  printf_begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(57);
  radio.openReadingPipe(0, pipeIn);
  radio.startListening();
  radio.printDetails();
  delay(2000);
}

