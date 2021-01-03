/*
 * qFM1000
 * Copyright (C) 2021  Luca Cireddu - IS0GVH
 * sardylan@gmail.com - is0gvh@gmail.com
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


#include "eeprombyte.hpp"

using namespace qfm1000::eeprom;

QTEST_MAIN(EEPROMByteTest)

#define TEST_EEPROM_FREQ (Frequency) 145000000
#define TEST_EEPROM_FREQBAND FrequencyBand::B0
#define TEST_EEPROM_FREQ_MSB (quint8) 0x5a
#define TEST_EEPROM_FREQ_LSB (quint8) 0xa0

#define TEST_EEPROM_CTCSS (CTCSS) 1
#define TEST_DEFAULT_CHANNEL (Channel) 10
#define TEST_TOT (TOT) 10
#define TEST_LOW_POWER (Power) 2

[[maybe_unused]] void EEPROMByteTest::initTestCase() {
    eeprom = new EEPROM();
}

[[maybe_unused]] void EEPROMByteTest::cleanupTestCase() {
    delete eeprom;
}

[[maybe_unused]] void EEPROMByteTest::init() {
    QCOMPARE(eeprom->getData().size(), EEPROM_SIZE);
}

[[maybe_unused]] void EEPROMByteTest::cleanup() {
    eeprom->clear();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

[[maybe_unused]] void EEPROMByteTest::testSimpleBitwise() {
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

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

[[maybe_unused]] void EEPROMByteTest::testSimpleQByteArray() {
    QByteArray data;

    data.clear();
    for (int i = 0; i < EEPROM_SIZE; i++)
        data.append((quint8) '\0');

    quint8 byte = data[10];
    QCOMPARE(byte, (quint8) 0x00);

    data[10] = 0x30;
    byte = data[10];
    QCOMPARE(byte, (quint8) 0x30);
}

#pragma clang diagnostic pop

[[maybe_unused]] void EEPROMByteTest::testEepromQByteArray() {
    quint8 byte = eeprom->getData()[10];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom->setTot(0x30);
    byte = eeprom->getData()[OFFSET_TOT];
    QCOMPARE(byte, (quint8) 0x30);
}

[[maybe_unused]] void EEPROMByteTest::testClear() {
    for (int i = 0; i < EEPROM_SIZE; i++)
        QCOMPARE(eeprom->getData()[i], '\0');
}

[[maybe_unused]] void EEPROMByteTest::testChannelRxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 msb = eeprom->getData()[offset];
        quint8 lsb = eeprom->getData()[offset + 1];
        QCOMPARE(msb, (quint8) 0x00);
        QCOMPARE(lsb, (quint8) 0x00);

        eeprom->setChannelRxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        msb = eeprom->getData()[offset];
        lsb = eeprom->getData()[offset + 1];
        QCOMPARE(msb, TEST_EEPROM_FREQ_MSB);
        QCOMPARE(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelTxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 msb = eeprom->getData()[offset + 2];
        quint8 lsb = eeprom->getData()[offset + 3];
        QCOMPARE(msb, (quint8) 0x00);
        QCOMPARE(lsb, (quint8) 0x00);

        eeprom->setChannelTxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        msb = eeprom->getData()[offset + 2];
        lsb = eeprom->getData()[offset + 3];
        QCOMPARE(msb, TEST_EEPROM_FREQ_MSB);
        QCOMPARE(lsb, TEST_EEPROM_FREQ_LSB);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelRxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 byte = eeprom->getData()[offset + 4];
        QCOMPARE(byte, (quint8) 0x00);

        eeprom->setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom->getData()[offset + 4];
        QCOMPARE(byte, (quint8) TEST_EEPROM_CTCSS);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelTxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 byte = eeprom->getData()[offset + 5];
        QCOMPARE(byte, (quint8) 0x00);

        eeprom->setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        byte = eeprom->getData()[offset + 5];
        QCOMPARE(byte, (quint8) TEST_EEPROM_CTCSS);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelPower() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 byte = eeprom->getData()[offset + 6];
        QCOMPARE(byte, (quint8) 0x00);

        quint8 value;
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

            QCOMPARE(byte, value);
        }

        value = eeprom->getData()[offset + 6];
        value &= 0b11000111;

        eeprom->setChannelPower(i, 6);
        byte = eeprom->getData()[offset + 6];
        QCOMPARE(byte, value);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelSquelch() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        quint8 byte = eeprom->getData()[offset + 7];
        QCOMPARE(byte, (quint8) 0x00);

        quint8 value;
        for (unsigned int p = 0; p <= 6; p++) {
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

            QCOMPARE(byte, value);
        }

        value = eeprom->getData()[offset + 7];
        value &= 0b11100011;

        eeprom->setChannelSquelch(i, 7);
        byte = eeprom->getData()[offset + 7];
        QCOMPARE(byte, value);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelSelectiveCalling() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        auto byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), false);

        eeprom->setChannelSelectiveCalling(i, true);
        byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), true);

        eeprom->setChannelSelectiveCalling(i, false);
        byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000010), false);
    }
}

[[maybe_unused]] void EEPROMByteTest::testChannelCpuOffset() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);
        auto byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), false);

        eeprom->setChannelCpuOffset(i, true);
        byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), true);

        eeprom->setChannelCpuOffset(i, false);
        byte = (quint8) eeprom->getData()[offset + 7];
        QCOMPARE((bool) (byte & 0b00000001), false);
    }
}

[[maybe_unused]] void EEPROMByteTest::testDefaultChannel() {
    auto byte = (quint8) eeprom->getData()[OFFSET_STARTUP_CHANNEL];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom->setDefaultChannel(TEST_DEFAULT_CHANNEL);
    byte = (quint8) eeprom->getData()[OFFSET_STARTUP_CHANNEL];
    QCOMPARE(byte, (quint8) TEST_DEFAULT_CHANNEL);
}

[[maybe_unused]] void EEPROMByteTest::testTot() {
    auto byte = (quint8) eeprom->getData()[OFFSET_TOT];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom->setTot(TEST_TOT);
    byte = (quint8) eeprom->getData()[OFFSET_TOT];
    QCOMPARE(byte, (quint8) TEST_TOT);
}

[[maybe_unused]] void EEPROMByteTest::testLowPower() {
    auto byte = (quint8) eeprom->getData()[OFFSET_LOW_POWER];
    QCOMPARE(byte, (quint8) 0x00);

    eeprom->setLowPower(TEST_LOW_POWER);
    byte = (quint8) eeprom->getData()[OFFSET_LOW_POWER];
    QCOMPARE(byte, (quint8) TEST_LOW_POWER);
}
