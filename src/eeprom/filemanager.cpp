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

bool FileManager::loadFromFile(EEPROM *eeprom, QString filename) {
    QFile file(filename);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray rawData = file.readAll();
    QByteArray data = parseFile(rawData);
    file.close();

    if (data.length() != 2048)
        return false;

    eeprom->setData(data);

    return true;
}

bool FileManager::saveToFile(EEPROM *eeprom, QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QByteArray data = eeprom->getData();
    file.write(data);
    file.close();

    return true;
}

QByteArray FileManager::parseFile(const QByteArray &rawData) {
    QByteArray data;

    if (rawData.size() == 2048) {
        data = rawData;
    } else if (isIntelHex(rawData)) {
        data = intelHexToByteArray(rawData);
    }

    return data;
}

bool FileManager::isIntelHex(const QByteArray &rawFile) {
    QStringList rows = splitInLines(rawFile);

    for (const QString &row : rows)
        if (!row.startsWith(":"))
            return false;

    return rows.last() == ":00000001FF";

}

QStringList FileManager::splitInLines(const QByteArray &rawData) {
    QString rawFileString = QString::fromLatin1(rawData);
    QStringList rows = rawFileString.replace('\r', '\n').split('\n', QString::SkipEmptyParts);
    return rows;
}

QByteArray FileManager::intelHexToByteArray(const QByteArray &rawData) {
    QStringList rows = splitInLines(rawData);

    QByteArray data;
    int previousOffest = 0;

    for (const QString &row : rows) {
        unsigned int checksum = 0;
        for (auto &c : QByteArray::fromHex(row.toLatin1()))
            checksum += (unsigned char) c;

        checksum &= 0b11111111;

        if (checksum != 0)
            return QByteArray();

        int size = row.mid(1, 2).toInt(nullptr, 16);
        int offset = row.mid(3, 4).toInt(nullptr, 16);
        int type = row.mid(7, 2).toInt(nullptr, 16);
        QByteArray lineData = QByteArray::fromHex(row.mid(9, size * 2).toLatin1());

        if (type == 0x01)
            break;
        if (type != 0x00)
            continue;

        if (offset != previousOffest)
            return QByteArray();

        previousOffest = offset + size;


        if (lineData.size() != size)
            return QByteArray();

        data.append(lineData);
    }

    return data;
}
