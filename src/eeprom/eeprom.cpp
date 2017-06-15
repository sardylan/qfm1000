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
    if (instance == 0)
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

unsigned int EEPROM::getChannelRxFreq(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((uint8_t) data[offset] << 8) | ((uint8_t) data[offset + 1]);
    return (unsigned int) (rxFreqBits * 6250);
}

void EEPROM::setChannelRxFreq(int channel, unsigned int freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxValue = (uint16_t) (freq / 6250);
    assign(offset + 0, (uint8_t) (rxValue >> 8));
    assign(offset + 1, (uint8_t) (rxValue & 0xff));
}

unsigned int EEPROM::getChannelTxFreq(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t rxFreqBits = ((uint8_t) data[offset + 2] << 8) | ((uint8_t) data[offset + 3]);
    return (unsigned int) (rxFreqBits * 6250);
}

void EEPROM::setChannelTxFreq(int channel, unsigned int freq) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint16_t txValue = (uint16_t) (freq / 6250);
    assign(offset + 2, (uint8_t) (txValue >> 8));
    assign(offset + 3, (uint8_t) (txValue & 0xff));
}

uint8_t EEPROM::getChannelRxCtcss(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    return (uint8_t) data[offset + 4];
}

void EEPROM::setChannelRxCtcss(int channel, int ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 4, (uint8_t) ctcss);
}

uint8_t EEPROM::getChannelTxCtcss(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    return (uint8_t) data[offset + 5];
}

void EEPROM::setChannelTxCtcss(int channel, int ctcss) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    assign(offset + 5, (uint8_t) ctcss);
}

unsigned int EEPROM::getChannelPower(int channel) {
    if (!isValidChannelNumber(channel))
        return 0;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint8_t txPowerByte = (uint8_t) data[offset + 6];
    switch (txPowerByte) {
        case 0xe8:
            return 5;
        case 0xe0:
            return 4;
        case 0xd8:
            return 3;
        case 0xd0:
            return 2;
        case 0xc8:
            return 1;
        case 0xc0:
        default:
            return 0;
    }
}

void EEPROM::setChannelPower(int channel, unsigned int power) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint8_t byte;
    switch (power) {
        case 5:
            byte = 0xe8;
            break;
        case 4:
            byte = 0xe0;
            break;
        case 3:
            byte = 0xd8;
            break;
        case 2:
            byte = 0xd0;
            break;
        case 1:
            byte = 0xc8;
            break;
        case 0:
        default:
            byte = 0xc0;
    }

    assign(offset + 6, byte);
}

bool EEPROM::getChannelSelectiveCalling(int channel) {
    if (!isValidChannelNumber(channel))
        return false;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint8_t configBit = (uint8_t) data[offset + 7];
    return (bool) (configBit & 0b00000010);
}

void EEPROM::setChannelSelectiveCalling(int channel, bool selectiveCalling) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint8_t byte = (uint8_t) data[offset + 7];
    byte |= 0b00101000;
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
    uint8_t configBit = (uint8_t) data[offset + 7];
    return (bool) (configBit & 0b00000001);
}

void EEPROM::setChannelCpuOffset(int channel, bool cpuOffset) {
    if (!isValidChannelNumber(channel))
        return;

    int offset = OFFSET_CHANNEL_FIRST + (channel * 8);
    uint8_t byte = (uint8_t) data[offset + 7];
    byte |= 0b00101000;
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

    uint8_t byte = (uint8_t) (defaultChannel >= 0 && defaultChannel < CHANNELS_COUNT
                              ? defaultChannel
                              : 0xff);
    assign(OFFSET_STARTUP_CHANNEL, byte);
}

uint8_t EEPROM::getTot() {
    return (uint8_t) data[OFFSET_TOT];
}

void EEPROM::setTot(int tot) {
    if (tot < 0 || tot > 255)
        return;

    assign(OFFSET_TOT, (uint8_t) tot);
}

int EEPROM::getLowPower() {
    return (int) data[OFFSET_LOW_POWER];
}

void EEPROM::setLowPower(int lowPower) {
    if (lowPower <= 0 || lowPower >= 5)
        return;

    assign(OFFSET_LOW_POWER, (uint8_t) lowPower);
}

void EEPROM::assign(int pos, uint8_t value) {
    data.replace(pos, 1, (const char *) &value, 1);
}
