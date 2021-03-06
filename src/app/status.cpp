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


#include "status.hpp"

using namespace qfm1000::app;

Status::Status(QObject *parent) : QObject(parent) {
    Status::serialPortName = "";
    Status::serialPortWorking = false;
    Status::instanceId = 0;
}

Status::~Status() = default;

const QString &Status::getSerialPortName() const {
    return serialPortName;
}

void Status::setSerialPortName(const QString &newValue) {
    Status::serialPortName = newValue;
}

bool Status::isSerialPortWorking() const {
    return serialPortWorking;
}

void Status::setSerialPortWorking(bool newValue) {
    Status::serialPortWorking = newValue;
}

quint64 Status::getInstanceId() const {
    return instanceId;
}

void Status::setInstanceId(quint64 newValue) {
    Status::instanceId = newValue;
}

