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


#include <QtCore/QIODevice>
#include <QtCore/QFile>
#include <QtCore/QStringList>

#include "filemanager.hpp"

using namespace qfm1000::eeprom;

bool FileManager::loadFromFile(EEPROM *eeprom, const QString &filename) {
    QFile file(filename);
    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray rawData = file.readAll();
    QByteArray data = parseFile(rawData);
    file.close();

    if (data.length() != EEPROM_SIZE)
        return false;

    eeprom->setData(data);

    return true;
}

bool FileManager::saveToFile(EEPROM *eeprom, const QString &filename, const FileFormat &fileFormat) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite))
        return false;

    QByteArray data = eeprom->getData();

    switch (fileFormat) {

        case FileFormat::FORMAT_BINARY:
            file.write(data);
            break;

        case FileFormat::FORMAT_INTEL_HEX:
            file.write(byteArrayToIntelHex(data));
            break;

        default:
            break;
    }

    file.close();

    return true;
}

QByteArray FileManager::parseFile(const QByteArray &rawData) {
    QByteArray data;

    FileFormat fileFormat = detectFormat(rawData);

    switch (fileFormat) {

        case FileFormat::FORMAT_BINARY:
            data = rawData;
            break;

        case FileFormat::FORMAT_INTEL_HEX:
            data = intelHexToByteArray(rawData);
            break;

        default:
            data.clear();
            break;
    }

    return data;
}

FileFormat FileManager::detectFormat(const QByteArray &rawData) {
    QStringList rows = splitInLines(rawData);

    bool containsIntelStartCodes = false;
    bool containsOnlyIntelStartCodes = true;

    for (const QString &row : rows)
        if (row.startsWith(":"))
            containsIntelStartCodes = true;
        else
            containsOnlyIntelStartCodes = false;

    if (!containsIntelStartCodes && rawData.size() == EEPROM_SIZE)
        return FileFormat::FORMAT_BINARY;

    if (containsOnlyIntelStartCodes && rows.last() == ":00000001FF")
        return FileFormat::FORMAT_INTEL_HEX;

    return FileFormat::FORMAT_UNKNOWN;
}

QStringList FileManager::splitInLines(const QByteArray &rawData) {
    QString rawFileString = QString::fromLatin1(rawData);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    auto rows = rawFileString.replace('\r', '\n').split('\n', Qt::SkipEmptyParts);
#else
    auto rows = rawFileString.replace('\r', '\n').split('\n', QString::SkipEmptyParts);
#endif
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

QByteArray FileManager::byteArrayToIntelHex(const QByteArray &rawData) {
    // TODO: To implement
    return QByteArray(rawData);
}
