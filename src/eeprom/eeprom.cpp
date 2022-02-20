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

    bool result = detectRadioType();
    if (!result)
        qWarning() << "Unable to detect radio type";

    QMetaObject::invokeMethod(this, "dataUpdated", Qt::QueuedConnection, Q_ARG(QByteArray, data));

    return result;
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

    QMetaObject::invokeMethod(this, "dataUpdated", Qt::QueuedConnection, Q_ARG(QByteArray, data));
}

bool EEPROM::getAdd300Mhz() const {
    return add300Mhz;
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

    int offset = firstAffectedByte(Param::PARAM_FREQ_RX, channel);

    quint16 rxFreqBits = static_cast<quint8>(data[offset]) << 8 | static_cast<quint8>(data[offset + 1]);
    return wordToFrequency(rxFreqBits);
}

void EEPROM::setChannelRxFreq(Channel channel, Frequency freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_FREQ_RX, channel);

    auto rxValue = frequencyToWord(freq);
    assign(offset + 0, static_cast<quint8>(rxValue >> 8));
    assign(offset + 1, static_cast<quint8>(rxValue & 0xff));
}

Frequency EEPROM::getChannelTxFreq(Channel channel) {
    if (!isValidChannelNumber(channel))
        return wordToFrequency(0);

    int offset = firstAffectedByte(Param::PARAM_FREQ_TX, channel);

    quint16 rxFreqBits = static_cast<quint8>(data[offset]) << 8 | static_cast<quint8>(data[offset + 1]);
    return wordToFrequency(rxFreqBits);
}

void EEPROM::setChannelTxFreq(Channel channel, Frequency freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_FREQ_TX, channel);

    auto txValue = frequencyToWord(freq);
    assign(offset, static_cast<quint8>(txValue >> 8));
    assign(offset + 1, static_cast<quint8>(txValue & 0xff));
}

CTCSS EEPROM::getChannelRxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return CTCSS::TONE_OFF;

    int offset = firstAffectedByte(Param::PARAM_CTCSS_RX, channel);

    auto byte = static_cast<quint8>(data[offset]);
    return static_cast<CTCSS>(byte);
}

void EEPROM::setChannelRxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_CTCSS_RX, channel);
    assign(offset, static_cast<quint8>(ctcss));
}

CTCSS EEPROM::getChannelTxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return CTCSS::TONE_OFF;

    int offset = firstAffectedByte(Param::PARAM_CTCSS_TX, channel);

    auto byte = static_cast<quint8>(data[offset]);
    return static_cast<CTCSS>(byte);
}

void EEPROM::setChannelTxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_CTCSS_TX, channel);
    assign(offset, static_cast<quint8>(ctcss));
}

Power EEPROM::getChannelPower(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Power::DISABLED;

    int offset = firstAffectedByte(Param::PARAM_POWER, channel);

    auto txPowerByte = static_cast<quint8>(data[offset]);
    auto value = static_cast<quint8>((txPowerByte & 0b00111000) >> 3);

    return static_cast<Power>(value);
}

void EEPROM::setChannelPower(Channel channel, Power power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_POWER, channel);

    auto byte = static_cast<quint8>(data[offset]);

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

    assign(offset, byte);
}

Squelch EEPROM::getChannelSquelch(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Squelch::OPEN;

    int offset = firstAffectedByte(Param::PARAM_SQUELCH, channel);

    auto squelchByte = static_cast<quint8>(data[offset]);
    int extractValue = squelchByte & 0b00011100;
    int shiftedValue = extractValue >> 2;
    auto value = static_cast<unsigned int>(shiftedValue);

    return static_cast<Squelch>(value);
}

void EEPROM::setChannelSquelch(Channel channel, Squelch squelch) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_SQUELCH, channel);

    auto byte = static_cast<quint8>(data[offset]);

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

    assign(offset, byte);
}

Flag EEPROM::getChannelSelectiveCalling(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Flag::DISABLED;

    int offset = firstAffectedByte(Param::PARAM_SELECTIVE_CALLING, channel);

    auto configBit = static_cast<quint8>(data[offset]);
    auto value = static_cast<bool>(configBit & 0b00000010);
    return static_cast<Flag>(value);
}

void EEPROM::setChannelSelectiveCalling(Channel channel, Flag selectiveCalling) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_SELECTIVE_CALLING, channel);

    auto byte = static_cast<quint8>(data[offset]);
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

    assign(offset, byte);
}

Flag EEPROM::getChannelCpuOffset(Channel channel) {
    if (!isValidChannelNumber(channel))
        return Flag::DISABLED;

    int offset = firstAffectedByte(Param::PARAM_CPU_OFFSET, channel);

    auto configBit = static_cast<quint8>(data[offset]);
    auto value = static_cast<bool>(configBit & 0b00000001);
    return static_cast<Flag>(value);
}

