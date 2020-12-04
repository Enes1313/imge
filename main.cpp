#include "imge.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Imge w;
    w.show();
    return a.exec();
}
