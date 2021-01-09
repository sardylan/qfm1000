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


#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QFile>

#include <QtWidgets/QMessageBox>

#include "main.hpp"
#include "version.hpp"

#include "../eeprom/filemanager.hpp"

using namespace qfm1000::app;

qfm1000::app::QFM1000 *mainApplication;

#ifdef Q_OS_LINUX

void signalHandler(int signal) {
    switch (signal) {
        case SIGINT:
        case SIGTERM:
            mainApplication->stop();
            break;

        default:
            return;
    }
}

#endif

#ifdef Q_OS_WINDOWS

BOOL WINAPI ctrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
            audioReceiver->stop();
            return TRUE;

        default:
            return FALSE;
    }
}

#endif


int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);

    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    qSetMessagePattern("\x1b[94;1m[\x1b[96;1m%{time yyyy-MM-dd hh:mm:ss.zzz}\x1b[94;1m]\x1b[39;0m "
                       "PID:\x1b[31m%{pid}\x1b[39m "
                       "TID:\x1b[91m%{threadid}\x1b[39m "
                       "["
                       "%{if-debug}\x1b[37m DEBUG  \x1b[39m%{endif}"
                       "%{if-info}\x1b[92m INFO   \x1b[39m%{endif}"
                       "%{if-warning}\x1b[93mWARNING \x1b[39m%{endif}"
                       "%{if-critical}\x1b[91mCRITICAL\x1b[39m%{endif}"
                       "%{if-fatal}\x1b[91;5m FATAL  \x1b[39;25m%{endif}"
                       "]: "
                       "%{file}:%{line} "
                       "[\x1b[97m%{function}()\x1b[39m] "
                       "%{message}");

    QApplication application(argc, argv);

    mainApplication = new QFM1000();
    QApplication::connect(mainApplication, &QFM1000::finished, []() { QCoreApplication::exit(); });
    mainApplication->entryPoint();

#ifdef Q_OS_LINUX
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif

#ifdef Q_OS_WINDOWS
    if (SetConsoleCtrlHandler(ctrlHandler, TRUE) != TRUE)
        qWarning() << "Unable to set Control Handler";
#endif

    return QApplication::exec();
}

QFM1000::QFM1000(QObject *parent) : QObject(parent) {

}

QFM1000::~QFM1000() {

}

void QFM1000::entryPoint() {
    start();
}

void QFM1000::start() {

}

void QFM1000::stop() {

}

void QFM1000::fileNew() {
//    if (status->isFileOpened() && status->isDataDirty(eeprom->getData())) {
//        QMessageBox messageBox;
//        messageBox.setText("Are you sure to create a new EEPROM file without saving changes?");
//        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//        int reply = messageBox.exec();
//
//        switch (reply) {
//            case QMessageBox::No:
//                return;
//            default:
//                break;
//        }
//    }

    QFile file(":/files/eeprom/new");
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    eeprom->setData(data);

    status->setEepromLoaded(true);
    status->setCurrentFileName("");
    status->clearOriginalData();

    QMetaObject::invokeMethod(mainWindow, "eepromUpdated");
}

void QFM1000::fileLoad() {
    eeprom::FileManager::loadFromFile(eeprom, );
}

void QFM1000::fileSave() {

}

void QFM1000::programmerRead() {

}

void QFM1000::programmerWrite() {

}

