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

#define ARDUINO_PROGRAMMER_TEST_SERIAL_PORT "/dev/ttyACM0"
#define ARDUINO_PROGRAMMER_TEST_SERIAL_BAUD QSerialPort::Baud115200

void ArduinoProgrammerTest::initTestCase() {
    programmer = new ArduinoProgrammer();
    programmer->init(ARDUINO_PROGRAMMER_TEST_SERIAL_PORT, ARDUINO_PROGRAMMER_TEST_SERIAL_BAUD);
}

void ArduinoProgrammerTest::cleanupTestCase() {
    programmer->close();

    delete programmer;
}

void ArduinoProgrammerTest::init() {
    while (!programmer->isReady())
        QThread::msleep(100);
}

void ArduinoProgrammerTest::cleanup() {

}

//void ArduinoProgrammerTest::testSinglePage() {
//    QByteArray data;
//    generateRandomData(&data, ARDUINO_PROGRAMMER_EEPROM_SIZE);
//
//    programmer->write(data);
//    QByteArray dataRead = programmer->read();
//
//    QCOMPARE(dataRead, data);
//}

void ArduinoProgrammerTest::generateRandomData(QByteArray *data, size_t len) {
    data->clear();

    for (size_t i = 0; i < len; i++)
        data->append((char) ((((double) qrand()) / RAND_MAX) * 0xff));
}
