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

    private slots:

        [[maybe_unused]] void initTestCase();

        [[maybe_unused]] void cleanupTestCase();

        [[maybe_unused]] void init();

        [[maybe_unused]] void cleanup();

        [[maybe_unused]] void clear();

        [[maybe_unused]] void frequencyBand();

        [[maybe_unused]] void channelRxFreq();

        [[maybe_unused]] void channelRxFreqBytes();

        [[maybe_unused]] void channelTxFreq();

        [[maybe_unused]] void channelTxFreqBytes();

        [[maybe_unused]] void channelRxCtcss();

        [[maybe_unused]] void channelRxCtcssBytes();

        [[maybe_unused]] void channelTxCtcss();

        [[maybe_unused]] void channelTxCtcssBytes();

        [[maybe_unused]] void channelPower();

        [[maybe_unused]] void channelPowerBytes();

        [[maybe_unused]] void channelSquelch();

        [[maybe_unused]] void channelSquelchBytes();

        [[maybe_unused]] void channelSelectiveCalling();

        [[maybe_unused]] void channelSelectiveCallingBytes();

        [[maybe_unused]] void channelCpuOffset();

        [[maybe_unused]] void channelCpuOffsetBytes();

        [[maybe_unused]] void startupChannel();

        [[maybe_unused]] void startupChannelBytes();

        [[maybe_unused]] void keyBeep();

        [[maybe_unused]] void keyBeepBytes();

        [[maybe_unused]] void tot();

        [[maybe_unused]] void totBytes();

        [[maybe_unused]] void lowPower();

        [[maybe_unused]] void lowPowerBytes();

    };

}

#endif
