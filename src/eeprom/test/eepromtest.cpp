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

#include <QtCore/QtGlobal>

#include "eepromtest.hpp"

using namespace qfm1000::eeprom;

QTEST_MAIN(EEPROMTest)

#define TEST_EEPROM_FREQ (unsigned int) 145000000
#define TEST_EEPROM_FREQBAND FrequencyBand::B0
#define TEST_EEPROM_CTCSS (quint8) 1
#define TEST_DEFAULT_CHANNEL 10
#define TEST_TOT 10
#define TEST_LOW_POWER 2

void EEPROMTest::testClear() {
    EEPROM eeprom;

    QCOMPARE(eeprom.getData().size(), EEPROM_SIZE);

    for (int i = 0; i < EEPROM_SIZE; i++)
        QCOMPARE(eeprom.getData()[i], '\0');
}

void EEPROMTest::testChannelRxFreq() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelRxFreq(i, TEST_EEPROM_FREQBAND), (unsigned int) 0);
        eeprom.setChannelRxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        QCOMPARE(eeprom.getChannelRxFreq(i, TEST_EEPROM_FREQBAND), TEST_EEPROM_FREQ);
    }
}

void EEPROMTest::testChannelTxFreq() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelTxFreq(i, TEST_EEPROM_FREQBAND), (unsigned int) 0);
        eeprom.setChannelTxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        QCOMPARE(eeprom.getChannelTxFreq(i, TEST_EEPROM_FREQBAND), TEST_EEPROM_FREQ);
    }
}

void EEPROMTest::testChannelRxCtcss() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelRxCtcss(i), (quint8) 0);
        eeprom.setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        QCOMPARE(eeprom.getChannelRxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

void EEPROMTest::testChannelTxCtcss() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelTxCtcss(i), (quint8) 0);
        eeprom.setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        QCOMPARE(eeprom.getChannelTxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

void EEPROMTest::testChannelPower() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelPower(i), (unsigned int) 0);

        for (unsigned int p = 0; p <= 5; p++) {
            eeprom.setChannelPower(i, p);
            unsigned int power = eeprom.getChannelPower(i);
            QCOMPARE(power, p);
        }

        eeprom.setChannelPower(i, 6);
        QCOMPARE(eeprom.getChannelPower(i), (unsigned int) 0);
    }
}

void EEPROMTest::testChannelSquelch() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelSquelch(i), (unsigned int) 0);

        for (unsigned int p = 0; p <= 6; p++) {
            eeprom.setChannelSquelch(i, p);
            unsigned int squelch = eeprom.getChannelSquelch(i);
            QCOMPARE(squelch, p);
        }

        eeprom.setChannelSquelch(i, 7);
        QCOMPARE(eeprom.getChannelSquelch(i), (unsigned int) 0);
    }
}

void EEPROMTest::testChannelSelectiveCalling() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelSelectiveCalling(i), false);
        eeprom.setChannelSelectiveCalling(i, true);
        QCOMPARE(eeprom.getChannelSelectiveCalling(i), true);
        eeprom.setChannelSelectiveCalling(i, false);
        QCOMPARE(eeprom.getChannelSelectiveCalling(i), false);
    }
}

void EEPROMTest::testChannelCpuOffset() {
    EEPROM eeprom;

    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom.getChannelCpuOffset(i), false);
        eeprom.setChannelCpuOffset(i, true);
        QCOMPARE(eeprom.getChannelCpuOffset(i), true);
        eeprom.setChannelCpuOffset(i, false);
        QCOMPARE(eeprom.getChannelCpuOffset(i), false);
    }
}

void EEPROMTest::testDefaultChannel() {
    EEPROM eeprom;

    QCOMPARE(eeprom.getDefaultChannel(), 0);
    eeprom.setDefaultChannel(TEST_DEFAULT_CHANNEL);
    QCOMPARE(eeprom.getDefaultChannel(), TEST_DEFAULT_CHANNEL);
}

void EEPROMTest::testTot() {
    EEPROM eeprom;

    QCOMPARE(eeprom.getTot(), (quint8) 0);
    eeprom.setTot(TEST_TOT);
    QCOMPARE(eeprom.getTot(), (quint8) TEST_TOT);
}

void EEPROMTest::testLowPower() {
    EEPROM eeprom;

    QCOMPARE(eeprom.getLowPower(), 0);
    eeprom.setLowPower(TEST_LOW_POWER);
    QCOMPARE(eeprom.getLowPower(), TEST_LOW_POWER);
}
