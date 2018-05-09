#include <Arduino.h>
#include <Wire.h>

#define PIN_LED 13

#define EEPROM_ADDRESS 0x50
#define BUFFER_SIZE 8

#define PROTOCOL_READY 'L'
#define PROTOCOL_OK 'K'
#define PROTOCOL_ERROR 'E'
#define PROTOCOL_READ 'R'
#define PROTOCOL_WRITE 'W'

int inWrite;
int inRead;

int buf_pos;
byte buffer[BUFFER_SIZE];
byte page;

void setup() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    Wire.begin();
    Serial.begin(9600);

    inWrite = 0;
    inRead = 0;

    buf_pos = 0;
    memset(buffer, '\0', BUFFER_SIZE);
    page = 0x00;

    Serial.write(PROTOCOL_READY);
}

void loop() {
    byte c;

    while (Serial.available()) {
        c = Serial.read();

        if (inWrite == 1) {
            page = c;
            inWrite = 2;
        } else if (inWrite == 2) {
            buffer[buf_pos] = c;
            buf_pos++;

            if (buf_pos == BUFFER_SIZE) {
                if (buffer_write())
                    Serial.write(PROTOCOL_OK);
                else
                    Serial.write(PROTOCOL_ERROR);

                inWrite = 0;
                buf_pos = 0;
            }
        } else if (inRead == 1) {
            page = c;

            buffer_read();
            send_buffer();

            inRead = 0;
        } else {
            switch (c) {
                case PROTOCOL_READ:
                    inRead = 1;
                    break;
                case PROTOCOL_WRITE:
                    inWrite = 1;
                    break;
                default:
                    Serial.write(PROTOCOL_ERROR);
            }
        }
    }
}

word compute_page() {
    return (page * BUFFER_SIZE);
}

int buffer_write() {
    int i;
    word page;

    page = compute_page();

    digitalWrite(PIN_LED, HIGH);

    for (i = 0; i < BUFFER_SIZE; i++)
        eeprom_write(page + i, buffer[i]);

    digitalWrite(PIN_LED, LOW);

    return 1;
}

int buffer_read() {
    int i;
    word page;

    page = compute_page();

    digitalWrite(PIN_LED, HIGH);

    for (i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = eeprom_read(page + i);

    digitalWrite(PIN_LED, LOW);

    return 1;
}

void send_buffer() {
    int i;

    for (i = 0; i < BUFFER_SIZE; i++)
        Serial.write(buffer[i]);
}

byte eeprom_read(word address) {
    byte data = 0;

    Wire.beginTransmission((byte)(EEPROM_ADDRESS | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.endTransmission();

    Wire.requestFrom((byte)(EEPROM_ADDRESS | ((address >> 8) & 0x07)), (byte) 1);
    while (!Wire.available())
        delay(1);

    data = Wire.read();

    return data;
}

void eeprom_write(word address, byte data) {
    Wire.beginTransmission((byte)(EEPROM_ADDRESS | ((address >> 8) & 0x07)));
    Wire.write(address & 0xFF);
    Wire.write(data);
    Wire.endTransmission();

    delay(10);
}
