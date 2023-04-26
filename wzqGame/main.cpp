#include "wzqwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wzqWidget w;
    w.show();
    return a.exec();
}
