/*
 * qFM1000
 * Copyright (C) 2017  Luca Cireddu
 * sardylan@gmail.com
 * http://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "eeprombytetest.hpp"

QTEST_MAIN(EEPROMByteTest)

#define CUSTOM_QTRY_COMPARE_NO_TIMEOUT(expr, expected) QTRY_COMPARE_WITH_TIMEOUT((expr), expected, 0)

#define TEST_EEPROM_FREQ (unsigned int) 145000000
#define TEST_EEPROM_FREQ_MSB (char) 0x5a
#define TEST_EEPROM_FREQ_LSB (char) 0xa0

#define TEST_EEPROM_CTCSS (uint8_t) 1
#define TEST_DEFAULT_CHANNEL 10
#define TEST_TOT 10
#define TEST_LOW_POWER 2

void EEPROMByteTest::initTestCase() {
    eeprom = EEPROM::getInstance();
}

void EEPROMByteTest::cleanupTestCase() {

}

void EEPROMByteTest::init() {
    eeprom->clear();
}

void EEPROMByteTest::cleanup() {

}

void EEPROMByteTest::testSimpleBitwise() {
    uint8_t input;
    uint8_t expected;

    input = 0b00000000;
    expected = 0b001100111;
    input |= 0b001100111;
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(input, expected);

    input = 0b11111111;
    expected = 0b001100111;
    input &= 0b001100111;
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(input, expected);

    input = 0b01010101;
    expected = 0b01010111;
    input |= 0b00000010;
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(input, expected);

    input = 0b01010111;
    expected = 0b01010101;
    input &= 0b11111101;
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(input, expected);

    input = 0b01010111;
    expected = 0b01010111;
    input &= 0b11111111;
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(input, expected);
}

void EEPROMByteTest::testSimpleQByteArray() {
    QByteArray data;

    data.clear();
    for (int i = 0; i < EEPROM_SIZE; i++)
        data.append((char) '\0');

    char byte = data[10];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

    data[10] = 0x30;
    byte = data[10];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x30);
}

void EEPROMByteTest::testEepromQByteArray() {
    char byte = eeprom->getData()[10];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

    eeprom->setTot(0x30);
    byte = eeprom->getData()[OFFSET_TOT];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x30);
}

void EEPROMByteTest::testClear() {
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getData().size(), EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; i++)
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getData()[i], '\0');
}

void EEPROMByteTest::testChannelRxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char msb = eeprom->getData()[offset];
        char lsb = eeprom->getData()[offset + 1];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(msb, (char) 0x00);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(lsb, (char) 0x00);

        eeprom->setChannelRxFreq(i, TEST_EEPROM_FREQ);
        msb = eeprom->getData()[offset];
        lsb = eeprom->getData()[offset + 1];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(msb, TEST_EEPROM_FREQ_MSB);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

void EEPROMByteTest::testChannelTxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char msb = eeprom->getData()[offset + 2];
        char lsb = eeprom->getData()[offset + 3];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(msb, (char) 0x00);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(lsb, (char) 0x00);

        eeprom->setChannelTxFreq(i, TEST_EEPROM_FREQ);
        msb = eeprom->getData()[offset + 2];
        lsb = eeprom->getData()[offset + 3];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(msb, TEST_EEPROM_FREQ_MSB);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

void EEPROMByteTest::testChannelRxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom->getData()[offset + 4];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

        eeprom->setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom->getData()[offset + 4];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) TEST_EEPROM_CTCSS);
    }
}

void EEPROMByteTest::testChannelTxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom->getData()[offset + 5];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

        eeprom->setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom->getData()[offset + 5];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) TEST_EEPROM_CTCSS);
    }
}

void EEPROMByteTest::testChannelPower() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom->getData()[offset + 6];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

        char value;
        for (unsigned int p = 0; p <= 5; p++) {
            value = eeprom->getData()[offset + 6];
            eeprom->setChannelPower(i, p);

            byte = eeprom->getData()[offset + 6];

            switch (p) {
                case 5:
                    value &= 0b11000111;
                    value |= 0b00101000;
                    break;
                case 4:
                    value &= 0b11000111;
                    value |= 0b00100000;
                    break;
                case 3:
                    value &= 0b11000111;
                    value |= 0b00011000;
                    break;
                case 2:
                    value &= 0b11000111;
                    value |= 0b00010000;
                    break;
                case 1:
                    value &= 0b11000111;
                    value |= 0b00001000;
                    break;
                case 0:
                default:
                    value &= 0b11000111;
            }

            CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, value);
        }

        value = eeprom->getData()[offset + 6];
        value &= 0b11000111;

        eeprom->setChannelPower(i, 6);
        byte = eeprom->getData()[offset + 6];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, value);
    }
}

void EEPROMByteTest::testChannelSquelch() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (char) 0x00);

        char value;
        for (unsigned int p = 0; p <= 5; p++) {
            value = eeprom->getData()[offset + 7];
            eeprom->setChannelSquelch(i, p);

            byte = eeprom->getData()[offset + 7];

            switch (p) {
                case 6:
                    value &= 0b11100011;
                    value |= 0b00011000;
                    break;
                case 5:
                    value &= 0b11100011;
                    value |= 0b00010100;
                    break;
                case 4:
                    value &= 0b11100011;
                    value |= 0b00010000;
                    break;
                case 3:
                    value &= 0b11100011;
                    value |= 0b00001100;
                    break;
                case 2:
                    value &= 0b11100011;
                    value |= 0b00001000;
                    break;
                case 1:
                    value &= 0b11100011;
                    value |= 0b00000100;
                    break;
                case 0:
                default:
                    value &= 0b11100011;
            }

            CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, value);
        }

        value = eeprom->getData()[offset + 7];
        value &= 0b11100011;

        eeprom->setChannelSquelch(i, 7);
        byte = eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, value);
    }
}

void EEPROMByteTest::testChannelSelectiveCalling() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        uint8_t byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000010), false);

        eeprom->setChannelSelectiveCalling(i, true);
        byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000010), true);

        eeprom->setChannelSelectiveCalling(i, false);
        byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000010), false);
    }
}

void EEPROMByteTest::testChannelCpuOffset() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        uint8_t byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000001), false);

        eeprom->setChannelCpuOffset(i, true);
        byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000001), true);

        eeprom->setChannelCpuOffset(i, false);
        byte = (uint8_t) eeprom->getData()[offset + 7];
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT((bool) (byte & 0b00000001), false);
    }
}

void EEPROMByteTest::testDefaultChannel() {
    uint8_t byte = (uint8_t) eeprom->getData()[OFFSET_STARTUP_CHANNEL];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) 0x00);

    eeprom->setDefaultChannel(TEST_DEFAULT_CHANNEL);
    byte = (uint8_t) eeprom->getData()[OFFSET_STARTUP_CHANNEL];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) TEST_DEFAULT_CHANNEL);
}

void EEPROMByteTest::testTot() {
    uint8_t byte = (uint8_t) eeprom->getData()[OFFSET_TOT];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) 0x00);

    eeprom->setTot(TEST_TOT);
    byte = (uint8_t) eeprom->getData()[OFFSET_TOT];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) TEST_TOT);
}

void EEPROMByteTest::testLowPower() {
    uint8_t byte = (uint8_t) eeprom->getData()[OFFSET_LOW_POWER];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) 0x00);

    eeprom->setLowPower(TEST_LOW_POWER);
    byte = (uint8_t) eeprom->getData()[OFFSET_LOW_POWER];
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(byte, (uint8_t) TEST_LOW_POWER);
}
