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

int main(int argc, char *argv[]) {
    EEPROM::registerMetaType();

    QTEST_MAIN_IMPL(EEPROMTest)
}

//QTEST_MAIN(EEPROMTest)

#define TEST_FREQ static_cast<Frequency>(145000000)
#define TEST_FREQ_BYTES_MSB '\x5A'
#define TEST_FREQ_BYTES_LSB '\xA0'

#define TEST_FREQBAND FrequencyBand::B0

#define TEST_CTCSS CTCSS::TONE_67_0
#define TEST_CTCSS_BYTES '\x01'

#define TEST_STARTUP_CHANNEL static_cast<Channel>(10)
#define TEST_STARTUP_CHANNEL_BYTES '\x0a'

#define TEST_KEY_BEEP Flag::ENABLED
#define TEST_KEY_BEEP_BYTES '\x01'

#define TEST_TOT static_cast<TOT>(10)
#define TEST_TOT_BYTES '\x0a'

#define TEST_LOW_POWER Power::WATT_1
#define TEST_LOW_POWER_BYTES '\x01'

#define checkSignalCount(signalSpy, count) QVERIFY(signalSpy.wait()); QCOMPARE(signalSpy.size(), count); signalSpy.clear()


[[maybe_unused]] void EEPROMTest::initTestCase() {
    eeprom = new EEPROM();
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

[[maybe_unused]] void EEPROMTest::clear() {
    eeprom->clear();

    for (int i = 0; i < EEPROM_SIZE; i++)
        QCOMPARE(eeprom->getData()[i], '\0');
}

[[maybe_unused]] void EEPROMTest::frequencyBand() {
// TODO
}

[[maybe_unused]] void EEPROMTest::channelRxFreq() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxFreq(i), static_cast<Frequency>(0));

        eeprom->setChannelRxFreq(i, TEST_FREQ);
        checkSignalCount(signalSpy, 2);

        QCOMPARE(eeprom->getChannelRxFreq(i), TEST_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::channelRxFreqBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 0], '\x00');
        QCOMPARE(eeprom->getData()[offset + 1], '\x00');

        eeprom->setChannelRxFreq(i, TEST_FREQ);
        QVERIFY(signalSpy.wait());
        QCOMPARE(signalSpy.size(), 2);

        QCOMPARE(eeprom->getData()[offset + 0], TEST_FREQ_BYTES_MSB);
        QCOMPARE(eeprom->getData()[offset + 1], TEST_FREQ_BYTES_LSB);

        QList<QVariant> signalArguments = signalSpy.takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 0);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_MSB);

        signalArguments = signalSpy.takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 1);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_LSB);

        signalSpy.clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelTxFreq() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxFreq(i), static_cast<Frequency>(0));

        eeprom->setChannelTxFreq(i, TEST_FREQ);
        checkSignalCount(signalSpy, 2);

        QCOMPARE(eeprom->getChannelTxFreq(i), TEST_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::channelTxFreqBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 2], '\x00');
        QCOMPARE(eeprom->getData()[offset + 3], '\x00');

        eeprom->setChannelTxFreq(i, TEST_FREQ);
        QCOMPARE(eeprom->getData()[offset + 2], TEST_FREQ_BYTES_MSB);
        QCOMPARE(eeprom->getData()[offset + 3], TEST_FREQ_BYTES_LSB);
    }
}

[[maybe_unused]] void EEPROMTest::channelRxCtcss() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxCtcss(i), CTCSS::TONE_OFF);

        eeprom->setChannelRxCtcss(i, TEST_CTCSS);
        checkSignalCount(signalSpy, 1);

        QCOMPARE(eeprom->getChannelRxCtcss(i), TEST_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::channelRxCtcssBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 4], '\x00');

        eeprom->setChannelRxCtcss(i, TEST_CTCSS);
        QCOMPARE(eeprom->getData()[offset + 4], TEST_CTCSS_BYTES);
    }
}

[[maybe_unused]] void EEPROMTest::channelTxCtcss() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxCtcss(i), CTCSS::TONE_OFF);

        eeprom->setChannelTxCtcss(i, TEST_CTCSS);
        checkSignalCount(signalSpy, 1);

        QCOMPARE(eeprom->getChannelTxCtcss(i), TEST_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::channelTxCtcssBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 5], '\x00');

        eeprom->setChannelTxCtcss(i, TEST_CTCSS);
        QCOMPARE(eeprom->getData()[offset + 5], TEST_CTCSS_BYTES);
    }
}

[[maybe_unused]] void EEPROMTest::channelPower() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelPower(i), Power::DISABLED);

        eeprom->setChannelPower(i, Power::WATT_1);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_1);

        eeprom->setChannelPower(i, Power::WATT_6);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_6);

        eeprom->setChannelPower(i, Power::WATT_10);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_10);

        eeprom->setChannelPower(i, Power::WATT_15);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_15);

        eeprom->setChannelPower(i, Power::WATT_25);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_25);
    }
}

