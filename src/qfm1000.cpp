/*
 * qFM1000
 * Copyright (C) 2017  Luca Cireddu
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

#include <QDebug>
#include <QMessageBox>

#include <config/manager.hpp>
#include <windows/configwindow.hpp>
#include <windows/aboutwindow.hpp>
#include <eeprom/filemanager.hpp>

#include "qfm1000.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    QFM1000 qFM1000(argc, argv);
    qFM1000.prepare();
    return qFM1000.run();
}

QFM1000::QFM1000(int &argc, char **argv) : QApplication(argc, argv) {
    status = Status::getInstance();
    config = Config::getInstance();
    eeprom = EEPROM::getInstance();

    mainWindow = new MainWindow();
}

QFM1000::~QFM1000() {
    delete mainWindow;
}

void QFM1000::prepare() {
    ConfigManager::load();
    ConfigManager::save();

    connect(mainWindow, SIGNAL(actionConfig()), this, SLOT(showConfigWindow()));
    connect(mainWindow, SIGNAL(actionAbout()), this, SLOT(showAboutWindow()));
    connect(mainWindow, SIGNAL(actionCloseFile()), this, SLOT(closeEepromFile()));
    connect(mainWindow, SIGNAL(actionLoadFile(QString)), this, SLOT(loadEepromFile(QString)));
    connect(mainWindow, SIGNAL(actionSaveFile(QString)), this, SLOT(saveEepromFile(QString)));
}

int QFM1000::run() {
    mainWindow->show();

    return QApplication::exec();
}

void QFM1000::showConfigWindow() {
    ConfigWindow configWindow;
    configWindow.exec();
}

void QFM1000::showAboutWindow() {
    AboutWindow aboutWindow;
    aboutWindow.exec();
}

void QFM1000::closeEepromFile() {
    if (status->isDataDirty(eeprom->getData())) {
        QMessageBox messageBox;
        messageBox.setText("Are you sure to close file without saving changes?");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int reply = messageBox.exec();

        switch (reply) {
            case QMessageBox::No:
                return;
            default:
                break;
        }
    }

    status->setCurrentFileName("");
    status->clearOriginalData();
    mainWindow->eepromUpdated();
}

void QFM1000::loadEepromFile(QString fileName) {
    FileManager::loadFromFile(eeprom, fileName);
    status->setCurrentFileName(fileName);
    status->setOriginalData(eeprom->getData());
    mainWindow->eepromUpdated();
}

void QFM1000::saveEepromFile(QString fileName) {
    FileManager::saveToFile(eeprom, fileName);
    status->setOriginalData(eeprom->getData());
    mainWindow->eepromUpdated();
}