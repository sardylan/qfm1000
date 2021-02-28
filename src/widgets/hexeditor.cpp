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
#include <QtCore/QtGlobal>
#include <QtCore/QChar>
#include <QtCore/QString>

#include <QtGui/QPainter>
#include <QtGui/QFontDatabase>
#include <QtGui/QPaintEvent>
#include <QtGui/QBrush>

#include "hexeditor.hpp"

using namespace qfm1000::widgets;

HexEditor::HexEditor(QWidget *parent) : QWidget(parent) {
    pageSize = HEXEDITOR_PAGESIZE_DEFAULT;

    marginTop = HEXEDITOR_MARGIN_TOP_DEFAULT;
    marginLeft = HEXEDITOR_MARGIN_LEFT_DEFAULT;

    cellSizeVertical = HEXEDITOR_CELL_SIZE_VERTICAL_DEFAULT;
    cellSizeHorizontal = HEXEDITOR_CELL_SIZE_HORIZONTAL_DEFAULT;

    fontSize = HEXEDITOR_FONT_SIZE_DEFAULT;

    rows = 0;
    addressesSize = 0;
    marginHex = 0;

    initSelectionMap();
}

HexEditor::~HexEditor() = default;

const QByteArray &HexEditor::getData() const {
    return data;
}

void HexEditor::setData(const QByteArray &newValue) {
    qInfo() << "Setting data";

    HexEditor::data = newValue;
    qDebug() << "Data size" << data.size();

    updateRows();
    updatePosSize();

    initSelectionMap();

    invokeUpdate();
}

int HexEditor::getPageSize() const {
    return pageSize;
}

void HexEditor::setPageSize(int newValue) {
    qInfo() << "Setting page size";

    HexEditor::pageSize = newValue;

    updateRows();

    invokeUpdate();
}

void HexEditor::setByte(int pos, const char &byte) {
    qInfo() << "Setting byte";

    if (pos >= HexEditor::data.size())
        return;

    HexEditor::data.replace(pos, 1, &byte);
}

void HexEditor::setByteSelected(int bytePosition, bool selected) {
    selectionMap.insert(bytePosition, selected);
    invokeUpdate();
}

void HexEditor::initSelectionMap() {
    selectionMap.clear();

    for (int i = 0; i < data.size(); i++)
        selectionMap.insert(i, false);
}

void HexEditor::updateRows() {
    rows = (data.size() / pageSize) + 1;
}

void HexEditor::updatePosSize() {
    addressesSize = QString::number(data.size(), 16).size();
    marginHex = (addressesSize + 1) * cellSizeHorizontal;
}

void HexEditor::invokeUpdate() {
    qInfo() << "Invoking widget update";

    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update), Qt::QueuedConnection);
}

void HexEditor::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.eraseRect(event->rect());

    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(fontSize);
    painter.setFont(font);

    setMinimumWidth(cellSizeHorizontal * 12);
    setMinimumHeight(cellSizeVertical * rows);

    for (int page = 0; page < rows; page++) {
        int offset = page * pageSize;

        paintPageAddresses(painter, page, offset);
        paintPageData(painter, page, offset);
    }

    painter.end();
}

void HexEditor::paintPageAddresses(QPainter &painter, int page, int offset) const {
    QBrush origBrush = painter.brush();
    QPen origPen = painter.pen();

    int x = marginLeft;
    int y = marginTop + (page * cellSizeVertical) + 1;

    int start = offset;
    int end = offset + (pageSize - 1) < data.size() ? offset + (pageSize - 1) : data.size();

    painter.setBrush(QBrush("#404d54"));
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawRect(QRect(0, (page * cellSizeVertical) - 1, marginHex, cellSizeVertical));

    QString text = QString("%1-%2")
            .arg(start, addressesSize, 16, QLatin1Char('0'))
            .arg(end, addressesSize, 16, QLatin1Char('0'))
            .toUpper();

    painter.setPen(QPen("#CCCCCC"));
    painter.drawText(x, y, text);

    painter.setBrush(origBrush);
    painter.setPen(origPen);
}

void HexEditor::paintPageData(QPainter &painter, int page, int offset) {
    QFont origFont = painter.font();
    QPen origPen = painter.pen();

    painter.setPen(QPen("#222222"));
    painter.setBackground(QBrush(Qt::red));

    QFont font = painter.font();
    font.setWeight(QFont::ExtraBold);
    painter.setFont(font);

    int y = marginTop + (page * cellSizeVertical) + 1;

    for (int col = 0; col < pageSize; col++) {
        int idx = offset + col;
        if (idx >= data.size())
            break;

        bool selected = selectionMap.value(idx, false);

        int x = marginLeft + marginHex + (col * cellSizeHorizontal);
        quint8 ch = data.at(idx);
        QString text = QString("%1")
                .arg(ch, 2, 16, QLatin1Char('0'))
                .toUpper();

        if (selected)
            painter.setBackgroundMode(Qt::OpaqueMode);

        painter.drawText(x, y, text);

        painter.setBackgroundMode(Qt::TransparentMode);
    }

    painter.setFont(origFont);
    painter.setPen(origPen);
}
