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

#include <QLineEdit>

#include "flagdelegate.hpp"

FlagDelegate::FlagDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *
FlagDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto *checkBox = new QCheckBox(parent);
    checkBox->setStyleSheet("padding: 5px;");
    return checkBox;
}

void FlagDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *checkBox = dynamic_cast<QCheckBox *>(editor);
    QString stringValue = index.model()->data(index).toString();
    bool value = stringValue == "enabled";
    checkBox->setChecked(value);
}

void FlagDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *checkBox = dynamic_cast<QCheckBox *>(editor);
    bool data = checkBox->isChecked();
    model->setData(index, data);
}

void FlagDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
