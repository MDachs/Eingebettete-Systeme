extern uint8_t buffer[6 * 84];

void initLCD {
  pinMode(PIN_RST, OUTPUT);
  pinMode(PIN_DC, OUTPUT);

  SPI.begin(PIN_CS);
  SD.begin(PIN_SD);
  Serial.begin(115200);
  restLCD;
}

void resetLCD {
  digitalWrite(PIN_RST, LOW);
  delay(500);
  digitalWrite(PIN_RST, HIGH);
  delay(500);
}

void clearDisplay(){
  for (int i = 0; i < 84 * 6; i++) {
    buffer[i] = 0x00;
  }
}

void initDataTransferLCD {
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
}

void setPixel(int x , int y, int value) {
  int ypos = y / 8;
  int pos;
  if (ypos == 0) {
    pos = x;
  } else if (x == 0) {
    pos = ypos * 84;
  } else {
    pos = x + (ypos * 84);
  }
  
  if (value == 1) {
    buffer[pos] |= 1 << (y % 8);
  } else {
    buffer[pos] &= ~(1 << (y % 8));
  }
}

int printChar(int x, int y, char value) {
  if (y < 0 || y > 39 || x < 0 || x > 77){
    return -1;
  }
  
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      int val = font[value-32][i] & (1 << j);
      int val2 = 0;
      if (val > 0){
        val2 = 1;
      }
      setPixel(x + i, y + j, val2);
      //printf("x: %d, y: %d, val2: %d\n", x+i, x+j, val2);
    }
  }
}

int printString(int x, int y, char* c_str){
  char * t;
  int i = 0;
   for (t = c_str; *t != '\0'; t++) {
    if (printChar(x+i*6,y,*t) == -1){
      return -1;
    }
    i++;
  }
}



