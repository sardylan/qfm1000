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


#ifndef __QFM1000__EEPROM__FILEMANAGER__H
#define __QFM1000__EEPROM__FILEMANAGER__H

#include <QtCore/QDebug>
#include <QtCore/QByteArray>
#include <QtCore/QString>

#include "eeprom.hpp"

#define EEPROM_FILES_NAME_FILTERS QStringList() \
    << "All EEPROM formats [*.bin, *.hex] (*.bin *.hex)" \
    << "Only Binary EEPROM [*.bin] (*.bin)" \
    << "Only Intel HEX EEPROM [*.hex] (*.hex)" \
    << "All files [*] (*)"

namespace qfm1000::eeprom::FileManager {

    enum class FileFormat {
        FORMAT_UNKNOWN,
        FORMAT_BINARY,
        FORMAT_INTEL_HEX
    };

    bool loadFromFile(EEPROM *eeprom, const QString &filename);

    bool saveToFile(EEPROM *eeprom, const QString &filename, const FileFormat &fileFormat = FileFormat::FORMAT_UNKNOWN);

    QByteArray parseFile(const QByteArray &rawData);

    FileFormat detectFormat(const QByteArray &rawFile);

    QStringList splitInLines(const QByteArray &rawData);

    QByteArray intelHexToByteArray(const QByteArray &rawData);

    QByteArray byteArrayToIntelHex(const QByteArray &rawData);

    void registerMetaTypes();

    QDebug operator<<(QDebug debug, const qfm1000::eeprom::FileManager::FileFormat &fileFormat);
}

Q_DECLARE_METATYPE(qfm1000::eeprom::FileManager::FileFormat)

#endif
