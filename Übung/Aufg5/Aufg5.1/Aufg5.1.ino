#include <SPI.h>

const int PIN_RST = 3;
const int PIN_CS = 10; // SCE PIN
const int PIN_DC = 9;

uint8_t buffer[6*84];

void setup() {
  // Display initialisieren
  pinMode(PIN_RST, OUTPUT);
  pinMode(PIN_DC, OUTPUT);

  SPI.begin(PIN_CS);

  // Reset durchführen
  digitalWrite(PIN_RST, LOW);
  delay(500);
  digitalWrite(PIN_RST, HIGH);
  delay(500);

  //Datenübertragung initialisieren
  digitalWrite(PIN_DC, LOW);
  SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(PIN_CS, 0x21);// FUNCTION SET (extended instruction set)
  SPI.transfer(PIN_CS, 0x13);// SET BIAS (1:48 MUX, 1/8 BIAS)
  SPI.transfer(PIN_CS, 0xBF);// SET CONTRAST (V_OP = 6,18 V)
  SPI.transfer(PIN_CS, 0x04);// SET TEMPERATURE COEFFICIENT
  SPI.transfer(PIN_CS, 0x20);// FUNCTION SET (normal instruction set)
  SPI.transfer(PIN_CS, 0x0C);// SET DISPLAY MODE (normal)
  SPI.transfer(PIN_CS, 0x40);// SET Y COORDINATE = 0
  SPI.transfer(PIN_CS, 0x80);// SET X COORDINATE = 0
  SPI.endTransaction();

  digitalWrite(PIN_DC, HIGH);
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, 0xFF);
    SPI.endTransaction();
  }
  delay(1000);
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, 0x00);
    SPI.endTransaction();
  }


}

void loop() {
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, buffer[i]);
    SPI.endTransaction();
  }

  delay(500);
  buffer[0] = 0xFF;

}

void setPixel(int x , int y, int value){
  buffer[0 * x] = value;
  
  
}

