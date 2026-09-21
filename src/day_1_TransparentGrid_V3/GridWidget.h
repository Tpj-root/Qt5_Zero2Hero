#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QColor>
#include <QVector>


// ============================================================
// LineGroup
//
// Represents ONE group of lines.
//
// Example:
//
// --vertical 1mm 10 red
//
// means:
//
// spacing = 1 mm
// number  = 10
// color   = red
// ============================================================

struct LineGroup
{
    double spacingMM;

    int count;

    QColor color;

    int width;
};


// ============================================================
// GridWidget
// ============================================================

class GridWidget : public QWidget
{
public:

    explicit GridWidget(QWidget *parent = nullptr);


    // --------------------------------------------------------
    // Read command-line arguments
    // --------------------------------------------------------

    bool parseArguments(
        int argc,
        char *argv[]
    );


protected:

    // --------------------------------------------------------
    // Qt painting
    // --------------------------------------------------------

    void paintEvent(
        QPaintEvent *event
    ) override;


    // --------------------------------------------------------
    // Mouse
    //
    // Used for:
    //
    // 1. Move window from top bar
    // 2. Close button
    // --------------------------------------------------------

    void mousePressEvent(
        QMouseEvent *event
    ) override;


private:

    // ========================================================
    // Line groups
    // ========================================================

    QVector<LineGroup> verticalGroups;

    QVector<LineGroup> horizontalGroups;


    // ========================================================
    // Default line width
    // ========================================================

    int defaultLineWidth;


    // ========================================================
    // Top bar
    // ========================================================

    static constexpr int TOP_BAR_HEIGHT = 30;


    // ========================================================
    // Helpers
    // ========================================================

    double mmToPixels(
        double mm
    ) const;


    void drawTopBar(
        QPainter &painter
    );


    void drawVerticalLines(
        QPainter &painter
    );


    void drawHorizontalLines(
        QPainter &painter
    );


    bool isInsideCloseButton(
        const QPoint &position
    ) const;


    void calculateWindowSize();
};

#endif