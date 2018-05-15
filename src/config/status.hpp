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

#ifndef __QFM1000_CONFIG_STATUS_H
#define __QFM1000_CONFIG_STATUS_H

#include <QString>
#include <QByteArray>

#include <eeprom.hpp>

class Status {

private:
    Status();

    static Status *instance;

    FrequencyBand frequencyBand;
    QByteArray originalData;
    QString currentFileName;
    bool serialEepromOpened;
    bool serialRadioOpened;

public:
    static Status *getInstance();

    FrequencyBand getFrequencyBand() const;

    void setFrequencyBand(FrequencyBand frequencyBand);

    void setOriginalData(const QByteArray &originalData);

    void clearOriginalData();

    const QString &getCurrentFileName() const;

    void setCurrentFileName(const QString &currentFileName);

    bool isDataDirty(const QByteArray &eepromData);

    bool isSerialEepromOpened() const;

    void setSerialEepromOpened(bool serialEepromOpened);

    bool isSerialRadioOpened() const;

    void setSerialRadioOpened(bool serialRadioOpened);
};

#endif
