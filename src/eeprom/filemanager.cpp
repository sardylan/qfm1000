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
    }

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

        uint8_t rxCtcss = (uint8_t) eeprom->channels[i]->getRxCtcss();
        data[0x2a + (i * 8)] = rxCtcss;

        uint8_t txCtcss = (uint8_t) eeprom->channels[i]->getRxCtcss();
        data[0x2b + (i * 8)] = txCtcss;
    }

    data[0x719] = eeprom->tot;

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return;

    file.write(data);
    file.close();
}
