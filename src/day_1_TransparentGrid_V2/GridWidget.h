#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPoint>

class GridWidget : public QWidget
{
public:
    explicit GridWidget(QWidget *parent = nullptr);

    bool parseArguments(int argc, char *argv[]);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    double verticalMM;
    double horizontalMM;

    int verticalLines;
    int horizontalLines;

    QColor lineColor;

    static constexpr int TOP_BAR_HEIGHT = 28;

    double mmToPixels(double mm) const;

    bool isInsideCloseButton(const QPoint &position) const;

    void drawTopBar(QPainter &painter);
    void drawGrid(QPainter &painter);
};

#endif