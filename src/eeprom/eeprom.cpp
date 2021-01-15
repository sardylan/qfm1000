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


#include <QtCore/QDebug>

#include "eeprom.hpp"

using namespace qfm1000::eeprom;

EEPROM::EEPROM(QObject *parent) : QObject(parent), data(EEPROM_SIZE, '\0') {
    frequencyBand = FrequencyBand::A9;
    add300Mhz = false;
    firstChannelOffset = OFFSET_CHANNEL_FIRST;
}

EEPROM::~EEPROM() = default;

void EEPROM::clear() {
    for (int i = 0; i < EEPROM_SIZE; i++)
        data[i] = '\0';
}

const QByteArray &EEPROM::getData() {
    return data;
}

bool EEPROM::setData(const QByteArray &newValue) {
    if (newValue.size() != EEPROM_SIZE) {
        qCritical() << "Wrong Data size" << "(" << newValue.size() << "instead" << EEPROM_SIZE << ")";
        return false;
    }

    EEPROM::data = newValue;

    return detectRadioType();
}

FrequencyBand EEPROM::getFrequencyBand() const {
    return frequencyBand;
}

void EEPROM::setFrequencyBand(FrequencyBand newValue) {
    EEPROM::frequencyBand = newValue;

    switch (EEPROM::frequencyBand) {
        case FrequencyBand::E0:
        case FrequencyBand::B0:
        case FrequencyBand::A9:
        case FrequencyBand::K1:
        case FrequencyBand::K2:
        case FrequencyBand::K8:
        case FrequencyBand::K9:
            add300Mhz = false;
            break;

        case FrequencyBand::TD:
        case FrequencyBand::TM:
        case FrequencyBand::TZ:
        case FrequencyBand::T4:
        case FrequencyBand::U0:
        case FrequencyBand::W1:
        case FrequencyBand::W4:
            add300Mhz = true;
            break;
    }
}

int EEPROM::getFirstChannelOffset() const {
    return firstChannelOffset;
}

void EEPROM::setFirstChannelOffset(int newValue) {
    EEPROM::firstChannelOffset = newValue;
}

Frequency EEPROM::getChannelRxFreq(Channel channel) {
    if (!isValidChannelNumber(channel))
        return wordToFrequency(0);

    int offset = computeOffset(channel);

    quint16 rxFreqBits = static_cast<quint8>(data[offset]) << 8 | static_cast<quint8>(data[offset + 1]);
    return wordToFrequency(rxFreqBits);
}

void EEPROM::setChannelRxFreq(Channel channel, Frequency freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto rxValue = frequencyToWord(freq);
    assign(offset + 0, static_cast<quint8>(rxValue >> 8));
    assign(offset + 1, static_cast<quint8>(rxValue & 0xff));
}

Frequency EEPROM::getChannelTxFreq(Channel channel) {
    if (!isValidChannelNumber(channel))
        return wordToFrequency(0);

    int offset = computeOffset(channel);

    quint16 rxFreqBits = static_cast<quint8>(data[offset + 2]) << 8 | static_cast<quint8>(data[offset + 3]);
    return wordToFrequency(rxFreqBits);
}

void EEPROM::setChannelTxFreq(Channel channel, Frequency freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto txValue = frequencyToWord(freq);
    assign(offset + 2, static_cast<quint8>(txValue >> 8));
    assign(offset + 3, static_cast<quint8>(txValue & 0xff));
}

CTCSS EEPROM::getChannelRxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return CTCSS::TONE_OFF;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 4]);
    return static_cast<CTCSS>(byte);
}

void EEPROM::setChannelRxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    assign(offset + 4, static_cast<quint8>(ctcss));
}

CTCSS EEPROM::getChannelTxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return CTCSS::TONE_OFF;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 5]);
    return static_cast<CTCSS>(byte);
}

void EEPROM::setChannelTxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    assign(offset + 5, static_cast<quint8>(ctcss));
}

Power EEPROM::getChannelPower(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Power::DISABLED;

    int offset = computeOffset(channel);

    auto txPowerByte = static_cast<quint8>(data[offset + 6]);
    auto value = static_cast<quint8>((txPowerByte & 0b00111000) >> 3);

    return static_cast<Power>(value);
}

