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
#include <QtCore/QFile>

#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>

#include <QtSerialPort/QSerialPort>

#include <eeprom/filemanager.hpp>

#include "qfm1000.hpp"

#include "windows/about.hpp"
#include "windows/license.hpp"
#include "windows/config.hpp"


using namespace qfm1000::app;

QFM1000::QFM1000(QObject *parent) : QObject(parent) {
    status = new Status();
    config = new Config();

    mainWindow = new windows::Main();

    counter = 0;
    instances = new QMap<quint64, Instance *>();
}

QFM1000::~QFM1000() {
    for (Instance *instance: instances->values())
        delete instance;

    delete instances;
    delete mainWindow;
    delete config;
    delete status;
}

void QFM1000::entryPoint() {
    qInfo() << "Entrypoint";

    start();
}

void QFM1000::start() {
    qInfo() << "Start";

    qDebug() << "Loading fonts";
    QFontDatabase::addApplicationFont(":/fonts/RobotoMono-VariableFont_wght.ttf");
    QFontDatabase::addApplicationFont(":/fonts/RobotoMono-Italic-VariableFont_wght.ttf");

    qDebug() << "Loading config";
    config->load();
    config->save();

    updateMainWindowFromConfig();

    connectSignals();

    qDebug() << "Displaying main window";
    mainWindow->show();
}

void QFM1000::connectSignals() const {
    qInfo() << "Connecting signals";

    connect(mainWindow, &windows::Main::displayAbout, this, &QFM1000::displayAbout);
    connect(mainWindow, &windows::Main::displayLicense, this, &QFM1000::displayLicense);

    connect(mainWindow, &windows::Main::actionConfiguration, this, &QFM1000::actionConfiguration);

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

        qDebug() << "Loading data from file";
        if (!eeprom::FileManager::loadFromFile(instance->getEeprom(), selectedFile)) {
            qWarning() << "Unable to load EEPROM from" << selectedFile;
            delete instance;
            continue;
        }

        qDebug() << "Setting instance params";
        instance->setFileName(selectedFile);
        instance->resetStatus();

        qDebug() << "Adding instance in instances map";
        instances->insert(instance->getId(), instance);

        qDebug() << "Adding instance to window";
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

    qDebug() << "Creating About window";
    auto *aboutWindow = new windows::About();

    qDebug() << "Connecting signals";
    connect(aboutWindow, &QDialog::finished, aboutWindow, &QObject::deleteLater, Qt::QueuedConnection);

    qDebug() << "Setting params";
    aboutWindow->setWindowModality(Qt::ApplicationModal);

    qDebug() << "Displaying dialog";
    QMetaObject::invokeMethod(aboutWindow, &QDialog::exec, Qt::QueuedConnection);
}

void QFM1000::displayLicense() {
    qInfo() << "Displaying license";

    qDebug() << "Creating License window";
    auto *licenseWindow = new windows::License();

    qDebug() << "Connecting signals";
    connect(licenseWindow, &QDialog::finished, licenseWindow, &QObject::deleteLater, Qt::QueuedConnection);

    qDebug() << "Setting params";
    licenseWindow->setWindowModality(Qt::ApplicationModal);

    qDebug() << "Displaying dialog";
    QMetaObject::invokeMethod(licenseWindow, &QDialog::exec, Qt::QueuedConnection);
}

void QFM1000::actionConfiguration() {
    qInfo() << "Action Configuration";

    qDebug() << "Creating Configuration window dialog";
    auto *configWindow = new windows::Config();

    qDebug() << "Connecting signals";
    connect(configWindow, &QDialog::finished, configWindow, &QObject::deleteLater, Qt::QueuedConnection);
    connect(configWindow, &windows::Config::updateConfig, this, &QFM1000::updateConfig, Qt::QueuedConnection);

    qDebug() << "Setting params";
    configWindow->setWindowModality(Qt::ApplicationModal);
    configWindow->setCurrentConfig(QFM1000::config);

    qDebug() << "Displaying dialog";
    QMetaObject::invokeMethod(configWindow, &QDialog::exec, Qt::QueuedConnection);
}

void QFM1000::updateConfig(Config *newConfig) {
    qInfo() << "Updating config";

    qDebug() << "Updating config from new config";
    QFM1000::config->update(newConfig);

    updateMainWindowFromConfig();

    qDebug() << "Saving config";
    QFM1000::config->save();
}

void QFM1000::updateMainWindowFromConfig() {
    qInfo() << "Updating main window from config";

    QMetaObject::invokeMethod(
            mainWindow,
            "updateSerialPortLabel",
            Qt::QueuedConnection,
            Q_ARG(QString, config->getInoProg()->getPortName()),
            Q_ARG(QSerialPort::BaudRate, config->getInoProg()->getBaudRate()),
            Q_ARG(QSerialPort::DataBits, config->getInoProg()->getDataBits()),
            Q_ARG(QSerialPort::Parity, config->getInoProg()->getParity()),
            Q_ARG(QSerialPort::StopBits, config->getInoProg()->getStopBits()),
            Q_ARG(QSerialPort::FlowControl, config->getInoProg()->getFlowControl())
    );
}