void EEPROM::setChannelCpuOffset(Channel channel, Flag cpuOffset) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = firstAffectedByte(Param::PARAM_CPU_OFFSET, channel);

    auto byte = static_cast<quint8>(data[offset]);
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

    assign(offset, byte);
}

Channel EEPROM::getStartupChannel() {
    int offset = firstAffectedByte(Param::PARAM_STARTUP_CHANNEL);
    auto value = static_cast<quint8>(data[offset]);
    return static_cast<Channel>(value < CHANNELS_COUNT ? value : (Channel) 0xff);
}

void EEPROM::setStartupChannel(Channel startupChannel) {
    if (!isValidChannelNumber(startupChannel) && startupChannel != static_cast<Channel>(0xff))
        return;

    auto byte = static_cast<quint8>(startupChannel);
    int offset = firstAffectedByte(Param::PARAM_STARTUP_CHANNEL);
    assign(offset, byte);
}

Flag EEPROM::getKeyBeep() {
    int offset = firstAffectedByte(Param::PARAM_KEY_BEEP);
    auto value = static_cast<quint8>(data[offset]);

    if (value == 0)
        return Flag::DISABLED;
    else
        return Flag::ENABLED;
}

void EEPROM::setKeyBeep(Flag keyBeep) {
    quint8 value = 0x00;

    if (keyBeep == Flag::ENABLED)
        value = 0x01;

    int offset = firstAffectedByte(Param::PARAM_KEY_BEEP);
    assign(offset, value);
}

TOT EEPROM::getTot() {
    int offset = firstAffectedByte(Param::PARAM_TOT);
    auto value = static_cast<quint8>(data[offset]);
    return static_cast<TOT>(value);
}

void EEPROM::setTot(TOT tot) {
    auto value = static_cast<quint8>(tot);
    int offset = firstAffectedByte(Param::PARAM_TOT);
    assign(offset, value);
}

Power EEPROM::getLowPower() {
    int offset = firstAffectedByte(Param::PARAM_LOW_POWER);
    auto value = static_cast<quint8>(data[offset]);
    return static_cast<Power>(value);
}

void EEPROM::setLowPower(Power power) {
    auto value = static_cast<quint8>(power);
    int offset = firstAffectedByte(Param::PARAM_LOW_POWER);
    assign(offset, value);
}

int EEPROM::firstAffectedByte(Param param, Channel channel) const {
    int offset = firstChannelOffset + (static_cast<quint8>(channel) * 8);

    switch (param) {
        case Param::PARAM_FREQ_RX:
            return offset;

        case Param::PARAM_FREQ_TX:
            return offset + 2;

        case Param::PARAM_CTCSS_RX:
            return offset + 4;

        case Param::PARAM_CTCSS_TX:
            return offset + 5;

        case Param::PARAM_POWER:
            return offset + 6;

        case Param::PARAM_SQUELCH:
        case Param::PARAM_SELECTIVE_CALLING:
        case Param::PARAM_CPU_OFFSET:
            return offset + 7;

        case Param::PARAM_STARTUP_CHANNEL:
            return OFFSET_STARTUP_CHANNEL;

        case Param::PARAM_KEY_BEEP:
            return OFFSET_KEY_BEEP;

        case Param::PARAM_TOT:
            return OFFSET_TOT;

        case Param::PARAM_LOW_POWER:
            return OFFSET_LOW_POWER;

        default:
            return -1;
    }
}

int EEPROM::byteSize(Param param) {
    switch (param) {
        case Param::PARAM_FREQ_RX:
        case Param::PARAM_FREQ_TX:
            return 2;

        case Param::PARAM_CTCSS_RX:
        case Param::PARAM_CTCSS_TX:
        case Param::PARAM_POWER:
        case Param::PARAM_SQUELCH:
        case Param::PARAM_SELECTIVE_CALLING:
        case Param::PARAM_CPU_OFFSET:
        case Param::PARAM_STARTUP_CHANNEL:
        case Param::PARAM_KEY_BEEP:
        case Param::PARAM_TOT:
        case Param::PARAM_LOW_POWER:
            return 1;

        default:
            return -1;
    }
}

