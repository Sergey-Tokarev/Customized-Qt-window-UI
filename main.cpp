#include "transparentmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gui::TransparentMainWindow w;
    w.show();

    return a.exec();
}