[[maybe_unused]] void EEPROMTest::channelPowerBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::DISABLED));

        eeprom->setChannelPower(i, Power::WATT_1);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_1));

        eeprom->setChannelPower(i, Power::WATT_6);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_6));

        eeprom->setChannelPower(i, Power::WATT_10);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_10));

        eeprom->setChannelPower(i, Power::WATT_15);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_15));

        eeprom->setChannelPower(i, Power::WATT_25);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_25));
    }
}

[[maybe_unused]] void EEPROMTest::channelSquelch() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::OPEN);

        eeprom->setChannelSquelch(i, Squelch::SINAD_9DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_9DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_12DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_12DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_15DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_15DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_18DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_18DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_21DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_21DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_24DB);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_24DB);
    }
}

[[maybe_unused]] void EEPROMTest::channelSquelchBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::OPEN));

        eeprom->setChannelSquelch(i, Squelch::SINAD_9DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_9DB));

        eeprom->setChannelSquelch(i, Squelch::SINAD_12DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_12DB));

        eeprom->setChannelSquelch(i, Squelch::SINAD_15DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_15DB));

        eeprom->setChannelSquelch(i, Squelch::SINAD_18DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_18DB));

        eeprom->setChannelSquelch(i, Squelch::SINAD_21DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_21DB));

        eeprom->setChannelSquelch(i, Squelch::SINAD_24DB);
        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_24DB));
    }
}

[[maybe_unused]] void EEPROMTest::channelSelectiveCalling() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::DISABLED);

        eeprom->setChannelSelectiveCalling(i, Flag::ENABLED);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::ENABLED);

        eeprom->setChannelSelectiveCalling(i, Flag::DISABLED);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::DISABLED);
    }
}

[[maybe_unused]] void EEPROMTest::channelSelectiveCallingBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::DISABLED));

        eeprom->setChannelSelectiveCalling(i, Flag::ENABLED);
        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::ENABLED));

        eeprom->setChannelSelectiveCalling(i, Flag::DISABLED);
        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::DISABLED));
    }
}

[[maybe_unused]] void EEPROMTest::channelCpuOffset() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::DISABLED);

        eeprom->setChannelCpuOffset(i, Flag::ENABLED);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::ENABLED);

        eeprom->setChannelCpuOffset(i, Flag::DISABLED);
        checkSignalCount(signalSpy, 1);
        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::DISABLED);
    }
}

[[maybe_unused]] void EEPROMTest::channelCpuOffsetBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::DISABLED));

        eeprom->setChannelCpuOffset(i, Flag::ENABLED);
        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::ENABLED));

        eeprom->setChannelCpuOffset(i, Flag::DISABLED);
        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::DISABLED));
    }
}

[[maybe_unused]] void EEPROMTest::startupChannel() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getStartupChannel(), static_cast<Channel>(0));

    eeprom->setStartupChannel(TEST_STARTUP_CHANNEL);
    checkSignalCount(signalSpy, 1);
    QCOMPARE(eeprom->getStartupChannel(), TEST_STARTUP_CHANNEL);
}

[[maybe_unused]] void EEPROMTest::startupChannelBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getData()[OFFSET_STARTUP_CHANNEL], '\x00');

    eeprom->setStartupChannel(TEST_STARTUP_CHANNEL);
    QCOMPARE(eeprom->getData()[OFFSET_STARTUP_CHANNEL], TEST_STARTUP_CHANNEL_BYTES);
}

[[maybe_unused]] void EEPROMTest::keyBeep() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getKeyBeep(), Flag::DISABLED);

    eeprom->setKeyBeep(TEST_KEY_BEEP);
    checkSignalCount(signalSpy, 1);
    QCOMPARE(eeprom->getKeyBeep(), TEST_KEY_BEEP);
}

[[maybe_unused]] void EEPROMTest::keyBeepBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getData()[OFFSET_KEY_BEEP], '\x00');

    eeprom->setKeyBeep(TEST_KEY_BEEP);
    QCOMPARE(eeprom->getData()[OFFSET_KEY_BEEP], TEST_KEY_BEEP_BYTES);
}

[[maybe_unused]] void EEPROMTest::tot() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getTot(), static_cast<TOT>(0));

    eeprom->setTot(TEST_TOT);
    QVERIFY(signalSpy.wait());
    checkSignalCount(signalSpy, 1);
    QCOMPARE(eeprom->getTot(), TEST_TOT);
}

[[maybe_unused]] void EEPROMTest::totBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getData()[OFFSET_TOT], '\x00');

    eeprom->setTot(TEST_TOT);
    QCOMPARE(eeprom->getData()[OFFSET_TOT], TEST_TOT_BYTES);
}

[[maybe_unused]] void EEPROMTest::lowPower() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getLowPower(), Power::DISABLED);

    eeprom->setLowPower(TEST_LOW_POWER);
    checkSignalCount(signalSpy, 1);
    QCOMPARE(eeprom->getLowPower(), TEST_LOW_POWER);
}

[[maybe_unused]] void EEPROMTest::lowPowerBytes() {
    QSignalSpy signalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy.isValid());

    QCOMPARE(eeprom->getData()[OFFSET_LOW_POWER], '\x00');

    eeprom->setLowPower(TEST_LOW_POWER);
    QCOMPARE(eeprom->getData()[OFFSET_LOW_POWER], TEST_LOW_POWER_BYTES);
}
