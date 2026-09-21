#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>
#include <QColor>

class GridWidget : public QWidget
{
public:
    explicit GridWidget(QWidget *parent = nullptr);

    bool parseArguments(int argc, char *argv[]);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double verticalMM;
    double horizontalMM;

    int verticalLines;
    int horizontalLines;

    QColor lineColor;

    double mmToPixels(double mm) const;
};

#endif