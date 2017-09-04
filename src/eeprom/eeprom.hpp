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

#ifndef __QFM1000_EEPROM_EEPROM_H
#define __QFM1000_EEPROM_EEPROM_H

#define EEPROM_SIZE 2048

#define CHANNELS_COUNT 96

#define OFFSET_CHANNEL_FIRST 0x26
#define OFFSET_TOT 0x719
#define OFFSET_STARTUP_CHANNEL 0x6D7
#define OFFSET_BUTTONS 0x729
#define OFFSET_LOW_POWER 0x71E

#define OFFSET_LAST_USED_CHANNEL 0x1
#define OFFSET_LAST_USED_CTCSS 0x2

#include <QVector>
#include <QByteArray>

class EEPROM {

private:
    EEPROM();

    static EEPROM *instance;

    QByteArray data;

    void assign(int pos, uint8_t value);

    bool isValidChannelNumber(int channel);

public:
    static EEPROM *getInstance();

    void clear();

    const QByteArray &getData();

    void setData(const QByteArray &data);

    void recompute();

    unsigned int getChannelRxFreq(int channel);

    void setChannelRxFreq(int channel, unsigned int freq);

    unsigned int getChannelTxFreq(int channel);

    void setChannelTxFreq(int channel, unsigned int freq);

    uint8_t getChannelRxCtcss(int channel);

    void setChannelRxCtcss(int channel, int ctcss);

    uint8_t getChannelTxCtcss(int channel);

    void setChannelTxCtcss(int channel, int ctcss);

    unsigned int getChannelPower(int channel);

    void setChannelPower(int channel, unsigned int power);

    bool getChannelSelectiveCalling(int channel);

    void setChannelSelectiveCalling(int channel, bool selectiveCalling);

    bool getChannelCpuOffset(int channel);

    void setChannelCpuOffset(int channel, bool cpuOffset);

    int getDefaultChannel();

    void setDefaultChannel(int defaultChannel);

    uint8_t getTot();

    void setTot(int tot);

    int getLowPower();

    void setLowPower(int lowPower);
};

#endif
