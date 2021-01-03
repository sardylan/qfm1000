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


#include "eeprom.hpp"

using namespace qfm1000::eeprom;

QTEST_MAIN(EEPROMTest)

#define TEST_EEPROM_FREQ (Frequency) 145000000
#define TEST_EEPROM_FREQBAND FrequencyBand::B0
#define TEST_EEPROM_CTCSS (CTCSS) 1
#define TEST_DEFAULT_CHANNEL (Channel) 10
#define TEST_TOT (TOT) 10
#define TEST_LOW_POWER (Power) 2

[[maybe_unused]] void EEPROMTest::initTestCase() {
    eeprom = new EEPROM();

    QCOMPARE(eeprom->getData().size(), EEPROM_SIZE);
}

[[maybe_unused]] void EEPROMTest::cleanupTestCase() {
    delete eeprom;
}

[[maybe_unused]] void EEPROMTest::init() {
    QCOMPARE(eeprom->getData().size(), EEPROM_SIZE);
}

[[maybe_unused]] void EEPROMTest::cleanup() {
    eeprom->clear();
}

[[maybe_unused]] void EEPROMTest::testClear() {
    for (int i = 0; i < EEPROM_SIZE; i++)
        QCOMPARE(eeprom->getData()[i], '\0');
}

[[maybe_unused]] void EEPROMTest::testChannelRxFreq() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxFreq(i, TEST_EEPROM_FREQBAND), (Channel) 0);
        eeprom->setChannelRxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        QCOMPARE(eeprom->getChannelRxFreq(i, TEST_EEPROM_FREQBAND), TEST_EEPROM_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelTxFreq() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxFreq(i, TEST_EEPROM_FREQBAND), (Channel) 0);
        eeprom->setChannelTxFreq(i, TEST_EEPROM_FREQ, TEST_EEPROM_FREQBAND);
        QCOMPARE(eeprom->getChannelTxFreq(i, TEST_EEPROM_FREQBAND), TEST_EEPROM_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelRxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxCtcss(i), (CTCSS) 0);
        eeprom->setChannelRxCtcss(i, TEST_EEPROM_CTCSS);
        QCOMPARE(eeprom->getChannelRxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelTxCtcss() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxCtcss(i), (CTCSS) 0);
        eeprom->setChannelTxCtcss(i, TEST_EEPROM_CTCSS);
        QCOMPARE(eeprom->getChannelTxCtcss(i), TEST_EEPROM_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelPower() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelPower(i), (Power) 0);

        for (unsigned int p = 0; p <= 5; p++) {
            eeprom->setChannelPower(i, p);
            unsigned int power = eeprom->getChannelPower(i);
            QCOMPARE(power, p);
        }

        eeprom->setChannelPower(i, 6);
        QCOMPARE(eeprom->getChannelPower(i), (Power) 0);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelSquelch() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSquelch(i), (Squelch) 0);

        for (unsigned int p = 0; p <= 6; p++) {
            eeprom->setChannelSquelch(i, p);
            unsigned int squelch = eeprom->getChannelSquelch(i);
            QCOMPARE(squelch, p);
        }

        eeprom->setChannelSquelch(i, 7);
        QCOMPARE(eeprom->getChannelSquelch(i), (Squelch) 0);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelSelectiveCalling() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), false);
        eeprom->setChannelSelectiveCalling(i, true);
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), true);
        eeprom->setChannelSelectiveCalling(i, false);
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), false);
    }
}

[[maybe_unused]] void EEPROMTest::testChannelCpuOffset() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelCpuOffset(i), false);
        eeprom->setChannelCpuOffset(i, true);
        QCOMPARE(eeprom->getChannelCpuOffset(i), true);
        eeprom->setChannelCpuOffset(i, false);
        QCOMPARE(eeprom->getChannelCpuOffset(i), false);
    }
}

[[maybe_unused]] void EEPROMTest::testDefaultChannel() {
    QCOMPARE(eeprom->getDefaultChannel(), (Channel) 0);
    eeprom->setDefaultChannel(TEST_DEFAULT_CHANNEL);
    QCOMPARE(eeprom->getDefaultChannel(), TEST_DEFAULT_CHANNEL);
}

[[maybe_unused]] void EEPROMTest::testTot() {
    QCOMPARE(eeprom->getTot(), (TOT) 0);
    eeprom->setTot(TEST_TOT);
    QCOMPARE(eeprom->getTot(), TEST_TOT);
}

[[maybe_unused]] void EEPROMTest::testLowPower() {
    QCOMPARE(eeprom->getLowPower(), (Power) 0);
    eeprom->setLowPower(TEST_LOW_POWER);
    QCOMPARE(eeprom->getLowPower(), TEST_LOW_POWER);
}
