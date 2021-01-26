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


#ifndef __QFM1000__QFM1000__CONFIG__H
#define __QFM1000__QFM1000__CONFIG__H

#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>

namespace qfm1000::app {

    class Config {

    public:

        class InoProg {

        public:

            [[nodiscard]] const QString &getPortName() const;

            void setPortName(const QString &newValue);

            [[nodiscard]] QSerialPort::BaudRate getBaudRate() const;

            void setBaudRate(QSerialPort::BaudRate newValue);

            [[nodiscard]] QSerialPort::DataBits getDataBits() const;

            void setDataBits(QSerialPort::DataBits newValue);

            [[nodiscard]] QSerialPort::Parity getParity() const;

            void setParity(QSerialPort::Parity newValue);

            [[nodiscard]] QSerialPort::StopBits getStopBits() const;

            void setStopBits(QSerialPort::StopBits newValue);

            [[nodiscard]] QSerialPort::FlowControl getFlowControl() const;

            void setFlowControl(QSerialPort::FlowControl newValue);

        private:

            QString portName;
            QSerialPort::BaudRate baudRate;
            QSerialPort::DataBits dataBits;
            QSerialPort::Parity parity;
            QSerialPort::StopBits stopBits;
            QSerialPort::FlowControl flowControl;

        };

        Config();

        ~Config();

        void update(Config *config);

        [[nodiscard]] InoProg *getInoProg() const;

        void load();

        void save();

        static void registerMetaTypes();

    private:

        InoProg *inoProg;

    };

}

Q_DECLARE_METATYPE(qfm1000::app::Config)

#endif
