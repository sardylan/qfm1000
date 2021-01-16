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

#define TEST_POWER_DISABLED_BYTES '\x00'
#define TEST_POWER_WATT_1_BYTES '\x01'
#define TEST_POWER_WATT_6_BYTES '\x02'
#define TEST_POWER_WATT_10_BYTES '\x03'
#define TEST_POWER_WATT_15_BYTES '\x04'
#define TEST_POWER_WATT_25_BYTES '\x05'

#define TEST_SQUELCH_OPEN_BYTES '\x00'
#define TEST_SQUELCH_SINAD_9DB_BYTES '\x01'
#define TEST_SQUELCH_SINAD_12DB_BYTES '\x02'
#define TEST_SQUELCH_SINAD_15DB_BYTES '\x03'
#define TEST_SQUELCH_SINAD_18DB_BYTES '\x04'
#define TEST_SQUELCH_SINAD_21DB_BYTES '\x05'
#define TEST_SQUELCH_SINAD_24DB_BYTES '\x06'

#define TEST_FLAG_DISABLED '\x00'
#define TEST_FLAG_ENABLED '\x01'

#define TEST_STARTUP_CHANNEL static_cast<Channel>(10)
#define TEST_STARTUP_CHANNEL_BYTES '\x0a'

#define TEST_KEY_BEEP Flag::ENABLED
#define TEST_KEY_BEEP_BYTES '\x01'

#define TEST_TOT static_cast<TOT>(10)
#define TEST_TOT_BYTES '\x0a'

#define TEST_LOW_POWER Power::WATT_1
#define TEST_LOW_POWER_BYTES '\x01'

[[maybe_unused]] void EEPROMTest::initTestCase() {
    eeprom = new EEPROM();

    signalSpy = new QSignalSpy(eeprom, &EEPROM::byteUpdated);
    QVERIFY(signalSpy->isValid());
}

[[maybe_unused]] void EEPROMTest::cleanupTestCase() {
    delete signalSpy;
    delete eeprom;
}

[[maybe_unused]] void EEPROMTest::init() {
    QCOMPARE(eeprom->getData().size(), EEPROM_SIZE);
}

[[maybe_unused]] void EEPROMTest::cleanup() {
    eeprom->clear();
    signalSpy->clear();
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
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxFreq(i), static_cast<Frequency>(0));

        eeprom->setChannelRxFreq(i, TEST_FREQ);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 2);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelRxFreq(i), TEST_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::channelRxFreqBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 0], '\x00');
        QCOMPARE(eeprom->getData()[offset + 1], '\x00');

        eeprom->setChannelRxFreq(i, TEST_FREQ);
        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 2);

        QCOMPARE(eeprom->getData()[offset + 0], TEST_FREQ_BYTES_MSB);
        QCOMPARE(eeprom->getData()[offset + 1], TEST_FREQ_BYTES_LSB);

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 0);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_MSB);

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 1);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_LSB);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelTxFreq() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxFreq(i), static_cast<Frequency>(0));

        eeprom->setChannelTxFreq(i, TEST_FREQ);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 2);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelTxFreq(i), TEST_FREQ);
    }
}

[[maybe_unused]] void EEPROMTest::channelTxFreqBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 2], '\x00');
        QCOMPARE(eeprom->getData()[offset + 3], '\x00');

        eeprom->setChannelTxFreq(i, TEST_FREQ);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 2);

        QCOMPARE(eeprom->getData()[offset + 2], TEST_FREQ_BYTES_MSB);
        QCOMPARE(eeprom->getData()[offset + 3], TEST_FREQ_BYTES_LSB);

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 2);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_MSB);

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 3);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_FREQ_BYTES_LSB);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelRxCtcss() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelRxCtcss(i), CTCSS::TONE_OFF);

        eeprom->setChannelRxCtcss(i, TEST_CTCSS);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelRxCtcss(i), TEST_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::channelRxCtcssBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 4], '\x00');

        eeprom->setChannelRxCtcss(i, TEST_CTCSS);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(eeprom->getData()[offset + 4], TEST_CTCSS_BYTES);

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 4);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_CTCSS_BYTES);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelTxCtcss() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelTxCtcss(i), CTCSS::TONE_OFF);

        eeprom->setChannelTxCtcss(i, TEST_CTCSS);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelTxCtcss(i), TEST_CTCSS);
    }
}

