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


#ifndef __QFM1000__QFM1000__STATUS__H
#define __QFM1000__QFM1000__STATUS__H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <QtSerialPort/QSerialPort>

namespace qfm1000::app {

    class Status : public QObject {
    Q_OBJECT

    public:

        explicit Status(QObject *parent = nullptr);

        ~Status() override;

        [[nodiscard]] const QString &getSerialPortName() const;

        void setSerialPortName(const QString &newValue);

        [[nodiscard]] bool isSerialPortWorking() const;

        void setSerialPortWorking(bool newValue);

        [[nodiscard]] quint64 getInstanceId() const;

        void setInstanceId(quint64 newValue);

    private:

        QString serialPortName;
        bool serialPortWorking;

        quint64 instanceId;

    };

}

#endif
