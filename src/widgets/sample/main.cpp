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
    scrollArea.show();

    QMetaObject::invokeMethod(&hexEditor, "setData", Qt::QueuedConnection, Q_ARG(QByteArray, QByteArray(
            "\x00\x01\x02\x03\x5ciaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociaociao")));

    return QApplication::exec();
}
