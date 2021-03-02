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


#ifndef __QFM1000__EEPROM__TEST__EEPROM__H
#define __QFM1000__EEPROM__TEST__EEPROM__H

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include "../eeprom.hpp"

namespace qfm1000::eeprom {

    class EEPROMTest : public QObject {
    Q_OBJECT

    private:

        EEPROM *eeprom;
        QSignalSpy *signalSpy;

    private slots:

         void initTestCase();

         void cleanupTestCase();

         void init();

         void cleanup();

         void clear();

         void frequencyBand();

         void channelRxFreq();

         void channelRxFreqBytes();

         void channelTxFreq();

         void channelTxFreqBytes();

         void channelRxCtcss();

         void channelRxCtcssBytes();

         void channelTxCtcss();

         void channelTxCtcssBytes();

         void channelPower();

         void channelPowerBytes();

         void channelSquelch();

         void channelSquelchBytes();

         void channelSelectiveCalling();

         void channelSelectiveCallingBytes();

         void channelCpuOffset();

         void channelCpuOffsetBytes();

         void startupChannel();

         void startupChannelBytes();

         void keyBeep();

         void keyBeepBytes();

         void tot();

         void totBytes();

         void lowPower();

         void lowPowerBytes();

    };

}

#endif
