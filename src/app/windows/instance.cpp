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
#include <QtCore/QModelIndex>

#include <QtWidgets/QComboBox>

#include <eeprom/eeprom.hpp>
#include <eeprom/model.hpp>

#include <delegators/readonly.hpp>
#include <delegators/flag.hpp>
#include <delegators/ctcss.hpp>
#include <delegators/power.hpp>
#include <delegators/squelch.hpp>

#include "instance.hpp"
#include "ui_instance.h"

using namespace qfm1000::app::windows;

Instance::Instance(eeprom::TableModel *model, QWidget *parent) : QWidget(parent), ui(new Ui::Instance) {
    ui->setupUi(this);

    Instance::model = model;

    hexEditor = new widgets::HexEditor();

    initUi();
    connectSignals();
}

Instance::~Instance() {
    delete hexEditor;

    delete ui;
}

bool Instance::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->defaultChannelComboBox) {
        int firstAffectedByte = model->getEeprom()->firstAffectedByte(eeprom::Param::PARAM_STARTUP_CHANNEL);
        parseEvent(firstAffectedByte, event);
    } else if (watched == ui->lowPowerComboBox) {
        int firstAffectedByte = model->getEeprom()->firstAffectedByte(eeprom::Param::PARAM_LOW_POWER);
        parseEvent(firstAffectedByte, event);
    } else if (watched == ui->totSlider) {
        int firstAffectedByte = model->getEeprom()->firstAffectedByte(eeprom::Param::PARAM_TOT);
        parseEvent(firstAffectedByte, event);
    } else if (watched == ui->totSpinBox) {
        int firstAffectedByte = model->getEeprom()->firstAffectedByte(eeprom::Param::PARAM_TOT);
        parseEvent(firstAffectedByte, event);
    } else if (watched == ui->beepCheckBox) {
        int firstAffectedByte = model->getEeprom()->firstAffectedByte(eeprom::Param::PARAM_KEY_BEEP);
        parseEvent(firstAffectedByte, event);
    } else if (watched == ui->channelsTableView) {
        switch (event->type()) {
            case QEvent::FocusOut:
                QMetaObject::invokeMethod(ui->channelsTableView->selectionModel(), "clear", Qt::QueuedConnection);
                break;
            default:
                break;
        }
    }

    return QObject::eventFilter(watched, event);
}

void Instance::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->channelsTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &Instance::channelSelectionChanged);

    ui->defaultChannelComboBox->installEventFilter(this);
    ui->lowPowerComboBox->installEventFilter(this);
    ui->totSlider->installEventFilter(this);
    ui->totSpinBox->installEventFilter(this);
    ui->beepCheckBox->installEventFilter(this);
    ui->channelsTableView->installEventFilter(this);
}

void Instance::initUi() {
    qInfo() << "Initalizing UI";

    ui->channelsTableView->setModel(Instance::model);

    ui->channelsTableView->setItemDelegateForColumn(2, new delegators::ReadOnlyDelegate(this));
    ui->channelsTableView->setItemDelegateForColumn(3, new delegators::CTCSSDelegate(this));
    ui->channelsTableView->setItemDelegateForColumn(4, new delegators::CTCSSDelegate(this));

    ui->channelsTableView->setItemDelegateForColumn(5, new delegators::PowerDelegate(this));
    ui->channelsTableView->setItemDelegateForColumn(6, new delegators::SquelchDelegate(this));
    ui->channelsTableView->setItemDelegateForColumn(7, new delegators::FlagDelegate(this));
    ui->channelsTableView->setItemDelegateForColumn(8, new delegators::FlagDelegate(this));

    ui->channelsTableView->resizeColumnsToContents();

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
    for (QModelIndex modelIndex: deselected.indexes()) {
        int column = modelIndex.column();
        eeprom::Param param = eeprom::TableModel::columnToParam(column);
        if (param == eeprom::Param::NONE)
            continue;

        int channel = modelIndex.row();

        int firstAffectedByte = model->getEeprom()->firstAffectedByte(param, channel);
        int byteSize = eeprom::EEPROM::byteSize(param);

        qDebug() << "Deelected" << param << "for channel" << channel
                 << " - "
                 << byteSize << "bytes from" << firstAffectedByte;

        for (int i = 0; i < byteSize; i++)
            QMetaObject::invokeMethod(hexEditor, "setByteSelected", Qt::QueuedConnection,
                                      Q_ARG(int, firstAffectedByte + i), Q_ARG(bool, false));
    }

    for (QModelIndex modelIndex: selected.indexes()) {
        int column = modelIndex.column();
        eeprom::Param param = eeprom::TableModel::columnToParam(column);
        if (param == eeprom::Param::NONE)
            continue;

        int channel = modelIndex.row();

        int firstAffectedByte = model->getEeprom()->firstAffectedByte(param, channel);
        int byteSize = eeprom::EEPROM::byteSize(param);

        qDebug() << "Selected" << param << "for channel" << channel
                 << " - "
                 << byteSize << "bytes from" << firstAffectedByte;

        for (int i = 0; i < byteSize; i++)
            QMetaObject::invokeMethod(hexEditor, "setByteSelected", Qt::QueuedConnection,
                                      Q_ARG(int, firstAffectedByte + i), Q_ARG(bool, true));
    }
}

void Instance::parseEvent(int bytePosition, QEvent *event) {
    switch (event->type()) {
        case QEvent::FocusIn:
            setByteSelected(bytePosition, true);

            break;
        case QEvent::FocusOut:
            setByteSelected(bytePosition, false);

            break;
        default:
            break;
    }

}

void Instance::setByteSelected(int bytePosition, bool selected) {
    QMetaObject::invokeMethod(hexEditor, "setByteSelected", Qt::QueuedConnection,
                              Q_ARG(int, bytePosition), Q_ARG(bool, selected));

}
