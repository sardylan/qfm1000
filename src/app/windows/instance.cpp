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
#include <QtCore/QItemSelectionModel>

#include <QtGui/QFont>
#include <QtGui/QFontDatabase>

#include "instance.hpp"
#include "ui_instance.h"

using namespace qfm1000::app::windows;

Instance::Instance(QAbstractItemModel *model, QWidget *parent) : QWidget(parent), ui(new Ui::Instance) {
    ui->setupUi(this);

    ui->channelsTableView->setModel(model);

    hexEditor = new widgets::HexEditor();

    connectSignals();
    initUi();
}

Instance::~Instance() {
    delete hexEditor;

    delete ui;
}

void Instance::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->channelsTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Instance::channelSelectionChanged);
}

void Instance::initUi() {
    qInfo() << "Initalizing UI";

    ui->hexDataScrollArea->setWidget(hexEditor);
    ui->hexDataScrollArea->setWidgetResizable(true);
}

void Instance::updateFileName(const QString &filename) {
    qInfo() << "Updating filename";

    if (filename.size() == 0) {
        setWindowTitle("<not saved>");
        return;
    }

    setWindowTitle(filename);
}

void Instance::dataUpdated(const QByteArray &value) {
    qInfo() << "Data updated";

    QMetaObject::invokeMethod(
            hexEditor,
            "setData",
            Qt::QueuedConnection,
            Q_ARG(QByteArray, value)
    );
}

void Instance::byteUpdated(int pos, quint8 value) {
    qInfo() << "Byte updated";

    QMetaObject::invokeMethod(
            hexEditor,
            "setByte",
            Qt::QueuedConnection,
            Q_ARG(int, pos),
            Q_ARG(char, value)
    );
}

void Instance::channelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    qInfo() << "Item selected:" << selected
            << "Item deselected:" << deselected;
}
