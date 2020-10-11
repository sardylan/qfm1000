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

#ifndef __QFM1000__EEPROM__EEPROMTEST_H
#define __QFM1000__EEPROM__EEPROMTEST_H

#include <QtTest/QtTest>

#include <eeprom.hpp>

namespace qfm1000::eeprom {

    class EEPROMTest : public QObject {

    Q_OBJECT

    private slots:

        void testClear();

        void testChannelRxFreq();

        void testChannelTxFreq();

        void testChannelRxCtcss();

        void testChannelTxCtcss();

        void testChannelPower();

        void testChannelSquelch();

        void testChannelSelectiveCalling();

        void testChannelCpuOffset();

        void testDefaultChannel();

        void testTot();

        void testLowPower();

    };

}

#endif