[[maybe_unused]] void EEPROMTest::channelTxCtcssBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(eeprom->getData()[offset + 5], '\x00');

        eeprom->setChannelTxCtcss(i, TEST_CTCSS);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(eeprom->getData()[offset + 5], TEST_CTCSS_BYTES);

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 5);
        QCOMPARE(signalArguments[1].toChar().toLatin1(), TEST_CTCSS_BYTES);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelPower() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelPower(i), Power::DISABLED);

        eeprom->setChannelPower(i, Power::WATT_1);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_1);

        eeprom->setChannelPower(i, Power::WATT_6);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_6);

        eeprom->setChannelPower(i, Power::WATT_10);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_10);

        eeprom->setChannelPower(i, Power::WATT_15);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_15);

        eeprom->setChannelPower(i, Power::WATT_25);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelPower(i), Power::WATT_25);
    }
}

[[maybe_unused]] void EEPROMTest::channelPowerBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::DISABLED));

        eeprom->setChannelPower(i, Power::WATT_1);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_1));

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 6);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00111000) >> 3, TEST_POWER_WATT_1_BYTES);

        signalSpy->clear();

        eeprom->setChannelPower(i, Power::WATT_6);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_6));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 6);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00111000) >> 3, TEST_POWER_WATT_6_BYTES);

        signalSpy->clear();

        eeprom->setChannelPower(i, Power::WATT_10);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_10));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 6);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00111000) >> 3, TEST_POWER_WATT_10_BYTES);

        signalSpy->clear();

        eeprom->setChannelPower(i, Power::WATT_15);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_15));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 6);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00111000) >> 3, TEST_POWER_WATT_15_BYTES);

        signalSpy->clear();

        eeprom->setChannelPower(i, Power::WATT_25);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 6] & 0b00111000) >> 3),
                 static_cast<quint8>(Power::WATT_25));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 6);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00111000) >> 3, TEST_POWER_WATT_25_BYTES);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelSquelch() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::OPEN);

        eeprom->setChannelSquelch(i, Squelch::SINAD_9DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_9DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_12DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_12DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_15DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_15DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_18DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_18DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_21DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_21DB);

        eeprom->setChannelSquelch(i, Squelch::SINAD_24DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSquelch(i), Squelch::SINAD_24DB);
    }
}

[[maybe_unused]] void EEPROMTest::channelSquelchBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::OPEN));

        eeprom->setChannelSquelch(i, Squelch::SINAD_9DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_9DB));

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_9DB_BYTES);

        signalSpy->clear();

        eeprom->setChannelSquelch(i, Squelch::SINAD_12DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_12DB));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_12DB_BYTES);

        signalSpy->clear();

        eeprom->setChannelSquelch(i, Squelch::SINAD_15DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_15DB));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_15DB_BYTES);

        signalSpy->clear();

        eeprom->setChannelSquelch(i, Squelch::SINAD_18DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_18DB));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_18DB_BYTES);

        signalSpy->clear();

        eeprom->setChannelSquelch(i, Squelch::SINAD_21DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_21DB));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_21DB_BYTES);

        signalSpy->clear();

        eeprom->setChannelSquelch(i, Squelch::SINAD_24DB);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<quint8>((eeprom->getData()[offset + 7] & 0b00011100) >> 2),
                 static_cast<quint8>(Squelch::SINAD_24DB));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00011100) >> 2, TEST_SQUELCH_SINAD_24DB_BYTES);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelSelectiveCalling() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::DISABLED);

        eeprom->setChannelSelectiveCalling(i, Flag::ENABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::ENABLED);

        eeprom->setChannelSelectiveCalling(i, Flag::DISABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelSelectiveCalling(i), Flag::DISABLED);
    }
}

[[maybe_unused]] void EEPROMTest::channelSelectiveCallingBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::DISABLED));

        eeprom->setChannelSelectiveCalling(i, Flag::ENABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::ENABLED));

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00000010) >> 1, TEST_FLAG_ENABLED);

        signalSpy->clear();

        eeprom->setChannelSelectiveCalling(i, Flag::DISABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<bool>((eeprom->getData()[offset + 7] & 0b00000010) >> 1),
                 static_cast<bool>(Flag::DISABLED));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE((signalArguments[1].toChar().toLatin1() & 0b00000010) >> 1, TEST_FLAG_DISABLED);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::channelCpuOffset() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::DISABLED);

        eeprom->setChannelCpuOffset(i, Flag::ENABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::ENABLED);

        eeprom->setChannelCpuOffset(i, Flag::DISABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);
        signalSpy->clear();

        QCOMPARE(eeprom->getChannelCpuOffset(i), Flag::DISABLED);
    }
}

