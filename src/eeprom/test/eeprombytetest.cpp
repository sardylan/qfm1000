/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
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

using namespace qfm1000::eeprom;

QTEST_MAIN(EEPROMByteTest)

#define TEST_EEPROM_FREQ (unsigned int) 145000000
#define TEST_EEPROM_FREQBAND FrequencyBand::B0
#define TEST_EEPROM_FREQ_MSB (char) 0x5a
#define TEST_EEPROM_FREQ_LSB (char) 0xa0

#define TEST_EEPROM_CTCSS (quint8) 1
#define TEST_DEFAULT_CHANNEL 10
#define TEST_TOT 10
#define TEST_LOW_POWER 2

void EEPROMByteTest::testSimpleBitwise() {
    quint8 input;
    quint8 expected;

    input = 0b00000000;
    expected = 0b001100111;
    input |= 0b001100111;
    QCOMPARE(input, expected);

    input = 0b11111111;
    expected = 0b001100111;
    input &= 0b001100111;
    QCOMPARE(input, expected);

    input = 0b01010101;
    expected = 0b01010111;
    input |= 0b00000010;
    QCOMPARE(input, expected);

    input = 0b01010111;
    expected = 0b01010101;
    input &= 0b11111101;
    QCOMPARE(input, expected);

    input = 0b01010111;
    expected = 0b01010111;
    input &= 0b11111111;
    QCOMPARE(input, expected);
}

void EEPROMByteTest::testSimpleQByteArray() {
    QByteArray data;

    data.clear();
    for (int i = 0; i < EEPROM_SIZE; i++)
        data.append((char) '\0');

    char byte = data[10];
    QCOMPARE(byte, (char) 0x00);

    data[10] = 0x30;
    byte = data[10];
    QCOMPARE(byte, (char) 0x30);
}

void EEPROMByteTest::testEepromQByteArray() {
    EEPROM eeprom;

    char byte = eeprom.getData()[10];
    QCOMPARE(byte, (char) 0x00);

    eeprom.setTot(0x30);
    byte = eeprom.getData()[OFFSET_TOT];
    QCOMPARE(byte, (char) 0x30);
}

void EEPROMByteTest::testClear() {
    EEPROM eeprom;

    QCOMPARE(eeprom.getData().size(), EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; i++)
        QCOMPARE(eeprom.getData()[i], '\0');
}

void EEPROMByteTest::testChannelRxFreq() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char msb = eeprom.getData()[offset];
        char lsb = eeprom.getData()[offset + 1];
        QCOMPARE(msb, (char) 0x00);
        QCOMPARE(lsb, (char) 0x00);

        eeprom.setChannelRxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        msb = eeprom.getData()[offset];
        lsb = eeprom.getData()[offset + 1];
        QCOMPARE(msb, TEST_EEPROM_FREQ_MSB);
        QCOMPARE(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

void EEPROMByteTest::testChannelTxFreq() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char msb = eeprom.getData()[offset + 2];
        char lsb = eeprom.getData()[offset + 3];
        QCOMPARE(msb, (char) 0x00);
        QCOMPARE(lsb, (char) 0x00);

        eeprom.setChannelTxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        msb = eeprom.getData()[offset + 2];
        lsb = eeprom.getData()[offset + 3];
        QCOMPARE(msb, TEST_EEPROM_FREQ_MSB);
        QCOMPARE(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

void EEPROMByteTest::testChannelRxCtcss() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom.getData()[offset + 4];
        QCOMPARE(byte, (char) 0x00);

        eeprom.setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom.getData()[offset + 4];
        QCOMPARE(byte, (char) TEST_EEPROM_CTCSS);
    }
}

void EEPROMByteTest::testChannelTxCtcss() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom.getData()[offset + 5];
        QCOMPARE(byte, (char) 0x00);

        eeprom.setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom.getData()[offset + 5];
        QCOMPARE(byte, (char) TEST_EEPROM_CTCSS);
    }
}

void EEPROMByteTest::testChannelPower() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom.getData()[offset + 6];
        QCOMPARE(byte, (char) 0x00);

        char value;
        for (unsigned int p = 0; p <= 5; p++) {
            value = eeprom.getData()[offset + 6];
            eeprom.setChannelPower(i, p);

            byte = eeprom.getData()[offset + 6];

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

            QCOMPARE(byte, value);
        }

        value = eeprom.getData()[offset + 6];
        value &= 0b11000111;

        eeprom.setChannelPower(i, 6);
        byte = eeprom.getData()[offset + 6];
        QCOMPARE(byte, value);
    }
}

void EEPROMByteTest::testChannelSquelch() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        char byte = eeprom.getData()[offset + 7];
        QCOMPARE(byte, (char) 0x00);

        char value;
        for (unsigned int p = 0; p <= 6; p++) {
            value = eeprom.getData()[offset + 7];
            eeprom.setChannelSquelch(i, p);

            byte = eeprom.getData()[offset + 7];

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

            QCOMPARE(byte, value);
        }

        value = eeprom.getData()[offset + 7];
        value &= 0b11100011;

        eeprom.setChannelSquelch(i, 7);
        byte = eeprom.getData()[offset + 7];
        QCOMPARE(byte, value);
    }
}

void EEPROMByteTest::testChannelSelectiveCalling() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        auto byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), false);

        eeprom.setChannelSelectiveCalling(i, true);
        byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), true);

        eeprom.setChannelSelectiveCalling(i, false);
        byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), false);
    }
}

void EEPROMByteTest::testChannelCpuOffset() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        auto byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), false);

        eeprom.setChannelCpuOffset(i, true);
        byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), true);

        eeprom.setChannelCpuOffset(i, false);
        byte = (quint8) eeprom.getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), false);
    }
}

void EEPROMByteTest::testDefaultChannel() {
    EEPROM eeprom;

    auto byte = (quint8) eeprom.getData()[OFFSET_STARTUP_CHANNEL];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom.setDefaultChannel(TEST_DEFAULT_CHANNEL);
    byte = (quint8) eeprom.getData()[OFFSET_STARTUP_CHANNEL];
    QCOMPARE(byte, (quint8) TEST_DEFAULT_CHANNEL);
}

void EEPROMByteTest::testTot() {
    EEPROM eeprom;

    auto byte = (quint8) eeprom.getData()[OFFSET_TOT];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom.setTot(TEST_TOT);
    byte = (quint8) eeprom.getData()[OFFSET_TOT];
    QCOMPARE(byte, (quint8) TEST_TOT);
}

void EEPROMByteTest::testLowPower() {
    EEPROM eeprom;

    auto byte = (quint8) eeprom.getData()[OFFSET_LOW_POWER];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom.setLowPower(TEST_LOW_POWER);
    byte = (quint8) eeprom.getData()[OFFSET_LOW_POWER];
    QCOMPARE(byte, (quint8) TEST_LOW_POWER);
}
