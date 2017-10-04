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

#include "squelchdelegate.hpp"
#include "helper.hpp"

SquelchDelegate::SquelchDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *
SquelchDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QComboBox *comboBox = createSquelchComboBox(parent);
    return comboBox;
}

void SquelchDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    QString stringValue = index.model()->data(index).toString();

    for (int i = 0; i < powerValues.size(); i++)
        if (stringValue == powerValues[i]) {
            comboBox->setCurrentIndex(i);
            return;
        }

    comboBox->setCurrentIndex(0);
}

void SquelchDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    int data = comboBox->currentData().toInt();
    model->setData(index, data);
}

void SquelchDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

QComboBox *SquelchDelegate::createSquelchComboBox(QWidget *parent) {
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->setEditable(true);
    comboBox->lineEdit()->setReadOnly(true);
    comboBox->lineEdit()->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < powerValues.size(); i++) {
        comboBox->addItem(squelchValues[i], i);
        comboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    return comboBox;
}
