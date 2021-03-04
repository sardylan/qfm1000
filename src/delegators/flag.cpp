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


#include <QtCore/QSize>

#include <QtWidgets/QCheckBox>

#include <eeprom/eeprom.hpp>

#include "flag.hpp"

using namespace qfm1000::delegators;

FlagDelegate::FlagDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

FlagDelegate::~FlagDelegate() = default;

QWidget *FlagDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto *checkBox = new QCheckBox(parent);
    checkBox->setStyleSheet("padding: 5px;");
    return checkBox;
}

void FlagDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *checkBox = dynamic_cast<QCheckBox *>(editor);
    auto value = index.model()->data(index, Qt::UserRole).value<eeprom::Flag>();
    checkBox->setChecked(value == eeprom::Flag::ENABLED);
}

void FlagDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto *checkBox = dynamic_cast<QCheckBox *>(editor);
    bool value = checkBox->isChecked();
    auto data = value ? eeprom::Flag::ENABLED : eeprom::Flag::DISABLED;
    model->setData(index, QVariant::fromValue(data));
}

void FlagDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QSize FlagDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const QSize &size = QStyledItemDelegate::sizeHint(option, index);
    return QSize(size.width() + 10, size.height());
}
