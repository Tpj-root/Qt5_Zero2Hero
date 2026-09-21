#include <QApplication>

#include "GridWidget.h"


int main(int argc, char *argv[])
{
    /*
        QApplication must be created before any QWidget.

        It manages:
        - GUI events
        - mouse events
        - keyboard events
        - window system
        - Qt event loop
    */

    QApplication app(argc, argv);


    /*
        Create our main application window.
    */

    GridWidget window;


    /*
        Read all command-line arguments.

        Example:

        ./app \
            --vertical 1mm 10 red \
            --horizontal 2mm 20 blue \
            --line-width 2
    */

    if (!window.parseArguments(argc, argv))
    {
        return 1;
    }


    /*
        Show the transparent grid window.
    */

    window.show();


    /*
        Start Qt event loop.

        This is required for:
        - mouse movement
        - window movement
        - repainting
        - close button
        - GUI events
    */

    return app.exec();
}