bool EEPROM::detectRadioType() {
    firstChannelOffset = OFFSET_CHANNEL_FIRST_ALTERNATIVE;
    add300Mhz = true;
    Frequency frequency = getChannelRxFreq(0);
    if (frequency >= 400000000 && frequency <= 480000000) {
        frequencyBand = FrequencyBand::U0;
        return true;
    }

    firstChannelOffset = OFFSET_CHANNEL_FIRST;
    add300Mhz = true;
    frequency = getChannelRxFreq(0);
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

void EEPROM::registerMetaTypes() {
    qRegisterMetaType<qfm1000::eeprom::FrequencyBand>("qfm1000::eeprom::FrequencyBand");
    qRegisterMetaType<qfm1000::eeprom::Flag>("qfm1000::eeprom::Flag");
    qRegisterMetaType<qfm1000::eeprom::CTCSS>("qfm1000::eeprom::CTCSS");
    qRegisterMetaType<qfm1000::eeprom::Power>("qfm1000::eeprom::Power");
    qRegisterMetaType<qfm1000::eeprom::Squelch>("qfm1000::eeprom::Squelch");
    qRegisterMetaType<qfm1000::eeprom::Param>("qfm1000::eeprom::Param");
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const FrequencyBand &frequencyBand) {
    QDebugStateSaver saver(debug);

    switch (frequencyBand) {

        case FrequencyBand::E0:
            debug.nospace() << "E0";
            break;
        case FrequencyBand::B0:
            debug.nospace() << "B0";
            break;
        case FrequencyBand::A9:
            debug.nospace() << "A9";
            break;
        case FrequencyBand::K1:
            debug.nospace() << "K1";
            break;
        case FrequencyBand::K2:
            debug.nospace() << "K2";
            break;
        case FrequencyBand::K8:
            debug.nospace() << "K8";
            break;
        case FrequencyBand::K9:
            debug.nospace() << "K9";
            break;
        case FrequencyBand::TD:
            debug.nospace() << "TD";
            break;
        case FrequencyBand::TM:
            debug.nospace() << "TM";
            break;
        case FrequencyBand::TZ:
            debug.nospace() << "TZ";
            break;
        case FrequencyBand::T4:
            debug.nospace() << "T4";
            break;
        case FrequencyBand::U0:
            debug.nospace() << "U0";
            break;
        case FrequencyBand::W1:
            debug.nospace() << "W1";
            break;
        case FrequencyBand::W4:
            debug.nospace() << "W4";
            break;
    }

    return debug;
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const Flag &flag) {
    QDebugStateSaver saver(debug);

    switch (flag) {

        case Flag::ENABLED:
            debug.nospace() << "enabled";
            break;
        case Flag::DISABLED:
            debug.nospace() << "disabled";
            break;
    }

    return debug;
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const CTCSS &ctcss) {
    QDebugStateSaver saver(debug);

    switch (ctcss) {

        case CTCSS::TONE_OFF:
            debug.nospace() << "OFF";
            break;
        case CTCSS::TONE_67_0:
            debug.nospace() << "67.0 Hz";
            break;
        case CTCSS::TONE_71_9:
            debug.nospace() << "71.9 Hz";
            break;
        case CTCSS::TONE_74_4:
            debug.nospace() << "74.4 Hz";
            break;
        case CTCSS::TONE_77_0:
            debug.nospace() << "77.0 Hz";
            break;
        case CTCSS::TONE_79_7:
            debug.nospace() << "79.7 Hz";
            break;
        case CTCSS::TONE_82_5:
            debug.nospace() << "82.5 Hz";
            break;
        case CTCSS::TONE_85_4:
            debug.nospace() << "85.4 Hz";
            break;
        case CTCSS::TONE_88_5:
            debug.nospace() << "88.5 Hz";
            break;
        case CTCSS::TONE_91_5:
            debug.nospace() << "91.5 Hz";
            break;
        case CTCSS::TONE_94_8:
            debug.nospace() << "94.8 Hz";
            break;
        case CTCSS::TONE_97_4:
            debug.nospace() << "97.4 Hz";
            break;
        case CTCSS::TONE_100_0:
            debug.nospace() << "100.0 Hz";
            break;
        case CTCSS::TONE_103_5:
            debug.nospace() << "103.5 Hz";
            break;
        case CTCSS::TONE_107_2:
            debug.nospace() << "107.2 Hz";
            break;
        case CTCSS::TONE_110_9:
            debug.nospace() << "110.9 Hz";
            break;
        case CTCSS::TONE_114_8:
            debug.nospace() << "114.8 Hz";
            break;
        case CTCSS::TONE_118_8:
            debug.nospace() << "118.8 Hz";
            break;
        case CTCSS::TONE_123_0:
            debug.nospace() << "123.0 Hz";
            break;
        case CTCSS::TONE_127_3:
            debug.nospace() << "127.3 Hz";
            break;
        case CTCSS::TONE_131_8:
            debug.nospace() << "131.8 Hz";
            break;
        case CTCSS::TONE_136_5:
            debug.nospace() << "136.5 Hz";
            break;
        case CTCSS::TONE_141_3:
            debug.nospace() << "141.3 Hz";
            break;
        case CTCSS::TONE_146_2:
            debug.nospace() << "146.2 Hz";
            break;
        case CTCSS::TONE_151_4:
            debug.nospace() << "151.4 Hz";
            break;
        case CTCSS::TONE_156_7:
            debug.nospace() << "156.7 Hz";
            break;
        case CTCSS::TONE_162_2:
            debug.nospace() << "162.2 Hz";
            break;
        case CTCSS::TONE_167_9:
            debug.nospace() << "167.9 Hz";
            break;
        case CTCSS::TONE_173_8:
            debug.nospace() << "173.8 Hz";
            break;
        case CTCSS::TONE_179_9:
            debug.nospace() << "179.9 Hz";
            break;
        case CTCSS::TONE_186_2:
            debug.nospace() << "186.2 Hz";
            break;
        case CTCSS::TONE_192_8:
            debug.nospace() << "192.8 Hz";
            break;
        case CTCSS::TONE_203_5:
            debug.nospace() << "203.5 Hz";
            break;
        case CTCSS::TONE_210_7:
            debug.nospace() << "210.7 Hz";
            break;
        case CTCSS::TONE_218_1:
            debug.nospace() << "218.1 Hz";
            break;
        case CTCSS::TONE_225_7:
            debug.nospace() << "225.7 Hz";
            break;
        case CTCSS::TONE_233_6:
            debug.nospace() << "233.6 Hz";
            break;
        case CTCSS::TONE_241_8:
            debug.nospace() << "241.8 Hz";
            break;
        case CTCSS::TONE_250_3:
            debug.nospace() << "250.3 Hz";
            break;
    }

    return debug;
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const Power &power) {
    QDebugStateSaver saver(debug);

    switch (power) {

        case Power::DISABLED:
            debug.nospace() << "disabled";
            break;
        case Power::WATT_1:
            debug.nospace() << "1W";
            break;
        case Power::WATT_6:
            debug.nospace() << "6W";
            break;
        case Power::WATT_10:
            debug.nospace() << "10W";
            break;
        case Power::WATT_15:
            debug.nospace() << "15W";
            break;
        case Power::WATT_25:
            debug.nospace() << "25W";
            break;
    }

    return debug;
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const Squelch &squelch) {
    QDebugStateSaver saver(debug);

    switch (squelch) {

        case Squelch::OPEN:
            debug.nospace() << "open";
            break;
        case Squelch::SINAD_9DB:
            debug.nospace() << "9dB SINAD";
            break;
        case Squelch::SINAD_12DB:
            debug.nospace() << "12dB SINAD";
            break;
        case Squelch::SINAD_15DB:
            debug.nospace() << "15dB SINAD";
            break;
        case Squelch::SINAD_18DB:
            debug.nospace() << "18dB SINAD";
            break;
        case Squelch::SINAD_21DB:
            debug.nospace() << "21dB SINAD";
            break;
        case Squelch::SINAD_24DB:
            debug.nospace() << "24dB SINAD";
            break;
    }

    return debug;
}

QDebug qfm1000::eeprom::operator<<(QDebug debug, const Param &param) {
    QDebugStateSaver saver(debug);

    switch (param) {

        case Param::NONE:
            debug.nospace() << "none";
            break;
        case Param::PARAM_FREQ_RX:
            debug.nospace() << "RX Freq";
            break;
        case Param::PARAM_FREQ_TX:
            debug.nospace() << "TX Freq";
            break;
        case Param::PARAM_CTCSS_RX:
            debug.nospace() << "RX CTCSS";
            break;
        case Param::PARAM_CTCSS_TX:
            debug.nospace() << "TX CTCSS";
            break;
        case Param::PARAM_POWER:
            debug.nospace() << "Power";
            break;
        case Param::PARAM_SQUELCH:
            debug.nospace() << "Squelch";
            break;
        case Param::PARAM_SELECTIVE_CALLING:
            debug.nospace() << "Selective Calling";
            break;
        case Param::PARAM_CPU_OFFSET:
            debug.nospace() << "CPU Offset";
            break;
        case Param::PARAM_STARTUP_CHANNEL:
            debug.nospace() << "Startup channel";
            break;
        case Param::PARAM_KEY_BEEP:
            debug.nospace() << "Key beep";
            break;
        case Param::PARAM_TOT:
            debug.nospace() << "TOT";
            break;
        case Param::PARAM_LOW_POWER:
            debug.nospace() << "Low Power";
            break;
    }

    return debug;
}
