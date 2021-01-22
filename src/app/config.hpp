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

            [[nodiscard]] QSerialPort::BaudRate getPortSpeed() const;

            void setPortSpeed(QSerialPort::BaudRate newValue);

        private:

            QString portName;
            QSerialPort::BaudRate portSpeed;

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