void EEPROM::setChannelPower(Channel channel, Power power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 6]);

    switch (power) {

        case Power::WATT_25:  // 0b11101000 - 0xe8
            byte &= 0b11000111;
            byte |= 0b00101000;
            break;

        case Power::WATT_15:  // 0b11100000 - 0xe0
            byte &= 0b11000111;
            byte |= 0b00100000;
            break;

        case Power::WATT_10:  // 0b11011000 - 0xd8
            byte &= 0b11000111;
            byte |= 0b00011000;
            break;

        case Power::WATT_6:   // 0b11010000 - 0xd0
            byte &= 0b11000111;
            byte |= 0b00010000;
            break;

        case Power::WATT_1:   // 0b11001000 - 0xc8
            byte &= 0b11000111;
            byte |= 0b00001000;
            break;

        case Power::DISABLED:
        default:              // 0b11000000 - 0xc0
            byte &= 0b11000111;
    }

    assign(offset + 6, byte);
}

Squelch EEPROM::getChannelSquelch(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Squelch::OPEN;

    int offset = computeOffset(channel);

    auto squelchByte = static_cast<quint8>(data[offset + 7]);
    int extractValue = squelchByte & 0b00011100;
    int shiftedValue = extractValue >> 2;
    auto value = static_cast<unsigned int>(shiftedValue);

    return static_cast<Squelch>(value);
}

void EEPROM::setChannelSquelch(Channel channel, Squelch squelch) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 7]);

    switch (squelch) {

        case Squelch::SINAD_24DB:
            byte &= 0b11100011;
            byte |= 0b00011000;
            break;

        case Squelch::SINAD_21DB:
            byte &= 0b11100011;
            byte |= 0b00010100;
            break;

        case Squelch::SINAD_18DB:
            byte &= 0b11100011;
            byte |= 0b00010000;
            break;

        case Squelch::SINAD_15DB:
            byte &= 0b11100011;
            byte |= 0b00001100;
            break;

        case Squelch::SINAD_12DB:
            byte &= 0b11100011;
            byte |= 0b00001000;
            break;

        case Squelch::SINAD_9DB:
            byte &= 0b11100011;
            byte |= 0b00000100;
            break;

        case Squelch::OPEN:
        default:
            byte &= 0b11100011;
    }

    assign(offset + 7, byte);
}

Flag EEPROM::getChannelSelectiveCalling(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Flag::DISABLED;

    int offset = computeOffset(channel);

    auto configBit = static_cast<quint8>(data[offset + 7]);
    auto value = static_cast<bool>(configBit & 0b00000010);
    return static_cast<Flag>(value);
}

void EEPROM::setChannelSelectiveCalling(Channel channel, Flag selectiveCalling) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 7]);
    byte &= 0b00000011;
    byte |= 0b00100100;

    switch (selectiveCalling) {

        case Flag::ENABLED:
            byte |= 0b00000010;
            break;

        case Flag::DISABLED:
        default:
            byte &= 0b11111101;
            break;
    }

    assign(offset + 7, byte);
}

Flag EEPROM::getChannelCpuOffset(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Flag::DISABLED;

    int offset = computeOffset(channel);

    auto configBit = static_cast<quint8>(data[offset + 7]);
    auto value = static_cast<bool>(configBit & 0b00000001);
    return static_cast<Flag>(value);
}

void EEPROM::setChannelCpuOffset(Channel channel, Flag cpuOffset) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = computeOffset(channel);

    auto byte = static_cast<quint8>(data[offset + 7]);
    byte &= 0b00000011;
    byte |= 0b00100100;

    switch (cpuOffset) {

        case Flag::ENABLED:
            byte |= 0b00000001;
            break;

        case Flag::DISABLED:
        default:
            byte &= 0b11111110;
            break;
    }

    assign(offset + 7, byte);
}

Channel EEPROM::getStartupChannel() {
    auto value = static_cast<quint8>(data[OFFSET_STARTUP_CHANNEL]);
    return static_cast<Channel>(value < CHANNELS_COUNT ? value : (Channel) 0xff);
}

