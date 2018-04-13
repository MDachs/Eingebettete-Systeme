#include <DueTimer.h>
auto led_pin = 7;
volatile uint8_t led_power = 0;
DueTimer timer;

int last3State = HIGH;
int last3Count = 0;
int last5State = HIGH;
int last5Count = 0;

void entprellen(void)
{
  int new3State = digitalRead(3);
  int new5State = digitalRead(5);
  if (new3State == LOW && last3State == HIGH) {
    //Pressed 3
    last3Count = 0;
  }
  if (new3State == HIGH && last3State == HIGH) {
    //Released 3
    if (last3Count <= 50) {
      last3Count++;
    }
    if (last3Count == 50) {
      firstReleased();
    }
  }
  if (new5State == LOW && last5State == HIGH) {
    //Pressed 5
    last5Count = 0;
  }
  if (new5State == HIGH && last5State == HIGH) {
    //Released 5
    if (last5Count <= 50) {
      last5Count++;
    }
    if (last5Count == 50) {
      secondReleased();
    }
  }

  last3State = new3State;
  last5State = new5State;
}
void setup()
{
  Serial.begin(9600); 
  // Konfiguration der Richtung des digitalen I/O Anschlusspins 13
  pinMode(led_pin, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  // Konfiguration des Hardware-Timers (@ 1 Hz)
  // Syntax: configure(timer_frequency, callback_function)
  if (timer.configure(1000, entprellen))
  {
    timer.start();
  }
}

void firstReleased(){
  led_power += 30;
}

void secondReleased(){
  led_power -= 30;
}

void loop()
{
  analogWrite(led_pin, led_power);
}
