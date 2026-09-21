#include "GridWidget.h"


#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QWindow>


// ============================================================
// CONSTRUCTOR
// ============================================================

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent),
      defaultLineWidth(1)
{
    // --------------------------------------------------------
    // Make the background transparent.
    //
    // IMPORTANT:
    //
    // We do NOT paint a background.
    // Only our lines and top bar are painted.
    // --------------------------------------------------------

    setAttribute(
        Qt::WA_TranslucentBackground
    );


    // --------------------------------------------------------
    // Remove normal operating-system title bar.
    //
    // We create our own top bar.
    // --------------------------------------------------------

    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint
    );


    // --------------------------------------------------------
    // Allow mouse events.
    // --------------------------------------------------------

    setMouseTracking(true);


    // --------------------------------------------------------
    // Initial minimum size.
    // --------------------------------------------------------

    setMinimumSize(
        100,
        100
    );
}


// ============================================================
// PARSE COMMAND LINE
// ============================================================
//
// Supported:
//
// --vertical 1mm 10 red
//
// --horizontal 2mm 20 blue
//
// --line-width 2
//
// Multiple groups are allowed:
//
// --vertical 1mm 10 red
// --vertical 2mm 20 blue
// --vertical 5mm 5 green
//
// --horizontal 1mm 10 white
// --horizontal 3mm 20 yellow
// ============================================================

bool GridWidget::parseArguments(
    int argc,
    char *argv[]
)
{
    // --------------------------------------------------------
    // Start clean.
    // --------------------------------------------------------

    verticalGroups.clear();

    horizontalGroups.clear();


    // --------------------------------------------------------
    // Read arguments one by one.
    // --------------------------------------------------------

    for (int i = 1; i < argc; ++i)
    {
        QString argument = argv[i];


        // ====================================================
        // --vertical
        //
        // Example:
        //
        // --vertical 1mm 10 red
        // ====================================================

        if (argument == "--vertical")
        {
            // Need:
            //
            // spacing
            // count
            // color

            if (i + 3 >= argc)
            {
                qDebug()
                    << "ERROR: --vertical requires:"
                    << "spacing count color";

                return false;
            }


            // ------------------------------------------------
            // Spacing
            // ------------------------------------------------

            QString spacingText =
                argv[++i];

            spacingText.remove(
                "mm",
                Qt::CaseInsensitive
            );


            bool spacingOK = false;

            double spacing =
                spacingText.toDouble(
                    &spacingOK
                );


            if (!spacingOK || spacing <= 0)
            {
                qDebug()
                    << "ERROR: Invalid vertical spacing:"
                    << spacingText;

                return false;
            }


            // ------------------------------------------------
            // Number of lines
            // ------------------------------------------------

            QString countText =
                argv[++i];


            bool countOK = false;

            int count =
                countText.toInt(
                    &countOK
                );


            if (!countOK || count <= 0)
            {
                qDebug()
                    << "ERROR: Invalid vertical line count:"
                    << countText;

                return false;
            }


            // ------------------------------------------------
            // Color
            // ------------------------------------------------

            QString colorText =
                argv[++i];


            QColor color(
                colorText
            );


            if (!color.isValid())
            {
                qDebug()
                    << "ERROR: Invalid vertical color:"
                    << colorText;

                return false;
            }


            // ------------------------------------------------
            // Create line group
            // ------------------------------------------------

            LineGroup group;

            group.spacingMM =
                spacing;

            group.count =
                count;

            group.color =
                color;

            group.width =
                defaultLineWidth;


            // ------------------------------------------------
            // Add group
            // ------------------------------------------------

            verticalGroups.append(
                group
            );
        }


        // ====================================================
        // --horizontal
        //
        // Example:
        //
        // --horizontal 2mm 10 blue
        // ====================================================

        else if (argument == "--horizontal")
        {
            if (i + 3 >= argc)
            {
                qDebug()
                    << "ERROR: --horizontal requires:"
                    << "spacing count color";

                return false;
            }


            // ------------------------------------------------
            // Spacing
            // ------------------------------------------------

            QString spacingText =
                argv[++i];


            spacingText.remove(
                "mm",
                Qt::CaseInsensitive
            );


            bool spacingOK = false;

            double spacing =
                spacingText.toDouble(
                    &spacingOK
                );


            if (!spacingOK || spacing <= 0)
            {
                qDebug()
                    << "ERROR: Invalid horizontal spacing:"
                    << spacingText;

                return false;
            }


            // ------------------------------------------------
            // Number of lines
            // ------------------------------------------------

            QString countText =
                argv[++i];


            bool countOK = false;

            int count =
                countText.toInt(
                    &countOK
                );


            if (!countOK || count <= 0)
            {
                qDebug()
                    << "ERROR: Invalid horizontal line count:"
                    << countText;

                return false;
            }


            // ------------------------------------------------
            // Color
            // ------------------------------------------------

            QString colorText =
                argv[++i];


            QColor color(
                colorText
            );


            if (!color.isValid())
            {
                qDebug()
                    << "ERROR: Invalid horizontal color:"
                    << colorText;

                return false;
            }


            // ------------------------------------------------
            // Create group
            // ------------------------------------------------

            LineGroup group;

            group.spacingMM =
                spacing;

            group.count =
                count;

            group.color =
                color;

            group.width =
                defaultLineWidth;


            // ------------------------------------------------
            // Add group
            // ------------------------------------------------

            horizontalGroups.append(
                group
            );
        }


        // ====================================================
        // --line-width
        //
        // Example:
        //
        // --line-width 2
        // ====================================================

        else if (argument == "--line-width")
        {
            if (i + 1 >= argc)
            {
                qDebug()
                    << "ERROR: --line-width requires a value";

                return false;
            }


            bool widthOK = false;


            defaultLineWidth =
                QString(argv[++i]).toInt(
                    &widthOK
                );


            if (!widthOK ||
                defaultLineWidth < 1)
            {
                qDebug()
                    << "ERROR: Invalid line width";

                return false;
            }
        }


        // ====================================================
        // Unknown argument
        // ====================================================

        else
        {
            qDebug()
                << "ERROR: Unknown argument:"
                << argument;

            qDebug()
                << "";
            qDebug()
                << "Usage:";
            qDebug()
                << "./app"
                << "--vertical 1mm 10 red"
                << "--horizontal 2mm 20 blue"
                << "--line-width 2";

            return false;
        }
    }


    // ========================================================
    // Apply line width
    // ========================================================

    for (LineGroup &group : verticalGroups)
    {
        group.width =
            defaultLineWidth;
    }


    for (LineGroup &group : horizontalGroups)
    {
        group.width =
            defaultLineWidth;
    }


    // ========================================================
    // At least one group required
    // ========================================================

    if (verticalGroups.isEmpty() &&
        horizontalGroups.isEmpty())
    {
        qDebug()
            << "ERROR: No lines specified.";

        qDebug()
            << "";
        qDebug()
            << "Example:";

        qDebug()
            << "./app"
            << "--vertical 1mm 10 red"
            << "--horizontal 2mm 10 blue";

        return false;
    }


    // ========================================================
    // Calculate window size
    // ========================================================

    calculateWindowSize();


    return true;
}


