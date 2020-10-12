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

#include <QtCore/QString>
#include "status.hpp"

Status *Status::instance = nullptr;

Status *Status::getInstance() {
    if (instance == nullptr)
        instance = new Status();

    return instance;
}

Status::Status() {
    frequencyBand = B0;
    currentFileName = "";
    originalData.clear();
    eepromLoaded = false;
    serialEepromOpened = false;
    serialRadioOpened = false;
}

FrequencyBand Status::getFrequencyBand() const {
    return frequencyBand;
}

void Status::setFrequencyBand(FrequencyBand frequencyBand) {
    Status::frequencyBand = frequencyBand;
}

void Status::setOriginalData(const QByteArray &originalData) {
    Status::originalData = originalData;
}

void Status::clearOriginalData() {
    originalData.clear();
}

const QString &Status::getCurrentFileName() const {
    return currentFileName;
}

void Status::setCurrentFileName(const QString &currentFileName) {
    Status::currentFileName = currentFileName;
}

bool Status::isFileOpened() {
    return originalData.size() == EEPROM_SIZE;
}

bool Status::isDataDirty(const QByteArray &eepromData) {
    if (originalData.length() != eepromData.length())
        return true;

    for (int i = 0; i < originalData.length() && i < eepromData.length(); i++)
        if (originalData[i] != eepromData[i])
            return true;

    return false;
}

bool Status::isEepromLoaded() const {
    return eepromLoaded;
}

void Status::setEepromLoaded(bool eepromLoaded) {
    Status::eepromLoaded = eepromLoaded;
}

bool Status::isSerialEepromOpened() const {
    return serialEepromOpened;
}

void Status::setSerialEepromOpened(bool serialEepromOpened) {
    Status::serialEepromOpened = serialEepromOpened;
}

bool Status::isSerialRadioOpened() const {
    return serialRadioOpened;
}

void Status::setSerialRadioOpened(bool serialRadioOpened) {
    Status::serialRadioOpened = serialRadioOpened;
}
