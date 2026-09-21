#include "GridWidget.h"

#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <QDebug>

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent),
      verticalMM(1.0),
      horizontalMM(1.0),
      verticalLines(10),
      horizontalLines(20),
      lineColor(Qt::red)
{
    /*
        Transparent window.

        WA_TranslucentBackground tells Qt that the
        window background itself should be transparent.
    */
    setAttribute(Qt::WA_TranslucentBackground);

    /*
        Remove normal window decorations.

        No:
        - title bar
        - minimize button
        - maximize button
        - close button
        - frame
    */
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint);

    /*
        We only draw lines.
        There is no toolbar or status bar because
        this application uses QWidget directly.
    */
    setMinimumSize(100, 100);
}


bool GridWidget::parseArguments(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        QString argument = argv[i];

        // --------------------------------------------
        // --vertical 1mm
        // --------------------------------------------
        if (argument == "--vertical")
        {
            if (i + 1 >= argc)
                return false;

            QString value = argv[++i];

            value.remove("mm");

            bool ok = false;
            verticalMM = value.toDouble(&ok);

            if (!ok || verticalMM <= 0)
                return false;
        }

        // --------------------------------------------
        // --horizontal 1mm
        // --------------------------------------------
        else if (argument == "--horizontal")
        {
            if (i + 1 >= argc)
                return false;

            QString value = argv[++i];

            value.remove("mm");

            bool ok = false;
            horizontalMM = value.toDouble(&ok);

            if (!ok || horizontalMM <= 0)
                return false;
        }

        // --------------------------------------------
        // --numofline 10 20
        //
        // 10 = vertical lines
        // 20 = horizontal lines
        // --------------------------------------------
        else if (argument == "--numofline")
        {
            if (i + 2 >= argc)
                return false;

            bool ok1 = false;
            bool ok2 = false;

            verticalLines =
                QString(argv[++i]).toInt(&ok1);

            horizontalLines =
                QString(argv[++i]).toInt(&ok2);

            if (!ok1 || !ok2)
                return false;

            if (verticalLines < 1 ||
                horizontalLines < 1)
                return false;
        }

        // --------------------------------------------
        // --color red
        // --------------------------------------------
        else if (argument == "--color")
        {
            if (i + 1 >= argc)
                return false;

            QString value = argv[++i];

            QColor color(value);

            if (!color.isValid())
            {
                qDebug() << "Invalid color:" << value;
                return false;
            }

            lineColor = color;
        }

        // --------------------------------------------
        // Unknown argument
        // --------------------------------------------
        else
        {
            qDebug() << "Unknown argument:" << argument;
            return false;
        }
    }

    /*
        Calculate the complete window size.

        Example:

        vertical:
            10 lines × 1mm

        horizontal:
            20 lines × 1mm
    */

    int width =
        static_cast<int>(
            mmToPixels(verticalMM) *
            (verticalLines + 1)
        );

    int height =
        static_cast<int>(
            mmToPixels(horizontalMM) *
            (horizontalLines + 1)
        );

    resize(width, height);

    return true;
}


double GridWidget::mmToPixels(double mm) const
{
    /*
        Qt reports screen resolution in DPI.

        1 inch = 25.4 mm

        Therefore:

        pixels = mm × DPI / 25.4
    */

    QScreen *screen = QApplication::primaryScreen();

    if (!screen)
        return mm * 3.78;

    double dpi = screen->logicalDotsPerInch();

    return mm * dpi / 25.4;
}


void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    /*
        QPainter is Qt's drawing system.

        We draw directly onto the QWidget.
    */
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    /*
        Transparent background.

        We deliberately DO NOT draw a rectangle.
    */

    QPen pen(lineColor);

    /*
        Thin 1-pixel line.
    */
    pen.setWidth(1);

    painter.setPen(pen);

    const double verticalSpacing =
        mmToPixels(verticalMM);

    const double horizontalSpacing =
        mmToPixels(horizontalMM);

    // --------------------------------------------
    // Draw vertical lines
    // --------------------------------------------

    for (int i = 1; i <= verticalLines; ++i)
    {
        double x = i * verticalSpacing;

        painter.drawLine(
            QPointF(x, 0),
            QPointF(x, height())
        );
    }

    // --------------------------------------------
    // Draw horizontal lines
    // --------------------------------------------

    for (int i = 1; i <= horizontalLines; ++i)
    {
        double y = i * horizontalSpacing;

        painter.drawLine(
            QPointF(0, y),
            QPointF(width(), y)
        );
    }
}