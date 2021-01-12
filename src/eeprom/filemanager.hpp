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

#include <QtCore/QByteArray>
#include <QtCore/QString>

#include "eeprom.hpp"

namespace qfm1000::eeprom {

    enum class FileFormat {
        FORMAT_UNKNOWN,
        FORMAT_BINARY,
        FORMAT_INTEL_HEX
    };

    class FileManager {

    public:

        static bool loadFromFile(EEPROM *eeprom, const QString &filename);

        static bool
        saveToFile(EEPROM *eeprom, const QString &filename, const FileFormat &fileFormat = FileFormat::FORMAT_UNKNOWN);

    private:

        static QByteArray parseFile(const QByteArray &rawData);

        static FileFormat detectFormat(const QByteArray &rawFile);

        static QStringList splitInLines(const QByteArray &rawData);

        static QByteArray intelHexToByteArray(const QByteArray &rawData);

        static QByteArray byteArrayToIntelHex(const QByteArray &rawData);
    };

}

#endif
