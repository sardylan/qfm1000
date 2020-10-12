/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
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

#ifndef __QFM1000_WINDOWS_WIDGETS_STATUSBAR_H
#define __QFM1000_WINDOWS_WIDGETS_STATUSBAR_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtWidgets/QLabel>

#include <config.hpp>
#include <status.hpp>

using namespace qfm1000::eeprom;
using namespace qfm1000::config;

namespace qfm1000::windows {

    class StatusBarWidgets : public QObject {
    Q_OBJECT

    public:
        explicit StatusBarWidgets(QObject *parent = nullptr);

        ~StatusBarWidgets() override;

        QLabel *time;
        QLabel *arduinoSerial;

    public slots:

        void updateFromConfig();

    private:
        Status *status;
        Config *config;
        QTimer *timeTimer;

        void initLabelStyle(QLabel *label);

        void initTime();

        void initTimeTimer();

        static QString serialPortParams(QSerialPort::DataBits dataBits,
                                        QSerialPort::Parity parity,
                                        QSerialPort::StopBits stopBits);

    private slots:

        void updateTime();
    };

}

#endif