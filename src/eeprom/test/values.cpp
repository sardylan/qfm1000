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
    QFETCH(Frequency, input);
    QFETCH(QString, expected);

    QString actual = Values::frequency(input);
    QCOMPARE(actual, expected);
}

[[maybe_unused]] void ValuesTest::frequency_data() {
    QTest::addColumn<Frequency>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("0 Hz") << 0u << "000.000";
    QTest::newRow("1 Hz") << 1u << "000.000";
    QTest::newRow("10 Hz") << 10u << "000.000";
    QTest::newRow("11 Hz") << 11u << "000.000";
    QTest::newRow("100 Hz") << 100u << "000.000.1";
    QTest::newRow("101 Hz") << 101u << "000.000.1";
    QTest::newRow("110 Hz") << 110u << "000.000.1";
    QTest::newRow("111 Hz") << 111u << "000.000.1";
    QTest::newRow("1000 Hz") << 1000u << "000.001";
    QTest::newRow("1100 Hz") << 1100u << "000.001.1";
    QTest::newRow("10000 Hz") << 10000u << "000.010";
    QTest::newRow("10200 Hz") << 10200u << "000.010.2";
    QTest::newRow("102000 Hz") << 102000u << "000.102";
    QTest::newRow("102500 Hz") << 102500u << "000.102.5";
    QTest::newRow("1025000 Hz") << 1025000u << "001.025";
    QTest::newRow("10000000 Hz") << 10000000u << "010.000";
    QTest::newRow("100000000 Hz") << 100000000u << "100.000";
    QTest::newRow("144000000 Hz") << 144000000u << "144.000";
    QTest::newRow("145600000 Hz") << 145600000u << "145.600";
    QTest::newRow("145612500 Hz") << 145612500u << "145.612.5";
    QTest::newRow("430000000 Hz") << 430000000u << "430.000";
    QTest::newRow("430200000 Hz") << 430200000u << "430.200";
    QTest::newRow("430837500 Hz") << 430837500u << "430.837.5";
}

[[maybe_unused]] void ValuesTest::shift() {
    QFETCH(Frequency, inputTx);
    QFETCH(Frequency, inputRx);
    QFETCH(QString, expected);

    QString actual = Values::shift(inputTx, inputRx);
    QCOMPARE(actual, expected);
}

[[maybe_unused]] void ValuesTest::shift_data() {
    QTest::addColumn<Frequency>("inputTx");
    QTest::addColumn<Frequency>("inputRx");
    QTest::addColumn<QString>("expected");

    QTest::newRow("0") << 0u << 0u << "0.0";
    QTest::newRow("1 Hz") << 1u << 0u << "+0.0";
    QTest::newRow("10 Hz") << 10u << 0u << "+0.0";
    QTest::newRow("100 Hz") << 100u << 0u << "+0.1";
    QTest::newRow("1 kHz") << 1000u << 0u << "+1.0";
    QTest::newRow("10 kHz") << 10000u << 0u << "+10.0";
    QTest::newRow("100 kHz") << 100000u << 0u << "+100.0";
    QTest::newRow("1 MHz") << 1000000u << 0u << "+1000.0";

    QTest::newRow("1 Hz") << 0u << 1u << "-0.0";
    QTest::newRow("10 Hz") << 0u << 10u << "-0.0";
    QTest::newRow("100 Hz") << 0u << 100u << "-0.1";
    QTest::newRow("1 kHz") << 0u << 1000u << "-1.0";
    QTest::newRow("10 kHz") << 0u << 10000u << "-10.0";
    QTest::newRow("100 kHz") << 0u << 100000u << "-100.0";
    QTest::newRow("1 MHz") << 0u << 1000000u << "-1000.0";

    QTest::newRow("+600 kHz") << 145125000u << 145725000u << "-600.0";
    QTest::newRow("+5 MHz") << 435100000u << 430100000u << "+5000.0";
}
