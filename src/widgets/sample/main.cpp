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
#include <QtCore/QByteArray>

#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>

#include "../hexeditor.hpp"

int main(int argc, char **argv) {
    QApplication application(argc, argv);

    qfm1000::widgets::HexEditor hexEditor;

    QScrollArea scrollArea;
    scrollArea.setWidget(&hexEditor);
    scrollArea.setWidgetResizable(true);
    scrollArea.setFixedWidth(320);
    scrollArea.setFixedHeight(160);
    scrollArea.show();

    QByteArray data = QByteArray(
            "\x00\x01\x02\x03\x04\xa6\x3d\xa2"
            "\x34\xb2\x1e\x48\xeb\x26\x78\x3c"
            "\x68\x6a\xa6\xb2\xf2\xb9\x8b\xf2"
            "\x31\x7a\x1b\xd2\x56\x6b\xc1\x4f"
            "\x2e\x14\x3e\x2b\xeb\x46\xa7\x45"
            "\xd2\x1c\x3c\x0a\xb1\xbe\x1b\x33"
            "\xa7\xd5\x70\x98\x94\x7c\xf6\x22",
            56
    );

    QMetaObject::invokeMethod(&hexEditor, "setData", Qt::QueuedConnection, Q_ARG(QByteArray, data));

    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 3), Q_ARG(bool, true));
    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 7), Q_ARG(bool, true));
    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 3), Q_ARG(bool, false));
    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 14), Q_ARG(bool, true));
    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 7), Q_ARG(bool, false));
    QMetaObject::invokeMethod(&hexEditor, "setByteSelected", Qt::QueuedConnection, Q_ARG(int, 14), Q_ARG(bool, false));

    return QApplication::exec();
}
