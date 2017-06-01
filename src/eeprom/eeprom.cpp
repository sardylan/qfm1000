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

int EEPROM::getLowPower() const {
    return lowPower;
}

void EEPROM::setLowPower(int lowPower) {
    EEPROM::lowPower = lowPower;
}

void EEPROM::updateRawData() {
    for (int i = 0; i < 96; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        unsigned int rxFreq = channels[i]->getRxFreq();
        uint16_t rxValue = (uint16_t) (rxFreq / 6250);
        data[offset + 0] = (uint8_t) (rxValue >> 8);
        data[offset + 1] = (uint8_t) (rxValue & 0xff);

        unsigned int txFreq = channels[i]->getTxFreq();
        uint16_t txValue = (uint16_t) (txFreq / 6250);
        data[offset + 2] = (uint8_t) (txValue >> 8);
        data[offset + 3] = (uint8_t) (txValue & 0xff);

        data[offset + 4] = (uint8_t) channels[i]->getRxCtcss();
        data[offset + 5] = (uint8_t) channels[i]->getRxCtcss();

        switch (channels[i]->getPower()) {
            case 5:
                data[offset + 6] = 0xe8;
                break;
            case 4:
                data[offset + 6] = 0xe0;
                break;
            case 3:
                data[offset + 6] = 0xd8;
                break;
            case 2:
                data[offset + 6] = 0xd0;
                break;
            case 1:
                data[offset + 6] = 0xc8;
                break;
            case 0:
            default:
                data[offset + 6] = 0xc0;
        }

        uint8_t configBit = 0b00101000;
        if (channels[i]->isSelectiveCalling())
            configBit |= 0b00000010;
        if (channels[i]->isCpuOffset())
            configBit |= 0b00000001;

        data[offset + 7] = configBit;
    }

    data[OFFSET_TOT] = (uint8_t) tot;

    data[OFFSET_STARTUP_CHANNEL] = (uint8_t) (defaultChannel >= 0 && defaultChannel < CHANNELS_COUNT
                                              ? defaultChannel
                                              : 0xff);

    data[OFFSET_LOW_POWER] = (uint8_t) lowPower;
}

void EEPROM::updateParams() {
    for (int i = 0; i < 96; i++) {
        int offset = OFFSET_CHANNEL_FIRST + (i * 8);

        uint16_t rxFreqBits = ((uint8_t) data[offset] << 8) | ((uint8_t) data[offset + 1]);
        unsigned int rxFreq = (unsigned int) (rxFreqBits * 6250);
        channels[i]->setRxFreq(rxFreq);

        uint16_t txFreqBits = ((uint8_t) data[offset + 2] << 8) | ((uint8_t) data[offset + 3]);
        unsigned int txFreq = (unsigned int) (txFreqBits * 6250);
        channels[i]->setTxFreq(txFreq);

        uint8_t rxCtcssBit = (uint8_t) data[offset + 4];
        channels[i]->setRxCtcss(rxCtcssBit);

        uint8_t txCtcssBit = (uint8_t) data[offset + 5];
        channels[i]->setTxCtcss(txCtcssBit);

        uint8_t txPowerBit = (uint8_t) data[offset + 6];
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

        uint8_t configBit = (uint8_t) data[offset + 7];

        channels[i]->setSelectiveCalling((bool) (configBit & 0b00000010));
        channels[i]->setCpuOffset((bool) (configBit & 0b00000001));
    }

    tot = (unsigned int) data[OFFSET_TOT];

    defaultChannel = (int) data[OFFSET_STARTUP_CHANNEL] >= 0 && (int) data[OFFSET_STARTUP_CHANNEL] < CHANNELS_COUNT
                     ? (int) data[OFFSET_STARTUP_CHANNEL]
                     : -1;

    lowPower = (unsigned int) data[OFFSET_LOW_POWER];
}
