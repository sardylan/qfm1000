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


#ifndef __QFM1000__EEPROM__TEST__VALUES__H
#define __QFM1000__EEPROM__TEST__VALUES__H

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include "../values.hpp"

namespace qfm1000::eeprom {

    class ValuesTest : public QObject {
    Q_OBJECT

    private slots:

        [[maybe_unused]] void frequency();

        [[maybe_unused]] void frequency_data();

        [[maybe_unused]] void shift();

        [[maybe_unused]] void shift_data();

    };

}

#endif
