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

#include "programmer_test.hpp"

QTEST_MAIN(ArduinoProgrammerTest)

#define CUSTOM_QTRY_COMPARE_NO_TIMEOUT(expr, expected) QTRY_COMPARE_WITH_TIMEOUT((expr), expected, 0)

void ArduinoProgrammerTest::initTestCase() {

}

void ArduinoProgrammerTest::cleanupTestCase() {

}

void ArduinoProgrammerTest::init() {

}

void ArduinoProgrammerTest::cleanup() {

}