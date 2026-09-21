#include <QApplication>
#include "GridWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GridWidget window;

    if (!window.parseArguments(argc, argv))
        return 1;

    window.show();

    return app.exec();
}