// ============================================================
// MM TO PIXELS
// ============================================================

double GridWidget::mmToPixels(
    double mm
) const
{
    QScreen *screen =
        QApplication::primaryScreen();


    if (!screen)
    {
        // Approximately 96 DPI.
        //
        // 1 mm ≈ 3.78 pixels

        return mm * 3.78;
    }


    const double dpi =
        screen->logicalDotsPerInch();


    /*
        1 inch = 25.4 mm

        pixels =
            mm × DPI / 25.4
    */

    return
        mm *
        dpi /
        25.4;
}


// ============================================================
// CALCULATE WINDOW SIZE
// ============================================================

void GridWidget::calculateWindowSize()
{
    double requiredWidth = 100.0;

    double requiredHeight = 100.0;


    // --------------------------------------------------------
    // Find total required vertical width.
    //
    // Each vertical group occupies:
    //
    // spacing × number of lines
    // --------------------------------------------------------

    for (const LineGroup &group :
         verticalGroups)
    {
        const double groupWidth =
            mmToPixels(
                group.spacingMM
            ) *
            group.count;


        requiredWidth +=
            groupWidth;
    }


    // --------------------------------------------------------
    // Find total required horizontal height.
    // --------------------------------------------------------

    for (const LineGroup &group :
         horizontalGroups)
    {
        const double groupHeight =
            mmToPixels(
                group.spacingMM
            ) *
            group.count;


        requiredHeight +=
            groupHeight;
    }


    // --------------------------------------------------------
    // Add top bar.
    // --------------------------------------------------------

    requiredHeight +=
        TOP_BAR_HEIGHT;


    // --------------------------------------------------------
    // Resize application.
    // --------------------------------------------------------

    resize(
        static_cast<int>(requiredWidth),
        static_cast<int>(requiredHeight)
    );
}


// ============================================================
// PAINT EVENT
// ============================================================

void GridWidget::paintEvent(
    QPaintEvent *event
)
{
    Q_UNUSED(event);


    QPainter painter(this);


    // --------------------------------------------------------
    // Smooth line drawing.
    // --------------------------------------------------------

    painter.setRenderHint(
        QPainter::Antialiasing,
        true
    );


    // --------------------------------------------------------
    // Draw top bar.
    // --------------------------------------------------------

    drawTopBar(
        painter
    );


    // --------------------------------------------------------
    // Move drawing origin below top bar.
    // --------------------------------------------------------

    painter.translate(
        0,
        TOP_BAR_HEIGHT
    );


    // --------------------------------------------------------
    // Draw vertical groups.
    // --------------------------------------------------------

    drawVerticalLines(
        painter
    );


    // --------------------------------------------------------
    // Draw horizontal groups.
    // --------------------------------------------------------

    drawHorizontalLines(
        painter
    );
}


// ============================================================
// TOP BAR
// ============================================================