//QFM1000::QFM1000(int &argc, char **argv) : QApplication(argc, argv) {
//    status = Status::getInstance();
//    config = Config::getInstance();
//
//    eeprom = EEPROM::getInstance();
//
//    mainWindow = new MainWindow();
//}
//
//QFM1000::~QFM1000() {
//    delete mainWindow;
//}
//
//void QFM1000::prepare() {
//    ConfigManager::load();
//    ConfigManager::save();
//
//    connect(mainWindow, &MainWindow::actionConfig, this, &QFM1000::showConfigWindow);
//    connect(mainWindow, &MainWindow::actionAbout, this, &QFM1000::showAboutWindow);
//
//    connect(mainWindow, &MainWindow::actionNewFile, this, &QFM1000::newEepromFile);
//    connect(mainWindow, &MainWindow::actionCloseFile, this, &QFM1000::closeEepromFile);
//    connect(mainWindow, &MainWindow::actionLoadFile, this, &QFM1000::loadEepromFile);
//    connect(mainWindow, &MainWindow::actionSaveFile, this, &QFM1000::saveEepromFile);
//
//    connect(mainWindow, &MainWindow::actionEepromRead, this, &QFM1000::readArduinoEeprom);
//    connect(mainWindow, &MainWindow::actionEepromWrite, this, &QFM1000::writeArduinoEeprom);
//}
//
//int QFM1000::run() {
//    mainWindow->show();
//
//    return QApplication::exec();
//}
//
//void QFM1000::showConfigWindow() {
//    ConfigWindow configWindow;
//    configWindow.exec();
//
//    QMetaObject::invokeMethod(mainWindow, "configUpdated");
//}
//
//void QFM1000::showAboutWindow() {
//    AboutWindow aboutWindow;
//    aboutWindow.exec();
//}
//
//void QFM1000::closeEepromFile() {
//    if (status->isDataDirty(eeprom->getData())) {
//        QMessageBox messageBox;
//        messageBox.setText("Are you sure to close file without saving changes?");
//        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//        int reply = messageBox.exec();
//
//        switch (reply) {
//            case QMessageBox::No:
//                return;
//            default:
//                break;
//        }
//    }
//
//    status->setEepromLoaded(false);
//    status->setCurrentFileName("");
//    status->clearOriginalData();
//
//    QMetaObject::invokeMethod(mainWindow, "eepromUpdated");
//}
//
//void QFM1000::loadEepromFile(QString fileName) {
//    bool result = FileManager::loadFromFile(eeprom, fileName);
//    if (!result) {
//        QMessageBox messageBox;
//        messageBox.setText("Error loading file");
//        messageBox.setStandardButtons(QMessageBox::Close);
//        messageBox.exec();
//        return;
//    }
//
//    status->setEepromLoaded(true);
//    status->setCurrentFileName(fileName);
//    status->setOriginalData(eeprom->getData());
//
//    QMetaObject::invokeMethod(mainWindow, "eepromUpdated");
//}
//
//void QFM1000::saveEepromFile(QString fileName) {
//    bool result = FileManager::saveToFile(eeprom, fileName);
//    if (!result) {
//        QMessageBox messageBox;
//        messageBox.setText("Error saving file");
//        messageBox.setStandardButtons(QMessageBox::Close);
//        messageBox.exec();
//        return;
//    }
//
//    status->setEepromLoaded(true);
//    status->setCurrentFileName(fileName);
//    status->setOriginalData(eeprom->getData());
//
//    QMetaObject::invokeMethod(mainWindow, "eepromUpdated");
//}
//
//void QFM1000::readArduinoEeprom() {
//    auto *window = new ArduinoWindow();
//    auto *programmer = new ArduinoProgrammer();
//    auto *thread = new QThread();
//
//    programmer->moveToThread(thread);
//    thread->start();
//
//    connect(window, &ArduinoWindow::finished, [=]() {
//        thread->quit();
//        thread->wait();
//
//        window->deleteLater();
//        programmer->deleteLater();
//        thread->deleteLater();
//    });
//
//    connect(programmer, &ArduinoProgrammer::connected, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Connected"));
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Reading EEPROM..."));
//        QMetaObject::invokeMethod(programmer, "read", Qt::QueuedConnection);
//    });
//
//    connect(programmer, &ArduinoProgrammer::disconnected, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Disconnected"));
//        QMetaObject::invokeMethod(window, "finish", Qt::QueuedConnection);
//    });
//
//    connect(programmer, &ArduinoProgrammer::pageRead, [=](uint8_t num) {
//        QMetaObject::invokeMethod(window, "progress", Qt::QueuedConnection, Q_ARG(int, 256), Q_ARG(int, num));
//    });
//
//    connect(programmer, &ArduinoProgrammer::readCompleted, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Read completed"));
//        QMetaObject::invokeMethod(window, "finish", Qt::QueuedConnection);
//        QMetaObject::invokeMethod(programmer, "close", Qt::QueuedConnection);
//    });
//
//    connect(programmer, &ArduinoProgrammer::eepromRead, [=](QByteArray data) {
//        eeprom->setData(data);
//        status->setEepromLoaded(true);
//        status->setCurrentFileName("");
//        status->setOriginalData(eeprom->getData());
//        QMetaObject::invokeMethod(mainWindow, "eepromUpdated", Qt::QueuedConnection);
//    });
//
//    QMetaObject::invokeMethod(window, "start", Qt::QueuedConnection);
//    QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Starting Arduino programmer"));
//
//    QMetaObject::invokeMethod(programmer, "init", Qt::QueuedConnection,
//                              Q_ARG(QString, config->getArduinoPortName()),
//                              Q_ARG(QSerialPort::BaudRate, config->getArduinoPortSpeed()));
//
//    window->exec();
//}
//
//void QFM1000::writeArduinoEeprom() {
//    auto *window = new ArduinoWindow();
//    auto *programmer = new ArduinoProgrammer();
//    auto *thread = new QThread();
//
//    programmer->moveToThread(thread);
//    thread->start();
//
//    connect(window, &ArduinoWindow::finished, [=]() {
//        thread->quit();
//        thread->wait();
//
//        window->deleteLater();
//        programmer->deleteLater();
//        thread->deleteLater();
//    });
//
//    connect(programmer, &ArduinoProgrammer::connected, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Connected"));
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Writing EEPROM..."));
//        QMetaObject::invokeMethod(programmer, "write", Qt::QueuedConnection, Q_ARG(QByteArray, eeprom->getData()));
//    });
//
//    connect(programmer, &ArduinoProgrammer::disconnected, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Disconnected"));
//        QMetaObject::invokeMethod(window, "finish", Qt::QueuedConnection);
//    });
//
//    connect(programmer, &ArduinoProgrammer::pageWritten, [=](uint8_t num) {
//        QMetaObject::invokeMethod(window, "progress", Qt::QueuedConnection, Q_ARG(int, 256), Q_ARG(int, num));
//    });
//
//    connect(programmer, &ArduinoProgrammer::writeCompleted, [=]() {
//        QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Write completed"));
//        QMetaObject::invokeMethod(window, "finish", Qt::QueuedConnection);
//        QMetaObject::invokeMethod(programmer, "close", Qt::QueuedConnection);
//    });
//
//    QMetaObject::invokeMethod(window, "start", Qt::QueuedConnection);
//    QMetaObject::invokeMethod(window, "log", Qt::QueuedConnection, Q_ARG(QString, "Starting Arduino programmer"));
//
//    QMetaObject::invokeMethod(programmer, "init", Qt::QueuedConnection,
//                              Q_ARG(QString, config->getArduinoPortName()),
//                              Q_ARG(QSerialPort::BaudRate, config->getArduinoPortSpeed()));
//
//    window->exec();
//}
