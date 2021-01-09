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


#ifndef __QFM1000__QFM1000__STATUS__H
#define __QFM1000__QFM1000__STATUS__H

#include <QtCore/QString>
#include <QtCore/QByteArray>

#include "../eeprom/eeprom.hpp"

namespace qfm1000::app {

    class Status {

    public:

        Status();

        ~Status();

        FrequencyBand getFrequencyBand() const;

        void setFrequencyBand(FrequencyBand frequencyBand);

        void setOriginalData(const QByteArray &originalData);

        void clearOriginalData();

        const QString &getCurrentFileName() const;

        void setCurrentFileName(const QString &currentFileName);

        bool isFileOpened();

        bool isDataDirty(const QByteArray &eepromData);

        bool isEepromLoaded() const;

        void setEepromLoaded(bool eepromLoaded);

        bool isSerialEepromOpened() const;

        void setSerialEepromOpened(bool serialEepromOpened);

        bool isSerialRadioOpened() const;

        void setSerialRadioOpened(bool serialRadioOpened);

    private:

        FrequencyBand frequencyBand;
        QByteArray originalData;
        QString currentFileName;
        bool eepromLoaded;
        bool serialEepromOpened;
        bool serialRadioOpened;

    };

}

#endif
