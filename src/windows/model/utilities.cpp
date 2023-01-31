/*
 * qFM1000
 * Copyright (C) 2023  Luca Cireddu
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

#include <QtCore/QRegExp>
#include <QtCore/QtMath>

#include "utilities.hpp"

unsigned int ModelUtilities::strFreqToInt(QString input) {
  if (!input.contains(".") and input.length() >= 7) {
    return input.toUInt();
  } else if (input.contains(QRegExp(R"(^\d{1,3}\.{0,1}$)"))) {
    QRegExp regExp(R"(^(\d{1,3})\.{0,1}$)");
    regExp.indexIn(input);
    return regExp.cap(1).toUInt() * 1000000;
  } else if (input.contains(QRegExp(R"(^\d{1,3}\.{0,1}$)"))) {
    QRegExp regExp(R"(^(\d{1,3})\.{0,1}$)");
    regExp.indexIn(input);
    return regExp.cap(1).toUInt() * 1000000;
  } else if (input.contains(QRegExp(R"(^\d{1,3}\.{0,1}\d{0,3}$)"))) {
    QRegExp regExp(R"(^(\d{1,3})\.{0,1}(\d{0,3})$)");
    regExp.indexIn(input);
    return regExp.cap(1).toUInt() * 1000000 +
           regExp.cap(2).leftJustified(3, '0').toUInt() * 1000;
  } else if (input.contains(
                 QRegExp(R"(^\d{1,3}\.{0,1}\d{0,3}\.{0,1}\d{0,3}$)"))) {
    QRegExp regExp(R"(^(\d{1,3})\.{0,1}(\d{0,3})\.{0,1}(\d{0,3})$)");
    regExp.indexIn(input);
    return regExp.cap(1).toUInt() * 1000000 +
           regExp.cap(2).leftJustified(3, '0').toUInt() * 1000 +
           regExp.cap(3).leftJustified(3, '0').toUInt();
  } else {
    return 0;
  }
}

QString ModelUtilities::intFreqToStr(unsigned int input) {
  double khz = (float)input / 1000;
  return QString::number(khz, 'f', 1);
}

QString ModelUtilities::shiftToStr(unsigned int txFreq, unsigned int rxFreq) {
  auto shift = (int)qFabs((int)txFreq - (int)rxFreq);
  double khz = (float)shift / 1000;
  QString value = QString::number(khz, 'f', 1);

  if (txFreq > rxFreq)
    value.insert(0, "+");
  else if (txFreq < rxFreq)
    value.insert(0, "-");

  return value;
}

QString ModelUtilities::boolToStr(bool value) {
  return value ? "enabled" : "disabled";
}
