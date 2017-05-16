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

#include "eeprom.hpp"

EEPROM *EEPROM::instance = nullptr;

EEPROM *EEPROM::getInstance() {
    if (instance == 0)
        instance = new EEPROM();

    return instance;
}


EEPROM::EEPROM() {
    for (int i = 0; i < CHANNELS_COUNT; i++) {
        channels[i] = new Channel();
    }

    defaultChannel = -1;
    tot = 0;
}

const QByteArray &EEPROM::getData() const {
    return data;
}

void EEPROM::setData(const QByteArray &data) {
    EEPROM::data = data;
}

Channel *EEPROM::getChannel(int index) const {
    return channels[index];
}

int EEPROM::getDefaultChannel() const {
    return defaultChannel;
}

void EEPROM::setDefaultChannel(int defaultChannel) {
    EEPROM::defaultChannel = defaultChannel;
}

int EEPROM::getTot() const {
    return tot;
}

void EEPROM::setTot(int tot) {
    EEPROM::tot = tot;
}
