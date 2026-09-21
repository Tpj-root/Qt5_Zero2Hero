#include "GridWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QWindow>


GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent),
      verticalMM(1.0),
      horizontalMM(1.0),
      verticalLines(10),
      horizontalLines(20),
      lineColor(Qt::red)
{
    // --------------------------------------------
    // Transparent background
    // --------------------------------------------

    setAttribute(Qt::WA_TranslucentBackground);

    // --------------------------------------------
    // Remove normal OS window frame
    //
    // We will create our own top bar.
    // --------------------------------------------

    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint
    );

    setMinimumSize(1000, 1000);

    // Mouse tracking allows us to receive
    // mouse move events.
    setMouseTracking(true);
}


// =================================================
// COMMAND LINE ARGUMENTS
// =================================================

bool GridWidget::parseArguments(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        QString argument = argv[i];

        // ----------------------------------------
        // --vertical 1mm
        // ----------------------------------------

        if (argument == "--vertical")
        {
            if (i + 1 >= argc)
                return false;

            QString value = argv[++i];

            value.remove("mm");

            bool ok = false;

            verticalMM = value.toDouble(&ok);

            if (!ok || verticalMM <= 0)
            {
                qDebug() << "Invalid vertical spacing:" << value;
                return false;
            }
        }

        // ----------------------------------------
        // --horizontal 1mm
        // ----------------------------------------

        else if (argument == "--horizontal")
        {
            if (i + 1 >= argc)
                return false;

            QString value = argv[++i];

            value.remove("mm");

            bool ok = false;

            horizontalMM = value.toDouble(&ok);

            if (!ok || horizontalMM <= 0)
            {
                qDebug() << "Invalid horizontal spacing:" << value;
                return false;
            }
        }

        // ----------------------------------------
        // --numofline 10 20
        //
        // 10 = vertical lines
        // 20 = horizontal lines
        // ----------------------------------------

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
            {
                qDebug() << "Invalid number of lines";
                return false;
            }

            if (verticalLines < 1 ||
                horizontalLines < 1)
            {
                qDebug() << "Number of lines must be greater than 0";
                return false;
            }
        }

        // ----------------------------------------
        // --color red
        // ----------------------------------------

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

        // ----------------------------------------
        // Unknown argument
        // ----------------------------------------

        else
        {
            qDebug() << "Unknown argument:" << argument;
            return false;
        }
    }


    // ============================================
    // Calculate window size
    // ============================================

    const double gridWidth =
        mmToPixels(verticalMM) *
        (verticalLines + 1);

    const double gridHeight =
        mmToPixels(horizontalMM) *
        (horizontalLines + 1);


    const int windowWidth =
        static_cast<int>(gridWidth);

    const int windowHeight =
        static_cast<int>(gridHeight) +
        TOP_BAR_HEIGHT;


    resize(windowWidth, windowHeight);

    return true;
}


// =================================================
// MM -> PIXELS
// =================================================

double GridWidget::mmToPixels(double mm) const
{
    QScreen *screen =
        QApplication::primaryScreen();

    if (!screen)
    {
        // Approximate 96 DPI fallback
        return mm * 3.78;
    }

    const double dpi =
        screen->logicalDotsPerInch();

    /*
        1 inch = 25.4 mm

        pixels = mm * DPI / 25.4
    */

    return mm * dpi / 25.4;
}


// =================================================
// PAINT EVENT
// =================================================

void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(
        QPainter::Antialiasing,
        true
    );


    // --------------------------------------------
    // Draw custom top bar
    // --------------------------------------------

    drawTopBar(painter);


    // --------------------------------------------
    // Draw grid
    // --------------------------------------------

    drawGrid(painter);
}


// =================================================
// TOP BAR
// =================================================