void GridWidget::drawTopBar(
    QPainter &painter
)
{
    painter.save();


    // --------------------------------------------------------
    // Semi-transparent dark bar.
    // --------------------------------------------------------

    QColor barColor(
        35,
        35,
        35,
        190
    );


    painter.setPen(
        Qt::NoPen
    );


    painter.setBrush(
        barColor
    );


    painter.drawRect(
        0,
        0,
        width(),
        TOP_BAR_HEIGHT
    );


    // --------------------------------------------------------
    // Title
    // --------------------------------------------------------

    painter.setPen(
        Qt::white
    );


    QFont font;

    font.setPointSize(9);

    font.setBold(true);


    painter.setFont(
        font
    );


    painter.drawText(
        10,
        0,
        width() - 50,
        TOP_BAR_HEIGHT,
        Qt::AlignVCenter,
        "GRID"
    );


    // ========================================================
    // CLOSE BUTTON
    // ========================================================

    const int closeWidth = 36;


    QRect closeRect(
        width() - closeWidth,
        0,
        closeWidth,
        TOP_BAR_HEIGHT
    );


    // --------------------------------------------------------
    // Close button background.
    // --------------------------------------------------------

    QColor closeColor(
        180,
        40,
        40,
        210
    );


    painter.setPen(
        Qt::NoPen
    );


    painter.setBrush(
        closeColor
    );


    painter.drawRect(
        closeRect
    );


    // --------------------------------------------------------
    // X symbol
    // --------------------------------------------------------

    QPen xPen(
        Qt::white,
        2
    );


    painter.setPen(
        xPen
    );


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


// ============================================================
// VERTICAL LINES
// ============================================================

void GridWidget::drawVerticalLines(
    QPainter &painter
)
{
    // --------------------------------------------------------
    // Every vertical group is independent.
    //
    // Example:
    //
    // Group 1:
    // 1mm × 10 red
    //
    // Group 2:
    // 2mm × 20 blue
    // --------------------------------------------------------

    for (const LineGroup &group :
         verticalGroups)
    {
        const double spacing =
            mmToPixels(
                group.spacingMM
            );


        // ----------------------------------------------------
        // Create pen for this group.
        // ----------------------------------------------------

        QPen pen(
            group.color
        );


        pen.setWidth(
            group.width
        );


        painter.setPen(
            pen
        );


        // ----------------------------------------------------
        // Draw lines.
        // ----------------------------------------------------

        for (int i = 1;
             i <= group.count;
             ++i)
        {
            const double x =
                spacing * i;


            painter.drawLine(
                QPointF(
                    x,
                    0
                ),

                QPointF(
                    x,
                    height() -
                    TOP_BAR_HEIGHT
                )
            );
        }
    }
}


// ============================================================
// HORIZONTAL LINES
// ============================================================

void GridWidget::drawHorizontalLines(
    QPainter &painter
)
{
    // --------------------------------------------------------
    // Every horizontal group is independent.
    // --------------------------------------------------------

    for (const LineGroup &group :
         horizontalGroups)
    {
        const double spacing =
            mmToPixels(
                group.spacingMM
            );


        // ----------------------------------------------------
        // Create pen.
        // ----------------------------------------------------

        QPen pen(
            group.color
        );


        pen.setWidth(
            group.width
        );


        painter.setPen(
            pen
        );


        // ----------------------------------------------------
        // Draw lines.
        // ----------------------------------------------------

        for (int i = 1;
             i <= group.count;
             ++i)
        {
            const double y =
                spacing * i;


            painter.drawLine(
                QPointF(
                    0,
                    y
                ),

                QPointF(
                    width(),
                    y
                )
            );
        }
    }
}


// ============================================================
// MOUSE PRESS
// ============================================================

void GridWidget::mousePressEvent(
    QMouseEvent *event
)
{
    // --------------------------------------------------------
    // Only left mouse button.
    // --------------------------------------------------------

    if (event->button() != Qt::LeftButton)
    {
        QWidget::mousePressEvent(
            event
        );

        return;
    }


    const QPoint position =
        event->pos();


    // ========================================================
    // CLOSE BUTTON
    // ========================================================

    if (isInsideCloseButton(
            position))
    {
        close();

        return;
    }


    // ========================================================
    // TOP BAR
    // ========================================================

    if (position.y() <= TOP_BAR_HEIGHT)
    {
        /*
            IMPORTANT:

            We use Qt's native system move.

            This is especially useful for a frameless
            window and Wayland/X11 environments.

            Qt asks the window manager to move the
            application window.
        */

        if (windowHandle())
        {
            windowHandle()->startSystemMove();
        }


        event->accept();

        return;
    }


    QWidget::mousePressEvent(
        event
    );
}


// ============================================================
// CLOSE BUTTON TEST
// ============================================================

bool GridWidget::isInsideCloseButton(
    const QPoint &position
) const
{
    const int closeWidth = 36;


    QRect closeRect(
        width() - closeWidth,
        0,
        closeWidth,
        TOP_BAR_HEIGHT
    );


    return closeRect.contains(
        position
    );
}