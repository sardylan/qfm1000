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


#ifndef __QFM1000__QFM1000__MAIN__H
#define __QFM1000__QFM1000__MAIN__H

#ifdef Q_OS_LINUX

#include <csignal>

void signalHandler(int signal);

#endif

#ifdef Q_OS_WINDOWS

#include <windows.h>

BOOL WINAPI ctrlHandler(DWORD ctrlHandler);

#endif

#include <QtCore/QObject>

#include "../eeprom/eeprom.hpp"
#include "../inoprog/inoprog.hpp"

using namespace qfm1000;

namespace qfm1000::app {

    class QFM1000 : public QObject {
    Q_OBJECT

    public:

        explicit QFM1000(QObject *parent = nullptr);

        ~QFM1000() override;

        void entryPoint();

        void start();

        void stop();

//    private:
//
//        eeprom::EEPROM *eeprom;

    signals:

        void finished();

    };

}

#endif
