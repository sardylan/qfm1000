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

#include "eepromtest.hpp"

QTEST_MAIN(EEPROMTest)

#define CUSTOM_QTRY_COMPARE_NO_TIMEOUT(expr, expected) QTRY_COMPARE_WITH_TIMEOUT((expr), expected, 0)

#define TEST_EEPROM_FREQ (unsigned int) 145000000
#define TEST_EEPROM_CTCSS (uint8_t) 1
#define TEST_DEFAULT_CHANNEL 10
#define TEST_TOT 10
#define TEST_LOW_POWER 2

void EEPROMTest::initTestCase() {
    eeprom = EEPROM::getInstance();
}

void EEPROMTest::cleanupTestCase() {

}

void EEPROMTest::init() {
    eeprom->clear();
}

void EEPROMTest::cleanup() {

}

void EEPROMTest::testClear() {
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getData().size(), EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; i++)
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getData()[i], '\0');
}

void EEPROMTest::testChannelRxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelRxFreq(i), (unsigned int) 0);
        eeprom->setChannelRxFreq(i, TEST_EEPROM_FREQ);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelRxFreq(i), TEST_EEPROM_FREQ);
    }
}

void EEPROMTest::testChannelTxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelTxFreq(i), (unsigned int) 0);
        eeprom->setChannelTxFreq(i, TEST_EEPROM_FREQ);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelTxFreq(i), TEST_EEPROM_FREQ);
    }
}

void EEPROMTest::testChannelRxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelRxCtcss(i), (uint8_t) 0);
        eeprom->setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelRxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

void EEPROMTest::testChannelTxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelTxCtcss(i), (uint8_t) 0);
        eeprom->setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelTxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

void EEPROMTest::testChannelPower() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelPower(i), (unsigned int) 0);

        for (unsigned int p = 0; p <= 5; p++) {
            eeprom->setChannelPower(i, p);
            unsigned int power = eeprom->getChannelPower(i);
            CUSTOM_QTRY_COMPARE_NO_TIMEOUT(power, p);
        }

        eeprom->setChannelPower(i, 6);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelPower(i), (unsigned int) 0);
    }
}

void EEPROMTest::testChannelSquelch() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelSquelch(i), (unsigned int) 0);

        for (unsigned int p = 0; p <= 6; p++) {
            eeprom->setChannelSquelch(i, p);
            unsigned int squelch = eeprom->getChannelSquelch(i);
            CUSTOM_QTRY_COMPARE_NO_TIMEOUT(squelch, p);
        }

        eeprom->setChannelSquelch(i, 7);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelSquelch(i), (unsigned int) 0);
    }
}

void EEPROMTest::testChannelSelectiveCalling() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelSelectiveCalling(i), false);
        eeprom->setChannelSelectiveCalling(i, true);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelSelectiveCalling(i), true);
        eeprom->setChannelSelectiveCalling(i, false);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelSelectiveCalling(i), false);
    }
}

void EEPROMTest::testChannelCpuOffset() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelCpuOffset(i), false);
        eeprom->setChannelCpuOffset(i, true);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelCpuOffset(i), true);
        eeprom->setChannelCpuOffset(i, false);
        CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getChannelCpuOffset(i), false);
    }
}

void EEPROMTest::testDefaultChannel() {
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getDefaultChannel(), 0);
    eeprom->setDefaultChannel(TEST_DEFAULT_CHANNEL);
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getDefaultChannel(), TEST_DEFAULT_CHANNEL);
}

void EEPROMTest::testTot() {
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getTot(), (uint8_t) 0);
    eeprom->setTot(TEST_TOT);
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getTot(), (uint8_t) TEST_TOT);
}

void EEPROMTest::testLowPower() {
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getLowPower(), 0);
    eeprom->setLowPower(TEST_LOW_POWER);
    CUSTOM_QTRY_COMPARE_NO_TIMEOUT(eeprom->getLowPower(), TEST_LOW_POWER);
}