void EEPROM::setStartupChannel(Channel startupChannel) {
    if (!isValidChannelNumber(startupChannel))
        return;

    auto byte = static_cast<quint8>(startupChannel < CHANNELS_COUNT ? startupChannel : (Channel) 0xff);
    assign(OFFSET_STARTUP_CHANNEL, byte);
}

Flag EEPROM::getKeyBeep() {
    auto value = static_cast<quint8>(data[OFFSET_KEY_BEEP]);

    if (value == 0)
        return Flag::DISABLED;
    else
        return Flag::ENABLED;
}

void EEPROM::setKeyBeep(Flag keyBeep) {
    quint8 value = 0x00;

    if (keyBeep == Flag::ENABLED)
        value = 0x01;

    assign(OFFSET_KEY_BEEP, value);
}

TOT EEPROM::getTot() {
    auto value = static_cast<quint8>(data[OFFSET_TOT]);
    return static_cast<TOT>(value);
}

void EEPROM::setTot(TOT tot) {
    auto value = static_cast<quint8>(tot);
    assign(OFFSET_TOT, value);
}

Power EEPROM::getLowPower() {
    auto value = static_cast<quint8>(data[OFFSET_LOW_POWER]);
    return static_cast<Power>(value);
}

void EEPROM::setLowPower(Power power) {
    auto value = static_cast<quint8>(power);
    assign(OFFSET_LOW_POWER, value);
}

int EEPROM::computeOffset(Channel channel) const {
    return firstChannelOffset + (static_cast<quint8>(channel) * 8);
}

bool EEPROM::detectRadioType() {
    firstChannelOffset = OFFSET_CHANNEL_FIRST;
    add300Mhz = true;
    Frequency frequency = getChannelRxFreq(0);
    if (frequency >= 400000000 && frequency <= 480000000) {
        frequencyBand = FrequencyBand::U0;
        return true;
    }

    firstChannelOffset = OFFSET_CHANNEL_FIRST_ALTERNATIVE;
    add300Mhz = false;
    frequency = getChannelRxFreq(0);
    if (frequency >= 120000000 && frequency <= 180000000) {
        frequencyBand = FrequencyBand::A9;
        return true;
    }

    firstChannelOffset = OFFSET_CHANNEL_FIRST;
    add300Mhz = false;
    frequency = getChannelRxFreq(0);
    if (frequency >= 120000000 && frequency <= 180000000) {
        frequencyBand = FrequencyBand::A9;
        return true;
    }

    return false;
}

void EEPROM::assign(int pos, quint8 value) {
    data.replace(pos, 1, (const char *) &value, 1);

    QMetaObject::invokeMethod(
            this,
            "byteUpdated",
            Qt::QueuedConnection,
            Q_ARG(int, pos),
            Q_ARG(quint8, value)
    );
}

Frequency EEPROM::wordToFrequency(quint16 word) const {
    if (add300Mhz)
        return static_cast<Frequency>((word + 48000) * 6250);
    else
        return static_cast<Frequency>(word * 6250);
}

quint16 EEPROM::frequencyToWord(Frequency frequency) const {
    if (add300Mhz)
        return static_cast<quint16>((frequency / 6250) - 48000);
    else
        return static_cast<quint16>(frequency / 6250);
}

bool EEPROM::isValidChannelNumber(Channel channel) {
    return channel < CHANNELS_COUNT;
}

void EEPROM::registerMetaType() {
    qRegisterMetaType<qfm1000::eeprom::FrequencyBand>("qfm1000::eeprom::FrequencyBand");
    qRegisterMetaType<qfm1000::eeprom::Flag>("qfm1000::eeprom::Flag");
    qRegisterMetaType<qfm1000::eeprom::CTCSS>("qfm1000::eeprom::CTCSS");
    qRegisterMetaType<qfm1000::eeprom::Power>("qfm1000::eeprom::Power");
    qRegisterMetaType<qfm1000::eeprom::Squelch>("qfm1000::eeprom::Squelch");
}
