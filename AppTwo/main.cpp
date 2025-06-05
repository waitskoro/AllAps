#include "mainmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainManager w;

    return a.exec();
}
