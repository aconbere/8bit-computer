int shiftData = 2;
int shiftClk = 3;
int shiftLatch = 4;
int eepromD0 = 5;
int eepromD7 = 12;
int writeEn = 13;

/* When outputEnable is true this will set the EEPROM into
    output mode where we will be able to read in that address
    from the chip.
*/
void setAddress(int address, bool outputEnable) {
  int outputEnableMask = outputEnable ? 0x00 : 0x80;
  shiftOut(shiftData, shiftClk, MSBFIRST, (address >> 8) | outputEnableMask);
  shiftOut(shiftData, shiftClk, MSBFIRST, address);

  digitalWrite(shiftLatch, LOW);
  digitalWrite(shiftLatch, HIGH);
  digitalWrite(shiftLatch, LOW);
}

byte readEEPROM(int address) {
  for (int pin = eepromD0; pin <= eepromD7; pin++) {
    pinMode(pin, INPUT);
  }
  setAddress(address, true);

  byte b = 0;

  for (int pin = eepromD7; pin >= eepromD0; pin--) {
    b = (b << 1) + digitalRead(pin);
  }

  return b;
}

void printContents() {
  for (int base = 0; base <= 255; base+=16) {
    byte data[16];22

    for (int offset = 0; offset <= 15; offset++) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x     %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10],
            data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void writeEEPROM(int address, byte data) {
  setAddress(address, false);

  for (int pin = eepromD0; pin <= eepromD7; pin++) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = eepromD0; pin <= eepromD7; pin++) {
    digitalWrite(pin, data & 0x01);
    data = data >> 1;
  }

  digitalWrite(writeEn, LOW);
  delayMicroseconds(1);
  digitalWrite(writeEn, HIGH);
  delay(10);
}

void setup() {
  pinMode(shiftData, OUTPUT);
  pinMode(shiftClk, OUTPUT);
  pinMode(shiftLatch, OUTPUT);
  digitalWrite(writeEn, HIGH);
  pinMode(writeEn, OUTPUT);

  Serial.begin(57600);

  writeEEPROM(0x00, 0x00);
  writeEEPROM(0x01, 0x01);
  writeEEPROM(0x02, 0x02);
  writeEEPROM(0x03, 0x03);
  writeEEPROM(0x04, 0x04);
  writeEEPROM(0x05, 0x05);
  writeEEPROM(0x06, 0x06);

  printContents();
}



void loop() {

}
