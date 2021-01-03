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

EEPROM::EEPROM() : data(EEPROM_SIZE, '\0') {

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
    return true;
}

Frequency EEPROM::getChannelRxFreq(Channel channel, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((uint8_t) data[offset] << 8) | ((uint8_t) data[offset + 1]);
    return wordToFrequency(rxFreqBits, frequencyBand);
}

void EEPROM::setChannelRxFreq(Channel channel, Frequency freq, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto rxValue = frequencyToWord(freq, frequencyBand);
    assign(offset + 0, (uint8_t) (rxValue >> 8));
    assign(offset + 1, (uint8_t) (rxValue & 0xff));
}

Frequency EEPROM::getChannelTxFreq(Channel channel, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((uint8_t) data[offset + 2] << 8) | ((uint8_t) data[offset + 3]);
    return wordToFrequency(rxFreqBits, frequencyBand);
}

void EEPROM::setChannelTxFreq(Channel channel, Frequency freq, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txValue = frequencyToWord(freq, frequencyBand);
    assign(offset + 2, (uint8_t) (txValue >> 8));
    assign(offset + 3, (uint8_t) (txValue & 0xff));
}

uint8_t EEPROM::getChannelRxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    return (uint8_t) data[offset + 4];
}

void EEPROM::setChannelRxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 4, (uint8_t) ctcss);
}

uint8_t EEPROM::getChannelTxCtcss(Channel channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    char b = data[offset + 5];
    return (uint8_t) b;
}

void EEPROM::setChannelTxCtcss(Channel channel, CTCSS ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 5, (uint8_t) ctcss);
}

Power EEPROM::getChannelPower(Channel channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txPowerByte = (uint8_t) data[offset + 6];
    auto value = static_cast<unsigned int>((txPowerByte & 0b00111000) >> 3);

    return value;
}

void EEPROM::setChannelPower(Channel channel, Power power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (uint8_t) data[offset + 6];
    switch (power) {
        case 5:  // 0b11101000 - 0xe8
            byte &= 0b11000111;
            byte |= 0b00101000;
            break;
        case 4:  // 0b11100000 - 0xe0
            byte &= 0b11000111;
            byte |= 0b00100000;
            break;
        case 3:  // 0b11011000 - 0xd8
            byte &= 0b11000111;
            byte |= 0b00011000;
            break;
        case 2:  // 0b11010000 - 0xd0
            byte &= 0b11000111;
            byte |= 0b00010000;
            break;
        case 1:  // 0b11001000 - 0xc8
            byte &= 0b11000111;
            byte |= 0b00001000;
            break;
        case 0:
        default: // 0b11000000 - 0xc0
            byte &= 0b11000111;
    }

    assign(offset + 6, byte);
}

Power EEPROM::getChannelSquelch(Channel channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txPowerByte = (uint8_t) data[offset + 7];
    int extractValue = txPowerByte & 0b00011100;
    int shiftedValue = extractValue >> 2;
    auto value = static_cast<unsigned int>(shiftedValue);

    return value;
}

void EEPROM::setChannelSquelch(Channel channel, Power power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (uint8_t) data[offset + 7];
    switch (power) {
        case 6:
            byte &= 0b11100011;
            byte |= 0b00011000;
            break;
        case 5:
            byte &= 0b11100011;
            byte |= 0b00010100;
            break;
        case 4:
            byte &= 0b11100011;
            byte |= 0b00010000;
            break;
        case 3:
            byte &= 0b11100011;
            byte |= 0b00001100;
            break;
        case 2:
            byte &= 0b11100011;
            byte |= 0b00001000;
            break;
        case 1:
            byte &= 0b11100011;
            byte |= 0b00000100;
            break;
        case 0:
        default:
            byte &= 0b11100011;
    }

    assign(offset + 7, byte);
}

bool EEPROM::getChannelSelectiveCalling(Channel channel) {
    if (!isValidChannelNumber(channel))
        return false;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto configBit = (uint8_t) data[offset + 7];
    return (bool) (configBit & 0b00000010);
}

void EEPROM::setChannelSelectiveCalling(Channel channel, bool selectiveCalling) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (uint8_t) data[offset + 7];
    byte &= 0b00000011;
    byte |= 0b00100100;
    if (selectiveCalling)
        byte |= 0b00000010;
    else
        byte &= 0b11111101;
    assign(offset + 7, byte);
}

bool EEPROM::getChannelCpuOffset(Channel channel) {
    if (!isValidChannelNumber(channel))
        return false;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto configBit = (uint8_t) data[offset + 7];
    return (bool) (configBit & 0b00000001);
}

void EEPROM::setChannelCpuOffset(Channel channel, bool cpuOffset) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (uint8_t) data[offset + 7];
    byte &= 0b00000011;
    byte |= 0b00100100;
    if (cpuOffset)
        byte |= 0b00000001;
    else
        byte &= 0b11111110;
    assign(offset + 7, byte);
}

Channel EEPROM::getDefaultChannel() {
    return (int) data[OFFSET_STARTUP_CHANNEL] >= 0 && (int) data[OFFSET_STARTUP_CHANNEL] < CHANNELS_COUNT
           ? (int) data[OFFSET_STARTUP_CHANNEL]
           : -1;
}

void EEPROM::setDefaultChannel(Channel defaultChannel) {
    if (!isValidChannelNumber(defaultChannel))
        return;

    auto byte = (uint8_t) (defaultChannel >= 0 && defaultChannel < CHANNELS_COUNT
                           ? defaultChannel
                           : 0xff);
    assign(OFFSET_STARTUP_CHANNEL, byte);
}

TOT EEPROM::getTot() {
    return (uint8_t) data[OFFSET_TOT];
}

void EEPROM::setTot(TOT tot) {
    if (tot < 0 || tot > 255)
        return;

    assign(OFFSET_TOT, (uint8_t) tot);
}

Power EEPROM::getLowPower() {
    return (int) data[OFFSET_LOW_POWER];
}

void EEPROM::setLowPower(Power lowPower) {
    if (lowPower <= 0 || lowPower >= 5)
        return;

    assign(OFFSET_LOW_POWER, (uint8_t) lowPower);
}

void EEPROM::assign(int pos, uint8_t value) {
    data.replace(pos, 1, (const char *) &value, 1);
}

bool EEPROM::isValidChannelNumber(Channel channel) {
    return channel >= 0 && channel < CHANNELS_COUNT;
}

Frequency EEPROM::wordToFrequency(uint16_t word, FrequencyBand frequencyBand) {
    switch (frequencyBand) {
        case B0:
        case A9:
            return (Frequency) (word * 6250);
        case TM:
        case T4:
        case U0:
            return (Frequency) ((word + 48000) * 6250);
        default:
            return 0;
    }
}

uint16_t EEPROM::frequencyToWord(Frequency frequency, FrequencyBand frequencyBand) {
    switch (frequencyBand) {
        case B0:
        case A9:
            return (uint16_t) (frequency / 6250);
        case TM:
        case T4:
        case U0:
            return (uint16_t) ((frequency / 6250) - 48000);
        default:
            return 0;
    }
}
