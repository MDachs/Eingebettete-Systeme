#include <DueTimer.h>
#include <SPI.h>

DueTimer timer;
DueTimer timer2;

uint8_t buffer[6 * 84];
int listSize = 0;


//LCD Pins
const int PIN_RST = 9;
const int PIN_CS = 10; // SCE PIN
const int PIN_DC = 3;

int playing = 0;

void setup() {

  Serial.begin(115200);
  Serial1.begin(9600);


  pinMode(31, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);

  initLCD();


  if (timer.configure(1000, entprellen))
  {
    timer.start();
  }
}

void loop() {
  checker(Serial1.readStringUntil('\n'));

  char cplay[] = "Playing";
  char crec[] = "Recording";
  printString(0, 0, (1) ? cplay : crec);

  updateDisplay();

}

bool checker(String str) {
  int checksum = 0;


  for (int i = str.lastIndexOf('$') + 1; i < str.indexOf('*'); i++) {
    checksum = checksum ^ str.charAt(i);
  }

  int ochecksum = str.substring(str.lastIndexOf('*') + 1, str.lastIndexOf('*') + 2).toInt();

  if (checksum == ochecksum) {
    printf("Str: %s Checksum:%02X\n", str.c_str(), checksum);
  }
}












