/*
 * qFM1000
 * Copyright (C) 2023  Luca Cireddu - IS0GVH
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

#include "utilities.hpp"
#include "../../../src/windows/model/utilities.hpp"

QTEST_MAIN(ModelUtilitiesTest)

void ModelUtilitiesTest::strFreqToInt_data() {
  QTest::addColumn<QString>("input");
  QTest::addColumn<unsigned int>("expected");

  QTest::newRow("empty") << "" << 0u;

  QTest::newRow("1000000") << "1000000" << 1000000u;

  QTest::newRow("1") << "1" << 1000000u;
  QTest::newRow("10") << "10" << 10000000u;
  QTest::newRow("40") << "40" << 40000000u;
  QTest::newRow("70") << "70" << 70000000u;
  QTest::newRow("100") << "100" << 100000000u;
  QTest::newRow("145") << "145" << 145000000u;
  QTest::newRow("200") << "200" << 200000000u;
  QTest::newRow("300") << "300" << 300000000u;
  QTest::newRow("400") << "400" << 400000000u;
  QTest::newRow("435") << "435" << 435000000u;

  QTest::newRow("1000") << "1000" << 100000000u;
  QTest::newRow("10000") << "10000" << 100000000u;
  QTest::newRow("40000") << "40000" << 400000000u;
  QTest::newRow("70000") << "70000" << 700000000u;
  QTest::newRow("100000") << "100000" << 100000000u;
  QTest::newRow("145000") << "145000" << 145000000u;
  QTest::newRow("200000") << "200000" << 200000000u;
  QTest::newRow("300000") << "300000" << 300000000u;
  QTest::newRow("400000") << "400000" << 400000000u;
  QTest::newRow("435000") << "435000" << 435000000u;

  QTest::newRow("10.") << "10." << 10000000u;
  QTest::newRow("10.0") << "10.0" << 10000000u;
  QTest::newRow("10.00") << "10.00" << 10000000u;
  QTest::newRow("10.000") << "10.000" << 10000000u;
  QTest::newRow("10.0000") << "10.0000" << 10000000u;
  QTest::newRow("40.") << "40." << 40000000u;
  QTest::newRow("40.0") << "40.0" << 40000000u;
  QTest::newRow("40.00") << "40.00" << 40000000u;
  QTest::newRow("40.000") << "40.000" << 40000000u;
  QTest::newRow("40.0000") << "40.0000" << 40000000u;
  QTest::newRow("70.") << "70." << 70000000u;
  QTest::newRow("70.0") << "70.0" << 70000000u;
  QTest::newRow("70.00") << "70.00" << 70000000u;
  QTest::newRow("70.000") << "70.000" << 70000000u;
  QTest::newRow("70.0000") << "70.0000" << 70000000u;
  QTest::newRow("100.") << "100." << 100000000u;
  QTest::newRow("100.0") << "100.0" << 100000000u;
  QTest::newRow("100.00") << "100.00" << 100000000u;
  QTest::newRow("100.000") << "100.000" << 100000000u;
  QTest::newRow("100.0000") << "100.0000" << 100000000u;
  QTest::newRow("145.") << "145." << 145000000u;
  QTest::newRow("145.0") << "145.0" << 145000000u;
  QTest::newRow("145.00") << "145.00" << 145000000u;
  QTest::newRow("145.000") << "145.000" << 145000000u;
  QTest::newRow("145.0000") << "145.0000" << 145000000u;
  QTest::newRow("200.") << "200." << 200000000u;
  QTest::newRow("200.0") << "200.0" << 200000000u;
  QTest::newRow("200.00") << "200.00" << 200000000u;
  QTest::newRow("200.000") << "200.000" << 200000000u;
  QTest::newRow("200.0000") << "200.0000" << 200000000u;
  QTest::newRow("300.") << "300." << 300000000u;
  QTest::newRow("300.0") << "300.0" << 300000000u;
  QTest::newRow("300.00") << "300.00" << 300000000u;
  QTest::newRow("300.000") << "300.000" << 300000000u;
  QTest::newRow("300.0000") << "300.0000" << 300000000u;
  QTest::newRow("400.") << "400." << 400000000u;
  QTest::newRow("400.0") << "400.0" << 400000000u;
  QTest::newRow("400.00") << "400.00" << 400000000u;
  QTest::newRow("400.000") << "400.000" << 400000000u;
  QTest::newRow("400.0000") << "400.0000" << 400000000u;
  QTest::newRow("435.") << "435." << 435000000u;
  QTest::newRow("435.0") << "435.0" << 435000000u;
  QTest::newRow("435.00") << "435.00" << 435000000u;
  QTest::newRow("435.000") << "435.000" << 435000000u;
  QTest::newRow("435.0000") << "435.0000" << 435000000u;

  QTest::newRow("10.1") << "10.1" << 10100000u;
  QTest::newRow("10.12") << "10.12" << 10120000u;
  QTest::newRow("10.123") << "10.123" << 10123000u;
  QTest::newRow("10.1234") << "10.1234" << 10123400u;
  QTest::newRow("10.12345") << "10.12345" << 10123450u;
  QTest::newRow("10.123456") << "10.123456" << 10123456u;
  QTest::newRow("40.1") << "40.1" << 40100000u;
  QTest::newRow("40.12") << "40.12" << 40120000u;
  QTest::newRow("40.123") << "40.123" << 40123000u;
  QTest::newRow("40.1234") << "40.1234" << 40123400u;
  QTest::newRow("40.12345") << "40.12345" << 40123450u;
  QTest::newRow("40.123456") << "40.123456" << 40123456u;
  QTest::newRow("70.1") << "70.1" << 70100000u;
  QTest::newRow("70.12") << "70.12" << 70120000u;
  QTest::newRow("70.123") << "70.123" << 70123000u;
  QTest::newRow("70.1234") << "70.1234" << 70123400u;
  QTest::newRow("70.12345") << "70.12345" << 70123450u;
  QTest::newRow("70.123456") << "70.123456" << 70123456u;
  QTest::newRow("100.1") << "100.1" << 100100000u;
  QTest::newRow("100.12") << "100.12" << 100120000u;
  QTest::newRow("100.123") << "100.123" << 100123000u;
  QTest::newRow("100.1234") << "100.1234" << 100123400u;
  QTest::newRow("100.12345") << "100.12345" << 100123450u;
  QTest::newRow("100.123456") << "100.123456" << 100123456u;
  QTest::newRow("145.1") << "145.1" << 145100000u;
  QTest::newRow("145.12") << "145.12" << 145120000u;
  QTest::newRow("145.123") << "145.123" << 145123000u;
  QTest::newRow("145.1234") << "145.1234" << 145123400u;
  QTest::newRow("145.12345") << "145.12345" << 145123450u;
  QTest::newRow("145.123456") << "145.123456" << 145123456u;
  QTest::newRow("200.1") << "200.1" << 200100000u;
  QTest::newRow("200.12") << "200.12" << 200120000u;
  QTest::newRow("200.123") << "200.123" << 200123000u;
  QTest::newRow("200.1234") << "200.1234" << 200123400u;
  QTest::newRow("200.12345") << "200.12345" << 200123450u;
  QTest::newRow("200.123456") << "200.123456" << 200123456u;
  QTest::newRow("300.1") << "300.1" << 300100000u;
  QTest::newRow("300.12") << "300.12" << 300120000u;
  QTest::newRow("300.123") << "300.123" << 300123000u;
  QTest::newRow("300.1234") << "300.1234" << 300123400u;
  QTest::newRow("300.12345") << "300.12345" << 300123450u;
  QTest::newRow("300.123456") << "300.123456" << 300123456u;
  QTest::newRow("400.1") << "400.1" << 400100000u;
  QTest::newRow("400.12") << "400.12" << 400120000u;
  QTest::newRow("400.123") << "400.123" << 400123000u;
  QTest::newRow("400.1234") << "400.1234" << 400123400u;
  QTest::newRow("400.12345") << "400.12345" << 400123450u;
  QTest::newRow("400.123456") << "400.123456" << 400123456u;
  QTest::newRow("435.1") << "435.1" << 435100000u;
  QTest::newRow("435.12") << "435.12" << 435120000u;
  QTest::newRow("435.123") << "435.123" << 435123000u;
  QTest::newRow("435.1234") << "435.1234" << 435123400u;
  QTest::newRow("435.12345") << "435.12345" << 435123450u;
  QTest::newRow("435.123456") << "435.123456" << 435123456u;

  QTest::newRow("10.000.") << "10.000." << 10000000u;
  QTest::newRow("10.000.1") << "10.000.1" << 10000100u;
  QTest::newRow("10.000.12") << "10.000.12" << 10000120u;
  QTest::newRow("10.000.123") << "10.000.123" << 10000123u;
  QTest::newRow("40.000.") << "40.000." << 40000000u;
  QTest::newRow("40.000.1") << "40.000.1" << 40000100u;
  QTest::newRow("40.000.12") << "40.000.12" << 40000120u;
  QTest::newRow("40.000.123") << "40.000.123" << 40000123u;
  QTest::newRow("70.000.") << "70.000." << 70000000u;
  QTest::newRow("70.000.1") << "70.000.1" << 70000100u;
  QTest::newRow("70.000.12") << "70.000.12" << 70000120u;
  QTest::newRow("70.000.123") << "70.000.123" << 70000123u;
  QTest::newRow("100.000.") << "100.000." << 100000000u;
  QTest::newRow("100.000.1") << "100.000.1" << 100000100u;
  QTest::newRow("100.000.12") << "100.000.12" << 100000120u;
  QTest::newRow("100.000.123") << "100.000.123" << 100000123u;
  QTest::newRow("145.000.") << "145.000." << 145000000u;
  QTest::newRow("145.000.1") << "145.000.1" << 145000100u;
  QTest::newRow("145.000.12") << "145.000.12" << 145000120u;
  QTest::newRow("145.000.123") << "145.000.123" << 145000123u;
  QTest::newRow("200.000.") << "200.000." << 200000000u;
  QTest::newRow("200.000.1") << "200.000.1" << 200000100u;
  QTest::newRow("200.000.12") << "200.000.12" << 200000120u;
  QTest::newRow("200.000.123") << "200.000.123" << 200000123u;
  QTest::newRow("300.000.") << "300.000." << 300000000u;
  QTest::newRow("300.000.1") << "300.000.1" << 300000100u;
  QTest::newRow("300.000.12") << "300.000.12" << 300000120u;
  QTest::newRow("300.000.123") << "300.000.123" << 300000123u;
  QTest::newRow("400.000.") << "400.000." << 400000000u;
  QTest::newRow("400.000.1") << "400.000.1" << 400000100u;
  QTest::newRow("400.000.12") << "400.000.12" << 400000120u;
  QTest::newRow("400.000.123") << "400.000.123" << 400000123u;
  QTest::newRow("435.000.") << "435.000." << 435000000u;
  QTest::newRow("435.000.1") << "435.000.1" << 435000100u;
  QTest::newRow("435.000.12") << "435.000.12" << 435000120u;
  QTest::newRow("435.000.123") << "435.000.123" << 435000123u;
}

void ModelUtilitiesTest::strFreqToInt() {
  QFETCH(QString, input);
  QFETCH(unsigned int, expected);

  unsigned int actual = ModelUtilities::strFreqToInt(input);
  QCOMPARE(actual, expected);
}
