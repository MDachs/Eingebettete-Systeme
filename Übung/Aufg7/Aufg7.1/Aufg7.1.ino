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


  pinMode(6, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);

  initLCD();


  if (timer.configure(1000, entprellen))
  {
    timer.start();
  }
}

void loop() {
  String str = Serial1.readStringUntil('\n');
  bool correct = checker(str);
  if (correct && str.startsWith("$GPGGA")) {
    int count = 0;
    char* command = strtok((char*)str.c_str(), ",");
    while (command != 0)
    {
      if (count == 2) {
        char lat[40];
        sprintf(lat, "La: %s", command);
        printString(0, 0, lat);
      }
      if (count == 4) {
        printf("Longitude: %s\n", command);
        char lon[40];
        sprintf(lon, "Lo: %s", command);
        printString(0, 8, lon);
      }
      if (count == 6) {
        printf("Pos Indicator: %s\n", command);
        if (atoi(command) == 0) {
          Serial.println("No Signal");
        }
      }
      if (count == 7) {
        printf("Sat count: %s\n", command);
      }

      count++;
      command = strtok(0, ",");
    }
  }

  char cplay[] = "Playing";
  char crec[] = "Recording";
  //printString(0, 0, (1) ? cplay : crec);

  updateDisplay();

}

bool checker(String str) {
  int checksum = 0;


  for (int i = str.indexOf('$') + 1; i < str.indexOf('*'); i++) {
    checksum = checksum ^ str.charAt(i);
  }

  //int ochecksum = str.substring(str.indexOf('*') + 1, str.indexOf('*') + 2).toInt();
  int ochecksum = (int) strtol( &str.c_str()[str.indexOf('*') + 1], NULL, 16);


  if (checksum == ochecksum) {
    printf("Str: %s Checksum:%02X\n", str.c_str(), checksum);
    return true;
  }

  return false;
}