void GridWidget::drawTopBar(QPainter &painter)
{
    /*
        We intentionally draw a small semi-transparent
        top bar.

        The rest of the window remains transparent.
    */

    painter.save();

    QColor barColor(
        40,
        40,
        40,
        180
    );

    painter.setBrush(barColor);

    painter.setPen(Qt::NoPen);

    painter.drawRect(
        0,
        0,
        width(),
        TOP_BAR_HEIGHT
    );


    // --------------------------------------------
    // Title
    // --------------------------------------------

    painter.setPen(Qt::white);

    QFont font;

    font.setPointSize(9);
    font.setBold(true);

    painter.setFont(font);

    painter.drawText(
        10,
        0,
        width() - 45,
        TOP_BAR_HEIGHT,
        Qt::AlignVCenter,
        "GRID"
    );


    // --------------------------------------------
    // Close button
    // --------------------------------------------

    const int buttonWidth = 36;

    QRect closeRect(
        width() - buttonWidth,
        0,
        buttonWidth,
        TOP_BAR_HEIGHT
    );


    // Close button background

    QColor closeColor(
        180,
        40,
        40,
        200
    );

    painter.setBrush(closeColor);

    painter.setPen(Qt::NoPen);

    painter.drawRect(closeRect);


    // --------------------------------------------
    // X symbol
    // --------------------------------------------

    painter.setPen(Qt::white);

    QPen closePen(
        Qt::white,
        2
    );

    painter.setPen(closePen);

    const int margin = 11;

    painter.drawLine(
        closeRect.left() + margin,
        closeRect.top() + margin,
        closeRect.right() - margin - 1,
        closeRect.bottom() - margin - 1
    );

    painter.drawLine(
        closeRect.right() - margin - 1,
        closeRect.top() + margin,
        closeRect.left() + margin,
        closeRect.bottom() - margin - 1
    );


    painter.restore();
}


// =================================================
// GRID
// =================================================

void GridWidget::drawGrid(QPainter &painter)
{
    painter.save();


    // --------------------------------------------
    // Grid starts BELOW top bar
    // --------------------------------------------

    painter.translate(
        0,
        TOP_BAR_HEIGHT
    );


    // --------------------------------------------
    // Line style
    // --------------------------------------------

    QPen pen(lineColor);

    pen.setWidth(1);

    painter.setPen(pen);


    const double verticalSpacing =
        mmToPixels(verticalMM);

    const double horizontalSpacing =
        mmToPixels(horizontalMM);


    // ============================================
    // VERTICAL LINES
    // ============================================

    for (int i = 1;
         i <= verticalLines;
         ++i)
    {
        const double x =
            i * verticalSpacing;

        painter.drawLine(
            QPointF(x, 0),
            QPointF(
                x,
                height() - TOP_BAR_HEIGHT
            )
        );
    }


    // ============================================
    // HORIZONTAL LINES
    // ============================================

    for (int i = 1;
         i <= horizontalLines;
         ++i)
    {
        const double y =
            i * horizontalSpacing;

        painter.drawLine(
            QPointF(0, y),
            QPointF(
                width(),
                y
            )
        );
    }


    painter.restore();
}


// =================================================
// MOUSE PRESS
// =================================================

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    const QPoint position = event->pos();

    // --------------------------------------------
    // Close button
    // --------------------------------------------

    if (isInsideCloseButton(position))
    {
        close();
        return;
    }

    // --------------------------------------------
    // Top bar
    // --------------------------------------------

    if (position.y() <= TOP_BAR_HEIGHT)
    {
        if (windowHandle())
        {
            windowHandle()->startSystemMove();
        }

        event->accept();
        return;
    }

    QWidget::mousePressEvent(event);
}

// // =================================================
// // MOUSE MOVE
// // =================================================

// void GridWidget::mouseMoveEvent(QMouseEvent *event)
// {
//     if (movingWindow)
//     {
//         /*
//             New window position:

//             current mouse position
//             -
//             original mouse offset
//         */

//         move(
//             event->globalPos() -
//             dragPosition
//         );

//         event->accept();

//         return;
//     }


//     QWidget::mouseMoveEvent(event);
// }


// // =================================================
// // MOUSE RELEASE
// // =================================================

// void GridWidget::mouseReleaseEvent(QMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton)
//     {
//         movingWindow = false;

//         event->accept();

//         return;
//     }


//     QWidget::mouseReleaseEvent(event);
// }


// =================================================
// CLOSE BUTTON HIT TEST
// =================================================

bool GridWidget::isInsideCloseButton(
    const QPoint &position) const
{
    const int buttonWidth = 36;

    QRect closeRect(
        width() - buttonWidth,
        0,
        buttonWidth,
        TOP_BAR_HEIGHT
    );

    return closeRect.contains(position);
}