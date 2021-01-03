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


#ifndef __QFM1000__EEPROM__EEPROM__TEST_H
#define __QFM1000__EEPROM__EEPROM__TEST_H

#include <QtTest>

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

        [[maybe_unused]] void testClear();

        [[maybe_unused]] void testChannelRxFreq();

        [[maybe_unused]] void testChannelTxFreq();

        [[maybe_unused]] void testChannelRxCtcss();

        [[maybe_unused]] void testChannelTxCtcss();

        [[maybe_unused]] void testChannelPower();

        [[maybe_unused]] void testChannelSquelch();

        [[maybe_unused]] void testChannelSelectiveCalling();

        [[maybe_unused]] void testChannelCpuOffset();

        [[maybe_unused]] void testDefaultChannel();

        [[maybe_unused]] void testTot();

        [[maybe_unused]] void testLowPower();

    };

}

#endif
