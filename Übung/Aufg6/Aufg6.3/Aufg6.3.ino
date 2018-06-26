#include <Servo.h>
#include <DueTimer.h>
#include <SPI.h>

Servo tiltServo;
Servo swingServo;

DueTimer timer;
DueTimer timer2;

uint8_t buffer[6 * 84];
int listSize = 0;


//LCD Pins
const int PIN_RST = 9;
const int PIN_CS = 10; // SCE PIN
const int PIN_DC = 3;

// joystick stuff
const int center = 1023 / 2;

int xpos = center;
int ypos = center;

int playing = 0;

void setup() {

  Serial.begin(115200);

  tiltServo.attach(7);
  swingServo.attach(6);

  pinMode(31, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);

  initLCD();

  //LED
  pinMode(26, OUTPUT);
  
  if (timer.configure(1000, entprellen))
  {
    timer.start();
  }
  if (timer2.configure(50, play))
  {
    timer2.start();
  }
}

void loop() {
  int xOut = analogRead(A8);
  int yOut = analogRead(A9);


  //Difference from Center
  int xoffset = xOut - center;
  int yoffset = yOut - center;

  //Toleranz
  xoffset = (xoffset > 50 || xoffset < -50) ? xoffset : 0;
  yoffset = (yoffset > 50 || yoffset < -50) ? yoffset : 0;

  //Sensitivität
  xpos -= xoffset / 50;
  ypos += yoffset / 50;

  //printf("xout: %d  ", xpos);
  //printf("yout: %d\n", ypos);

  //Boundary Checks
  xpos = (xpos < 1024) ? xpos : 1023;
  ypos = (ypos < 1024) ? ypos : 1023;
  xpos = (xpos >= 0) ? xpos : 0;
  ypos = (ypos >= 0) ? ypos : 0;

  //Skalierung
  xOut = map(xpos, 0, 1023, 0, 180);
  yOut = map(ypos, 0, 1023, 0, 180);
  //Schreiben
  tiltServo.write(xOut);
  swingServo.write(yOut);
  delay(20);

  char cplay[] = "Playing";
  char crec[] = "Recording";
  printString(0,0,(playing) ? cplay : crec);

  char cpos[30] = "";
  sprintf(cpos, "X: %03d Y: %03d", xOut, yOut);
  printString(0,8, cpos);

  char csize[30] = "";
  sprintf(csize, "%03d/%03d", listSize, 10);
  printString(0,16, csize);

  updateDisplay();

}



