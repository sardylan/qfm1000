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
#include <QtCore/QCoreApplication>

#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include "app.hpp"
#include "version.hpp"

#include "windows/about.hpp"

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
            mainApplication->stop();
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

    qfm1000::eeprom::EEPROM::registerMetaTypes();
    qfm1000::inoprog::InoProg::registerMetaTypes();

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
    mainWindow = new windows::Main();

    counter = 0;
    instances = new QMap<quint64, Instance *>();
}

QFM1000::~QFM1000() {
    delete instances;
    delete mainWindow;
}

void QFM1000::entryPoint() {
    qInfo() << "Entrypoint";

    start();
}

void QFM1000::start() {
    qInfo() << "Start";

    QFontDatabase::addApplicationFont(":/fonts/RobotoMono-VariableFont_wght.ttf");
    QFontDatabase::addApplicationFont(":/fonts/RobotoMono-Italic-VariableFont_wght.ttf");

    connectSignals();

    mainWindow->show();
}

void QFM1000::connectSignals() const {
    qInfo() << "Connecting signals";

    connect(mainWindow, &windows::Main::displayAbout, this, &QFM1000::displayAbout);

    connect(mainWindow, &windows::Main::actionFileOpen, this, &QFM1000::actionFileOpen);
}

void QFM1000::stop() {
    qInfo() << "Stop";

    mainWindow->close();
}

void QFM1000::actionFileOpen() {
    qInfo() << "Action File Open";

    QFileDialog fileDialog(mainWindow, Qt::Dialog);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setNameFilters(EEPROM_FILES_NAME_FILTERS);
    fileDialog.exec();

    QStringList selectedFiles = fileDialog.selectedFiles();
    qDebug() << "Files to open:" << selectedFiles;

    for (const QString &selectedFile:selectedFiles) {
        qDebug() << "Loading" << selectedFile;

        quint64 id = counter;
        counter++;

        qDebug() << "Generating new Instance with ID" << id;

        auto *instance = new Instance(id);

        if (!eeprom::FileManager::loadFromFile(instance->getEeprom(), selectedFile)) {
            qWarning() << "Unable to load EEPROM from" << selectedFile;
            delete instance;
            continue;
        }

        instance->setFileName(selectedFile);
        instance->resetStatus();

        instances->insert(instance->getId(), instance);

        QMetaObject::invokeMethod(
                mainWindow,
                "addInstance",
                Qt::QueuedConnection,
                Q_ARG(quint64, id),
                Q_ARG(qfm1000::app::windows::Instance*, instance->getWindow())
        );
    }
}

void QFM1000::displayAbout() {
    qInfo() << "Displaying about";

    windows::About aboutWindow;
    aboutWindow.setWindowModality(Qt::ApplicationModal);
    aboutWindow.exec();
}
