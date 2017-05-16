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

#ifndef __QFM1000_EEPROM_CONFIG_H
#define __QFM1000_EEPROM_CONFIG_H

#define CHANNELS_COUNT 96

#include <QVector>
#include <QByteArray>

#include "channel.hpp"

class EEPROM {

private:
    EEPROM();

    static EEPROM *instance;

    void updateRawData();

    void updateParams();

    QByteArray data;

    Channel *channels[CHANNELS_COUNT];
    int defaultChannel;
    int tot;

public:
    static EEPROM *getInstance();

    const QByteArray &getData();

    void setData(const QByteArray &data);

    Channel *getChannel(int index) const;

    int getDefaultChannel() const;

    void setDefaultChannel(int defaultChannel);

    int getTot() const;

    void setTot(int tot);

};

#endif
