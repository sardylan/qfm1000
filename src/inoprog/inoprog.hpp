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


#ifndef __QFM1000__INOPROG__INOPROG__H
#define __QFM1000__INOPROG__INOPROG__H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtSerialPort/QSerialPort>

#include <utilities/service.hpp>

#define INOPROG_EEPROM_PAGE_SIZE 8
#define INOPROG_EEPROM_PAGE_COUNT 256

#define INOPROG_PROTOCOL_READY 'L'
#define INOPROG_PROTOCOL_OK 'K'
#define INOPROG_PROTOCOL_ERROR 'E'
#define INOPROG_PROTOCOL_READ 'R'
#define INOPROG_PROTOCOL_WRITE 'W'

using namespace qfm1000;

namespace qfm1000::inoprog {

    typedef quint8 PageNum;

    enum class InoProgError {
        INOPROG_ERROR_NO_ANSWER,
        INOPROG_ERROR_NOT_READY,
        INOPROG_ERROR_UNABLE_OPEN_SERIAL,
        INOPROG_ERROR_SERIAL_PORT,
        INOPROG_ERROR_PAGE
    };

    class InoProg : public utilities::Service {
    Q_OBJECT

    public:

        explicit InoProg(QObject *parent = nullptr);

        ~InoProg() override;

        [[nodiscard]] const QString &getPortName() const;

        void setPortName(const QString &newValue);

        [[nodiscard]] QSerialPort::BaudRate getPortSpeed() const;

        void setPortSpeed(QSerialPort::BaudRate newValue);

        static void registerMetaTypes();

    public slots:

        void start() override;

        void stop() override;

        QByteArray readEeprom();

        void writeEeprom(const QByteArray &data);

    private:

        QMutex mutex;

        QString portName;
        QSerialPort::BaudRate portSpeed;

        QSerialPort serialPort;
        bool ready;

        void connectSignals();

        bool isReady();

        void programmerStart();

        void programmerStop(bool sendSignal = true);

        void emitProgress(int value);

        void emitError(InoProgError inoProgError);

    private slots:

        void errorOccurred(QSerialPort::SerialPortError serialPortError);

        QByteArray readPage(qfm1000::inoprog::PageNum num);

        bool writePage(qfm1000::inoprog::PageNum num, const QByteArray &pageData);

    signals:

        void connected();

        void disconnected();

        void error(qfm1000::inoprog::InoProgError inoProgError);

        void pageRead(qfm1000::inoprog::PageNum num);

        void pageWritten(qfm1000::inoprog::PageNum num);

        void progress(int max, int value);

        void readCompleted();

        void writeCompleted();

    };

}

Q_DECLARE_METATYPE(qfm1000::inoprog::InoProgError)

#endif
