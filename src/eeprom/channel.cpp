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

#include "channel.hpp"

Channel::Channel() {
    rxFreq = 144000000;
    txFreq = 144000000;
    rxCtcss = 0;
    txCtcss = 0;
    power = 5;
    selectiveCalling = false;
    cpuOffset = false;
}

unsigned int Channel::getRxFreq() const {
    return rxFreq;
}

void Channel::setRxFreq(unsigned int rxFreq) {
    Channel::rxFreq = rxFreq;
}

unsigned int Channel::getTxFreq() const {
    return txFreq;
}

void Channel::setTxFreq(unsigned int txFreq) {
    Channel::txFreq = txFreq;
}

unsigned int Channel::getRxCtcss() const {
    return rxCtcss;
}

void Channel::setRxCtcss(unsigned int rxCtcss) {
    Channel::rxCtcss = rxCtcss;
}

unsigned int Channel::getTxCtcss() const {
    return txCtcss;
}

void Channel::setTxCtcss(unsigned int txCtcss) {
    Channel::txCtcss = txCtcss;
}

unsigned int Channel::getPower() const {
    return power;
}

void Channel::setPower(unsigned int power) {
    Channel::power = power;
}

bool Channel::isSelectiveCalling() const {
    return selectiveCalling;
}

void Channel::setSelectiveCalling(bool selectiveCalling) {
    Channel::selectiveCalling = selectiveCalling;
}

bool Channel::isCpuOffset() const {
    return cpuOffset;
}

void Channel::setCpuOffset(bool cpuOffset) {
    Channel::cpuOffset = cpuOffset;
}
