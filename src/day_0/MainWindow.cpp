#include "MainWindow.h"

#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    label = new QLabel("Hello Qt!", this);

    label->setGeometry(50, 40, 300, 40);

    button = new QPushButton("Click Me", this);

    button->setGeometry(50, 100, 150, 40);

    connect(
        button,
        &QPushButton::clicked,
        this,
        &MainWindow::buttonClicked
    );

    setWindowTitle("My First Qt5 Application");

    resize(400, 250);
}

void MainWindow::buttonClicked()
{
    label->setText("Button clicked!");
}