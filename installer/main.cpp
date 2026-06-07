#include "mainwindow.h"

#include <QApplication>
#include <QStyle>
#include <QStyleHints>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPalette pal = qApp->palette();
    bool dark = pal.color(QPalette::Window).lightness() < 128;
    if (dark) {
        qApp->setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: #eaeaea;
        }
    )");
        std::cerr << "a\n";
    } else {
        qApp->setStyleSheet("");
    }

    MainWindow w;
    w.setWindowTitle("YASOS INSTALLER");
    w.show();
    return QCoreApplication::exec();
}
