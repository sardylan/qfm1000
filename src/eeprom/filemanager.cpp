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
    QFile file(filename);
    if (!file.exists() || file.size() != 2048)
        return;

    if (!file.open(QIODevice::ReadWrite))
        return;

    //TODO: Implement file seek and byte write for each channel and for TOT feature

    file.close();
}
