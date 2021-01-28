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


#include <QtCore/QDebug>

#include "instance.hpp"

using namespace qfm1000;
using namespace qfm1000::app;

Instance::Instance(quint64 id, QObject *parent) : QObject(parent), id(id) {
    Instance::oldEeprom = new eeprom::EEPROM();
    Instance::eeprom = new eeprom::EEPROM();

    Instance::window = new windows::Instance();

    connectSignals();
}

Instance::~Instance() {
    delete Instance::window;

    delete Instance::oldEeprom;
    delete Instance::eeprom;
}

quint64 Instance::getId() const {
    return id;
}

eeprom::EEPROM *Instance::getEeprom() const {
    return eeprom;
}

eeprom::EEPROM *Instance::getOldEeprom() const {
    return oldEeprom;
}

windows::Instance *Instance::getWindow() const {
    return window;
}

const QString &Instance::getFileName() const {
    return fileName;
}

void Instance::setFileName(const QString &newValue) {
    qInfo() << "Setting file name";

    Instance::fileName = newValue;
    Instance::window->updateFileName(Instance::fileName);
}

void Instance::connectSignals() {
    qInfo() << "Connecting signals";

    connect(eeprom, &eeprom::EEPROM::byteUpdated, window, &windows::Instance::byteUpdated);
    connect(eeprom, &eeprom::EEPROM::dataUpdated, window, &windows::Instance::dataUpdated);
}

void Instance::resetStatus() {
    qInfo() << "Resetting status";
    Instance::oldEeprom->setData(eeprom->getData());
}
