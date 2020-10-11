/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
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

#ifndef __QFM1000__EEPROM__FILEMANAGER_H
#define __QFM1000__EEPROM__FILEMANAGER_H

#include <QString>

#include "eeprom.hpp"

namespace qfm1000::eeprom {

    class FileManager {

    public:
        static bool loadFromFile(EEPROM *eeprom, const QString& filename);

        static bool saveToFile(EEPROM *eeprom, const QString& filename);

    private:
        static QByteArray parseFile(const QByteArray &rawData);

        static bool isIntelHex(const QByteArray &rawFile);

        static QStringList splitInLines(const QByteArray &rawData);

        static QByteArray intelHexToByteArray(const QByteArray &rawData);
    };

}

#endif
