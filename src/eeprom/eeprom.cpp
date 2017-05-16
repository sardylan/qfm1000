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
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        channels[i] = new Channel();
    }

    defaultChannel = -1;
    tot = 0;
}

const QByteArray &EEPROM::getData() {
    updateRawData();
    return data;
}

void EEPROM::setData(const QByteArray &data) {
    EEPROM::data = data;
    updateParams();
}

Channel *EEPROM::getChannel(int index) const {
    return channels[index];
}

int EEPROM::getDefaultChannel() const {
    return defaultChannel;
}

void EEPROM::setDefaultChannel(int defaultChannel) {
    EEPROM::defaultChannel = defaultChannel;
}

int EEPROM::getTot() const {
    return tot;
}

void EEPROM::setTot(int tot) {
    EEPROM::tot = tot;
}

void EEPROM::updateRawData() {
    for (int i = 0; i < 96; i++) {
        unsigned int rxFreq = channels[i]->getRxFreq();
        uint16_t rxValue = (uint16_t) (rxFreq / 6250);
        data[0x26 + (i * 8)] = (uint8_t) (rxValue >> 8);
        data[0x27 + (i * 8)] = (uint8_t) (rxValue & 0xff);

        unsigned int txFreq = channels[i]->getTxFreq();
        uint16_t txValue = (uint16_t) (txFreq / 6250);
        data[0x28 + (i * 8)] = (uint8_t) (txValue >> 8);
        data[0x29 + (i * 8)] = (uint8_t) (txValue & 0xff);

        data[0x2a + (i * 8)] = (uint8_t) channels[i]->getRxCtcss();
        data[0x2b + (i * 8)] = (uint8_t) channels[i]->getRxCtcss();

        switch (channels[i]->getPower()) {
            case 5:
                data[0x2c + (i * 8)] = 0xe8;
                break;
            case 4:
                data[0x2c + (i * 8)] = 0xe0;
                break;
            case 3:
                data[0x2c + (i * 8)] = 0xd8;
                break;
            case 2:
                data[0x2c + (i * 8)] = 0xd0;
                break;
            case 1:
                data[0x2c + (i * 8)] = 0xc8;
                break;
            case 0:
            default:
                data[0x2c + (i * 8)] = 0xc0;
        }

        uint8_t configBit = 0b00101000;
        if (channels[i]->isSelectiveCalling())
            configBit |= 0b00000010;
        if (channels[i]->isCpuOffset())
            configBit |= 0b00000001;

        data[0x2d + (i * 8)] = configBit;
    }

    data[0x719] = (uint8_t) tot;
    data[0x1] = (uint8_t) (defaultChannel >= 0 && defaultChannel < CHANNELS_COUNT
                           ? defaultChannel
                           : 0xff);
}

void EEPROM::updateParams() {
    for (int i = 0; i < 96; i++) {
        uint16_t rxFreqBits = ((uint8_t) data[0x26 + (i * 8)] << 8) | ((uint8_t) data[0x27 + (i * 8)]);
        unsigned int rxFreq = (unsigned int) (rxFreqBits * 6250);
        channels[i]->setRxFreq(rxFreq);

        uint16_t txFreqBits = ((uint8_t) data[0x28 + (i * 8)] << 8) | ((uint8_t) data[0x29 + (i * 8)]);
        unsigned int txFreq = (unsigned int) (txFreqBits * 6250);
        channels[i]->setTxFreq(txFreq);

        uint8_t rxCtcssBit = (uint8_t) data[0x2a + (i * 8)];
        channels[i]->setRxCtcss(rxCtcssBit);

        uint8_t txCtcssBit = (uint8_t) data[0x2b + (i * 8)];
        channels[i]->setTxCtcss(txCtcssBit);

        uint8_t txPowerBit = (uint8_t) data[0x2c + (i * 8)];
        switch (txPowerBit) {
            case 0xe8:
                channels[i]->setPower(5);
                break;
            case 0xe0:
                channels[i]->setPower(4);
                break;
            case 0xd8:
                channels[i]->setPower(3);
                break;
            case 0xd0:
                channels[i]->setPower(2);
                break;
            case 0xc8:
                channels[i]->setPower(1);
                break;
            case 0xc0:
            default:
                channels[i]->setPower(0);
        }

        uint8_t configBit = (uint8_t) data[0x2d + (i * 8)];

        channels[i]->setSelectiveCalling((bool) (configBit & 0b00000010));
        channels[i]->setCpuOffset((bool) (configBit & 0b00000001));
    }

    defaultChannel = (int) data[0x1] >= 0 && (int) data[0x1] < CHANNELS_COUNT ? (int) data[0x1] : -1;
    tot = (unsigned int) data[0x719];
}