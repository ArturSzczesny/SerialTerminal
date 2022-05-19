#include "serialterminal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialTerminal w;
    w.show();
    return a.exec();
}
