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

#include "eeprom.hpp"

EEPROM *EEPROM::instance = nullptr;

EEPROM *EEPROM::getInstance() {
    if (instance == nullptr)
        instance = new EEPROM();

    return instance;
}

EEPROM::EEPROM() {
    clear();
}

void EEPROM::clear() {
    data.clear();
    for (int i = 0; i < EEPROM_SIZE; i++)
        data.append((char) '\0');
}

const QByteArray &EEPROM::getData() {
    return data;
}

void EEPROM::setData(const QByteArray &data) {
    EEPROM::data = data;
}

bool EEPROM::isValidChannelNumber(int channel) {
    return channel >= 0 && channel < CHANNELS_COUNT;
}

unsigned int EEPROM::wordToFrequency(uint16_t word, FrequencyBand frequencyBand) {
    switch (frequencyBand) {
        case B0:
        case A9:
            return (unsigned int) (word * 6250);
        case TM:
        case T4:
        case U0:
            return (unsigned int) ((word + 48000) * 6250);
        default:
            return 0;
    }
}

uint16_t EEPROM::frequencyToWord(unsigned int frequency, FrequencyBand frequencyBand) {
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

unsigned int EEPROM::getChannelRxFreq(int channel, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((quint8) data[offset] << 8) | ((quint8) data[offset + 1]);
    return wordToFrequency(rxFreqBits, frequencyBand);
}

void EEPROM::setChannelRxFreq(int channel, unsigned int freq, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto rxValue = frequencyToWord(freq, frequencyBand);
    assign(offset + 0, (quint8) (rxValue >> 8));
    assign(offset + 1, (quint8) (rxValue & 0xff));
}

unsigned int EEPROM::getChannelTxFreq(int channel, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((quint8) data[offset + 2] << 8) | ((quint8) data[offset + 3]);
    return wordToFrequency(rxFreqBits, frequencyBand);
}

void EEPROM::setChannelTxFreq(int channel, unsigned int freq, FrequencyBand frequencyBand) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txValue = frequencyToWord(freq, frequencyBand);
    assign(offset + 2, (quint8) (txValue >> 8));
    assign(offset + 3, (quint8) (txValue & 0xff));
}

quint8 EEPROM::getChannelRxCtcss(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    return (quint8) data[offset + 4];
}

void EEPROM::setChannelRxCtcss(int channel, int ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 4, (quint8) ctcss);
}

quint8 EEPROM::getChannelTxCtcss(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    char b = data[offset + 5];
    return (quint8) b;
}

void EEPROM::setChannelTxCtcss(int channel, int ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 5, (quint8) ctcss);
}

unsigned int EEPROM::getChannelPower(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txPowerByte = (quint8) data[offset + 6];
    auto value = static_cast<unsigned int>((txPowerByte & 0b00111000) >> 3);

    return value;
}

void EEPROM::setChannelPower(int channel, unsigned int power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (quint8) data[offset + 6];
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

unsigned int EEPROM::getChannelSquelch(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto txPowerByte = (quint8) data[offset + 7];
    int extractValue = txPowerByte & 0b00011100;
    int shiftedValue = extractValue >> 2;
    auto value = static_cast<unsigned int>(shiftedValue);

    return value;
}

void EEPROM::setChannelSquelch(int channel, unsigned int power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (quint8) data[offset + 7];
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

bool EEPROM::getChannelSelectiveCalling(int channel) {
    if (!isValidChannelNumber(channel))
        return false;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto configBit = (quint8) data[offset + 7];
    return (bool) (configBit & 0b00000010);
}

void EEPROM::setChannelSelectiveCalling(int channel, bool selectiveCalling) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (quint8) data[offset + 7];
    byte &= 0b00000011;
    byte |= 0b00100100;
    if (selectiveCalling)
        byte |= 0b00000010;
    else
        byte &= 0b11111101;
    assign(offset + 7, byte);
}

bool EEPROM::getChannelCpuOffset(int channel) {
    if (!isValidChannelNumber(channel))
        return false;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto configBit = (quint8) data[offset + 7];
    return (bool) (configBit & 0b00000001);
}

void EEPROM::setChannelCpuOffset(int channel, bool cpuOffset) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    auto byte = (quint8) data[offset + 7];
    byte &= 0b00000011;
    byte |= 0b00100100;
    if (cpuOffset)
        byte |= 0b00000001;
    else
        byte &= 0b11111110;
    assign(offset + 7, byte);
}

int EEPROM::getDefaultChannel() {
    return (int) data[OFFSET_STARTUP_CHANNEL] >= 0 && (int) data[OFFSET_STARTUP_CHANNEL] < CHANNELS_COUNT
           ? (int) data[OFFSET_STARTUP_CHANNEL]
           : -1;
}

void EEPROM::setDefaultChannel(int defaultChannel) {
    if (!isValidChannelNumber(defaultChannel))
        return;

    auto byte = (quint8) (defaultChannel >= 0 && defaultChannel < CHANNELS_COUNT
                           ? defaultChannel
                           : 0xff);
    assign(OFFSET_STARTUP_CHANNEL, byte);
}

quint8 EEPROM::getTot() {
    return (quint8) data[OFFSET_TOT];
}

void EEPROM::setTot(int tot) {
    if (tot < 0 || tot > 255)
        return;

    assign(OFFSET_TOT, (quint8) tot);
}

int EEPROM::getLowPower() {
    return (int) data[OFFSET_LOW_POWER];
}

void EEPROM::setLowPower(int lowPower) {
    if (lowPower <= 0 || lowPower >= 5)
        return;

    assign(OFFSET_LOW_POWER, (quint8) lowPower);
}

void EEPROM::assign(int pos, quint8 value) {
    data.replace(pos, 1, (const char *) &value, 1);
}
