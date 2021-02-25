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


#include "values.hpp"

using namespace qfm1000::eeprom;

int main(int argc, char *argv[]) {
    QTEST_MAIN_IMPL(ValuesTest)
}

[[maybe_unused]] void ValuesTest::frequency() {
    QCOMPARE(Values::frequency(0), "000.000");
    QCOMPARE(Values::frequency(1), "000.000");
    QCOMPARE(Values::frequency(10), "000.000");
    QCOMPARE(Values::frequency(11), "000.000");
    QCOMPARE(Values::frequency(100), "000.000.1");
    QCOMPARE(Values::frequency(101), "000.000.1");
    QCOMPARE(Values::frequency(110), "000.000.1");
    QCOMPARE(Values::frequency(111), "000.000.1");
    QCOMPARE(Values::frequency(1000), "000.001");
    QCOMPARE(Values::frequency(1100), "000.001.1");
    QCOMPARE(Values::frequency(10000), "000.010");
    QCOMPARE(Values::frequency(10200), "000.010.2");
    QCOMPARE(Values::frequency(102000), "000.102");
    QCOMPARE(Values::frequency(102500), "000.102.5");
    QCOMPARE(Values::frequency(1025000), "001.025");
    QCOMPARE(Values::frequency(10000000), "010.000");
    QCOMPARE(Values::frequency(100000000), "100.000");
    QCOMPARE(Values::frequency(144000000), "144.000");
    QCOMPARE(Values::frequency(145600000), "145.600");
    QCOMPARE(Values::frequency(145612500), "145.612.5");
    QCOMPARE(Values::frequency(430000000), "430.000");
    QCOMPARE(Values::frequency(430200000), "430.200");
    QCOMPARE(Values::frequency(430837500), "430.837.5");
}

[[maybe_unused]] void ValuesTest::shift() {
    QCOMPARE(Values::shift(0, 0), "0.0");
    QCOMPARE(Values::shift(1, 0), "+0.0");
    QCOMPARE(Values::shift(10, 0), "+0.0");
    QCOMPARE(Values::shift(100, 0), "+0.1");
    QCOMPARE(Values::shift(1000, 0), "+1.0");
    QCOMPARE(Values::shift(10000, 0), "+10.0");
    QCOMPARE(Values::shift(100000, 0), "+100.0");
    QCOMPARE(Values::shift(1000000, 0), "+1000.0");
    QCOMPARE(Values::shift(0, 1), "-0.0");
    QCOMPARE(Values::shift(0, 10), "-0.0");
    QCOMPARE(Values::shift(0, 100), "-0.1");
    QCOMPARE(Values::shift(0, 1000), "-1.0");
    QCOMPARE(Values::shift(0, 10000), "-10.0");
    QCOMPARE(Values::shift(0, 100000), "-100.0");
    QCOMPARE(Values::shift(0, 1000000), "-1000.0");
    QCOMPARE(Values::shift(145125000, 145725000), "-600.0");
    QCOMPARE(Values::shift(435100000, 430100000), "+5000.0");
}
