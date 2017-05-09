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

#ifndef __QFM1000_EEPROM_CHANNEL_H
#define __QFM1000_EEPROM_CHANNEL_H

class Channel {

private:
    unsigned int rxFreq;
    unsigned int txFreq;
    unsigned int rxCtcss;
    unsigned int txCtcss;
    unsigned int power;
    bool selectiveCalling;
    bool cpuOffset;
public:
    bool isSelectiveCalling() const;

    void setSelectiveCalling(bool selectiveCalling);

    bool isCpuOffset() const;

    void setCpuOffset(bool cpuOffset);

public:
    Channel();

    unsigned int getRxFreq() const;

    void setRxFreq(unsigned int rxFreq);

    unsigned int getTxFreq() const;

    void setTxFreq(unsigned int txFreq);

    unsigned int getRxCtcss() const;

    void setRxCtcss(unsigned int rxCtcss);

    unsigned int getTxCtcss() const;

    void setTxCtcss(unsigned int txCtcss);

    unsigned int getPower() const;

    void setPower(unsigned int power);
};

#endif
