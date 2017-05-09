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

#include <QFile>

#include "filemanager.hpp"

void FileManager::loadFromFile(EEPROM *eeprom, QString filename) {
    QFile file(filename);
    if (!file.exists() || file.size() != 2048)
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    file.close();

    eeprom->rawData.clear();
    eeprom->rawData.append(data);

    for (int i = 0; i < 96; i++) {
        uint16_t rxFreqBits = ((uint8_t) data.at(0x26 + (i * 8)) << 8) | ((uint8_t) data.at(0x27 + (i * 8)));
        unsigned int rxFreq = (unsigned int) (rxFreqBits * 6250);
        eeprom->channels[i]->setRxFreq(rxFreq);

        uint16_t txFreqBits = ((uint8_t) data.at(0x28 + (i * 8)) << 8) | ((uint8_t) data.at(0x29 + (i * 8)));
        unsigned int txFreq = (unsigned int) (txFreqBits * 6250);
        eeprom->channels[i]->setTxFreq(txFreq);

        uint8_t rxCtcssBit = (uint8_t) data.at(0x2a + (i * 8));
        eeprom->channels[i]->setRxCtcss(rxCtcssBit);

        uint8_t txCtcssBit = (uint8_t) data.at(0x2b + (i * 8));
        eeprom->channels[i]->setTxCtcss(txCtcssBit);

        uint8_t txPowerBit = (uint8_t) data.at(0x2c + (i * 8));
        switch (txPowerBit) {
            case 0xe8:
                eeprom->channels[i]->setPower(5);
                break;
            case 0xe0:
                eeprom->channels[i]->setPower(4);
                break;
            case 0xd8:
                eeprom->channels[i]->setPower(3);
                break;
            case 0xd0:
                eeprom->channels[i]->setPower(2);
                break;
            case 0xc8:
                eeprom->channels[i]->setPower(1);
                break;
            case 0xc0:
            default:
                eeprom->channels[i]->setPower(0);
        }

        uint8_t configBit = (uint8_t) data.at(0x2d + (i * 8));

        eeprom->channels[i]->setSelectiveCalling((bool) (configBit & 0b00000010));
        eeprom->channels[i]->setCpuOffset((bool) (configBit & 0b00000001));
    }

    eeprom->defaultChannel = (uint8_t) data.at(0x1);
    eeprom->tot = (uint8_t) data.at(0x719);
}

void FileManager::saveToFile(EEPROM *eeprom, QString filename) {
    QByteArray data;
    data.clear();
    data.append(eeprom->rawData);

    for (int i = 0; i < 96; i++) {
        unsigned int rxFreq = eeprom->channels[i]->getRxFreq();
        uint16_t rxValue = (uint16_t) (rxFreq / 6250);
        data[0x26 + (i * 8)] = (uint8_t) (rxValue >> 8);
        data[0x27 + (i * 8)] = (uint8_t) (rxValue & 0xff);

        unsigned int txFreq = eeprom->channels[i]->getTxFreq();
        uint16_t txValue = (uint16_t) (txFreq / 6250);
        data[0x28 + (i * 8)] = (uint8_t) (txValue >> 8);
        data[0x29 + (i * 8)] = (uint8_t) (txValue & 0xff);

        data[0x2a + (i * 8)] = (uint8_t) eeprom->channels[i]->getRxCtcss();
        data[0x2b + (i * 8)] = (uint8_t) eeprom->channels[i]->getRxCtcss();

        switch (eeprom->channels[i]->getPower()) {
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
        if (eeprom->channels[i]->isSelectiveCalling())
            configBit |= 0b00000010;
        if (eeprom->channels[i]->isCpuOffset())
            configBit |= 0b00000001;

        data[0x2d + (i * 8)] = configBit;
    }

    data[0x719] = eeprom->tot;
    data[0x1] = eeprom->defaultChannel;

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return;

    file.write(data);
    file.close();
}
