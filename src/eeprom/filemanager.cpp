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

    eeprom->setData(data);
}

void FileManager::saveToFile(EEPROM *eeprom, QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return;

    QByteArray data = eeprom->getData();
    file.write(data);
    file.close();
}