[[maybe_unused]] void EEPROMTest::channelCpuOffsetBytes() {
    for (Channel i = 0; i < CHANNELS_COUNT; i++) {
        int offset = eeprom->computeOffset(i);

        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::DISABLED));

        eeprom->setChannelCpuOffset(i, Flag::ENABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::ENABLED));

        QList<QVariant> signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_ENABLED);

        signalSpy->clear();

        eeprom->setChannelCpuOffset(i, Flag::DISABLED);

        QVERIFY(signalSpy->wait());
        QCOMPARE(signalSpy->size(), 1);

        QCOMPARE(static_cast<bool>(eeprom->getData()[offset + 7] & 0b00000001),
                 static_cast<bool>(Flag::DISABLED));

        signalArguments = signalSpy->takeFirst();
        QCOMPARE(signalArguments[0].toInt(), offset + 7);
        QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_DISABLED);

        signalSpy->clear();
    }
}

[[maybe_unused]] void EEPROMTest::startupChannel() {
    QCOMPARE(eeprom->getStartupChannel(), static_cast<Channel>(0));

    eeprom->setStartupChannel(TEST_STARTUP_CHANNEL);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);
    signalSpy->clear();

    QCOMPARE(eeprom->getStartupChannel(), TEST_STARTUP_CHANNEL);
}

[[maybe_unused]] void EEPROMTest::startupChannelBytes() {
    QCOMPARE(eeprom->getData()[OFFSET_STARTUP_CHANNEL], '\x00');

    eeprom->setStartupChannel(TEST_STARTUP_CHANNEL);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);

    QCOMPARE(eeprom->getData()[OFFSET_STARTUP_CHANNEL], TEST_STARTUP_CHANNEL_BYTES);

    QList<QVariant> signalArguments = signalSpy->takeFirst();
    QCOMPARE(signalArguments[0].toInt(), OFFSET_STARTUP_CHANNEL);
    QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_DISABLED);

    signalSpy->clear();
}

[[maybe_unused]] void EEPROMTest::keyBeep() {
    QCOMPARE(eeprom->getKeyBeep(), Flag::DISABLED);

    eeprom->setKeyBeep(TEST_KEY_BEEP);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);
    signalSpy->clear();

    QCOMPARE(eeprom->getKeyBeep(), TEST_KEY_BEEP);
}

[[maybe_unused]] void EEPROMTest::keyBeepBytes() {
    QCOMPARE(eeprom->getData()[OFFSET_KEY_BEEP], '\x00');

    eeprom->setKeyBeep(TEST_KEY_BEEP);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);

    QCOMPARE(eeprom->getData()[OFFSET_KEY_BEEP], TEST_KEY_BEEP_BYTES);

    QList<QVariant> signalArguments = signalSpy->takeFirst();
    QCOMPARE(signalArguments[0].toInt(), OFFSET_KEY_BEEP);
    QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_ENABLED);

    signalSpy->clear();
}

[[maybe_unused]] void EEPROMTest::tot() {
    QCOMPARE(eeprom->getTot(), static_cast<TOT>(0));

    eeprom->setTot(TEST_TOT);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);
    signalSpy->clear();

    QCOMPARE(eeprom->getTot(), TEST_TOT);
}

[[maybe_unused]] void EEPROMTest::totBytes() {
    QCOMPARE(eeprom->getData()[OFFSET_TOT], '\x00');

    eeprom->setTot(TEST_TOT);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);

    QCOMPARE(eeprom->getData()[OFFSET_TOT], TEST_TOT_BYTES);

    QList<QVariant> signalArguments = signalSpy->takeFirst();
    QCOMPARE(signalArguments[0].toInt(), OFFSET_TOT);
    QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_DISABLED);

    signalSpy->clear();
}

[[maybe_unused]] void EEPROMTest::lowPower() {
    QCOMPARE(eeprom->getLowPower(), Power::DISABLED);

    eeprom->setLowPower(TEST_LOW_POWER);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);
    signalSpy->clear();

    QCOMPARE(eeprom->getLowPower(), TEST_LOW_POWER);
}

[[maybe_unused]] void EEPROMTest::lowPowerBytes() {
    QCOMPARE(eeprom->getData()[OFFSET_LOW_POWER], '\x00');

    eeprom->setLowPower(TEST_LOW_POWER);

    QVERIFY(signalSpy->wait());
    QCOMPARE(signalSpy->size(), 1);

    QCOMPARE(eeprom->getData()[OFFSET_LOW_POWER], TEST_LOW_POWER_BYTES);

    QList<QVariant> signalArguments = signalSpy->takeFirst();
    QCOMPARE(signalArguments[0].toInt(), OFFSET_LOW_POWER);
    QCOMPARE(signalArguments[1].toChar().toLatin1() & 0b00000001, TEST_FLAG_ENABLED);

    signalSpy->clear();
}
