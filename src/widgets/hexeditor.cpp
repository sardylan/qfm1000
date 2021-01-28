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
#include <QtCore/QString>

#include <QtGui/QPainter>

#include "hexeditor.hpp"

using namespace qfm1000::widgets;

HexEditor::HexEditor(QWidget *parent) : QWidget(parent) {
    pageSize = HEXEDITOR_PAGESIZE_DEFAULT;

    marginTop = HEXEDITOR_MARGIN_TOP_DEFAULT;
    marginLeft = HEXEDITOR_MARGIN_LEFT_DEFAULT;

    cellSizeVertical = HEXEDITOR_CELL_SIZE_VERTICAL_DEFAULT;
    cellSizeHorizontal = HEXEDITOR_CELL_SIZE_HORIZONTAL_DEFAULT;

    fontSize = HEXEDITOR_FONT_SIZE_DEFAULT;
}

HexEditor::~HexEditor() = default;

const QByteArray &HexEditor::getData() const {
    return data;
}

void HexEditor::setData(const QByteArray &newValue) {
    qInfo() << "Setting data";

    HexEditor::data = newValue;
    invokeUpdate();
}

int HexEditor::getPageSize() const {
    return pageSize;
}

void HexEditor::setPageSize(int newValue) {
    qInfo() << "Setting page size";

    HexEditor::pageSize = newValue;
    invokeUpdate();
}

void HexEditor::setByte(int pos, const char &byte) {
    qInfo() << "Setting byte";

    if (pos >= HexEditor::data.size())
        return;

    HexEditor::data.replace(pos, 1, &byte);
}

void HexEditor::invokeUpdate() {
    qInfo() << "Invoking widget update";

    QMetaObject::invokeMethod(this, qOverload<>(&QWidget::update), Qt::QueuedConnection);
}

void HexEditor::paintEvent(QPaintEvent *event) {
    qInfo() << "Paint event";

    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font = painter.font();
    font.setFamily("monospace");
    font.setPointSize(fontSize);
    painter.setFont(font);

    int rows = (data.size() / pageSize) + 1;
    int posSize = QString::number(data.size(), 16).size();

    int marginHex = (posSize + 1) * cellSizeHorizontal;

    setMinimumWidth(cellSizeHorizontal * 12);
    setMinimumHeight(cellSizeVertical * rows);

    for (int row = 0; row < rows; row++) {
        int offset = row * pageSize;

        paintPositions(painter, posSize, row, offset);
        paintHexData(painter, marginHex, row, offset);
    }

    painter.end();
}

void HexEditor::paintHexData(QPainter &painter, int marginHex, int row, int offset) {
    for (int col = 0; col < pageSize; col++) {
        int idx = offset + col;
        if (idx >= data.size())
            break;

        int x = marginLeft + marginHex + (col * cellSizeHorizontal);
        int y = marginTop + (row * cellSizeVertical);
        QString text = QString::number(data[idx], 16).toUpper();
        painter.drawText(x, y, text);
    }
}

void HexEditor::paintPositions(QPainter &painter, int posSize, int row, int offset) const {
    int x = marginLeft;
    int y = marginTop + (row * cellSizeVertical);

    int start = offset;
    int end = offset + (pageSize - 1) < data.size() ? offset + (pageSize - 1) : data.size();

    QString text = QString("%1-%2")
            .arg(start, posSize, 16, QLatin1Char('0'))
            .arg(end, posSize, 16, QLatin1Char('0'))
            .toUpper();

    painter.drawText(x, y, text);
}
