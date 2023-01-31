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

#ifndef __QFM1000__WINDOWS__MODEL__UTILITIES__H
#define __QFM1000__WINDOWS__MODEL__UTILITIES__H

#include <QtCore/QString>

class ModelUtilities {

public:
  static unsigned int strFreqToInt(QString input);

  static QString intFreqToStr(unsigned int input);

  static QString shiftToStr(unsigned int txFreq, unsigned int rxFreq);

  static QString boolToStr(bool value);
};

#endif
