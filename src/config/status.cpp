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

#include <QtCore/QString>
#include "status.hpp"

Status *Status::instance = nullptr;

Status *Status::getInstance() {
    if (instance == 0)
        instance = new Status();

    return instance;
}

Status::Status() {
    serialOpened = false;
    currentFileName = "";
    originalData.clear();
}

const QString &Status::getCurrentFileName() const {
    return currentFileName;
}

void Status::setCurrentFileName(const QString &currentFileName) {
    Status::currentFileName = currentFileName;
}

void Status::setOriginalData(const QByteArray &originalData) {
    Status::originalData = originalData;
}

void Status::clearOriginalData() {
    originalData.clear();
}

bool Status::isSerialOpened() const {
    return serialOpened;
}

void Status::setSerialOpened(bool serialOpen) {
    Status::serialOpened = serialOpen;
}

bool Status::isDataDirty(const QByteArray &eepromData) {
    if (originalData.length() != eepromData.length())
        return true;

    for (int i = 0; i < originalData.length() && i < eepromData.length(); i++)
        if (originalData[i] != eepromData[i])
            return true;

    return false;
}
