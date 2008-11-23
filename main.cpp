#include <QtCore>
#include <QtGui>
#include "ftp.h"
#include "model.h"
#include "node.h"
#include "proxymodel.h"
#include "burgos.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Burgos b;
    b.show();

    return a.exec();
}
