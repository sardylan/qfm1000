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


#include <QtCore/QByteArray>
#include <QtCore/QStringList>

#include "filemanager.hpp"

#include "../../src/eeprom/filemanager.hpp"

using namespace qfm1000::eeprom;

#define FAKE_EEPROM_DATA_2047   "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f" \
                                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e"

#define FAKE_EEPROM_DATA_INTEL ":0123456789ABCDEF"
#define FAKE_EEPROM_DATA_INTEL_LAST_LINE ":00000001FF"

QTEST_MAIN(FileManagerTest)

void FileManagerTest::detectFormat() {
    QFETCH(QByteArray, input);
    QFETCH(FileManager::FileFormat, expected);

    FileManager::FileFormat actual = FileManager::detectFormat(input);
    QCOMPARE(actual, expected);
}

void FileManagerTest::detectFormat_data() {
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<FileManager::FileFormat>("expected");

    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("\n") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("\n\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("a") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("a\n") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("a\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral("a\n\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral(":a") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral(":a\n") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral(":a\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN") << QByteArrayLiteral(":a\n\r") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN")
            << QByteArrayLiteral(FAKE_EEPROM_DATA_2047) << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN")
            << QByteArrayLiteral(":" FAKE_EEPROM_DATA_2047) << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN")
            << QByteArrayLiteral(FAKE_EEPROM_DATA_2047 ":") << FileManager::FileFormat::FORMAT_UNKNOWN;
    QTest::newRow("FORMAT_UNKNOWN")
            << QByteArrayLiteral("a" FAKE_EEPROM_DATA_INTEL "\n" FAKE_EEPROM_DATA_INTEL_LAST_LINE) << FileManager::FileFormat::FORMAT_UNKNOWN;

    QTest::newRow("FORMAT_BINARY")
            << QByteArrayLiteral(FAKE_EEPROM_DATA_2047 "\x0f") << FileManager::FileFormat::FORMAT_BINARY;

    QTest::newRow("FORMAT_INTEL_HEX")
            << QByteArrayLiteral(FAKE_EEPROM_DATA_INTEL "\n" FAKE_EEPROM_DATA_INTEL_LAST_LINE) << FileManager::FileFormat::FORMAT_INTEL_HEX;
}

void FileManagerTest::splitInLines() {
    QFETCH(QByteArray, input);
    QFETCH(QStringList, expected);

    QStringList actual = FileManager::splitInLines(input);
    QCOMPARE(actual, expected);
}

void FileManagerTest::splitInLines_data() {
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("zero row") << QByteArrayLiteral("") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\n") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\n\n") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\r") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\r\r") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\n\r") << QStringList{};
    QTest::newRow("zero row") << QByteArrayLiteral("\n\r\n\r") << QStringList{};

    QTest::newRow("one row") << QByteArrayLiteral("a") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("\na") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("a\n") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("a") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("\ra") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("a\r") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("a") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("\n\ra") << QStringList{"a"};
    QTest::newRow("one row") << QByteArrayLiteral("a\n\r") << QStringList{"a"};

    QTest::newRow("two rows") << QByteArrayLiteral("a\nb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\na\nb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("a\nb\n") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\na\nb\n") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("a\rb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\ra\rb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("a\rb\r") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\ra\rb\r") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("a\n\rb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\n\ra\n\rb") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("a\n\rb\n\r") << QStringList{"a", "b"};
    QTest::newRow("two rows") << QByteArrayLiteral("\n\ra\n\rb\n\r") << QStringList{"a", "b"};
}

void FileManagerTest::intelHexToByteArray() {

}

void FileManagerTest::byteArrayToIntelHex() {

}

