# Qt5 — Own Class Types & How to Use Them

When building a Qt5 application with **OOP**, you normally create your own classes instead of putting everything inside `main.cpp`.

The basic idea is:

```text
main.cpp
   │
   └── MainWindow
          │
          ├── Button
          ├── Label
          └── MyCalculator
```

---

## 1. Basic C++ class

Before Qt classes, understand a normal C++ class:

```cpp
class Calculator
{
public:
    int add(int a, int b)
    {
        return a + b;
    }
};
```

Use it:

```cpp
Calculator calc;

int result = calc.add(10, 20);
```

Result:

```text
30
```

The class contains **data + functions**.

---

# 2. Qt's own class types

Qt provides many classes that you can use directly.

Some important Qt5 classes:

| Qt class       | Purpose                    |
| -------------- | -------------------------- |
| `QApplication` | Starts the GUI application |
| `QWidget`      | Basic GUI window/widget    |
| `QMainWindow`  | Main application window    |
| `QDialog`      | Dialog window              |
| `QPushButton`  | Button                     |
| `QLabel`       | Text/display               |
| `QLineEdit`    | Text input                 |
| `QTextEdit`    | Multi-line text            |
| `QCheckBox`    | Checkbox                   |
| `QComboBox`    | Dropdown                   |
| `QListWidget`  | List                       |
| `QTimer`       | Timer                      |
| `QString`      | Qt string                  |
| `QFile`        | File handling              |
| `QSerialPort`  | Serial communication       |

---

# 3. `QWidget` — the basic GUI class

A very simple Qt application:

```cpp
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;

    window.resize(500, 300);
    window.setWindowTitle("My Qt5 App");
    window.show();

    return app.exec();
}
```

Think:

```cpp
QWidget window;
```

means:

> Create an object of Qt's `QWidget` class.

Then:

```cpp
window.show();
```

means:

> Show that object as a window.

---

# 4. Creating your OWN Qt class

This is where OOP becomes useful.

Suppose your application has a main window.

Create:

```text
MainWindow.h
MainWindow.cpp
main.cpp
```

### MainWindow.h

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
public:
    MainWindow();
};

#endif
```

Important line:

```cpp
class MainWindow : public QMainWindow
```

means:

```text
QMainWindow
     ↑
     │ inherits
     │
MainWindow
```

Your `MainWindow` **is a QMainWindow**.

---

# 5. MainWindow.cpp

```cpp
#include "MainWindow.h"

MainWindow::MainWindow()
{
    resize(600, 400);
    setWindowTitle("My Application");
}
```

The constructor:

```cpp
MainWindow::MainWindow()
```

runs when you create:

```cpp
MainWindow window;
```

---

# 6. main.cpp

```cpp
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
```

Now the structure is:

```text
main.cpp
   │
   │ creates
   ▼
MainWindow
   │
   │ inherits
   ▼
QMainWindow
```

---

# 7. Adding your own button

Now let's make the example slightly more useful.

### MainWindow.h

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    QPushButton *button;
};

#endif
```

We have our own member variable:

```cpp
QPushButton *button;
```

---

### MainWindow.cpp

```cpp
#include "MainWindow.h"

#include <QPushButton>

MainWindow::MainWindow()
{
    resize(600, 400);
    setWindowTitle("My Qt5 App");

    button = new QPushButton("Click Me", this);

    button->setGeometry(200, 150, 200, 50);
}
```

Now:

```cpp
button = new QPushButton("Click Me", this);
```

creates a Qt button object.

```text
MainWindow
    │
    └── QPushButton
```

---

# 8. Signals and Slots

Qt's most important concept is:

```text
Signal → Slot
```

For example:

```text
User clicks button
       │
       ▼
QPushButton emits signal
       │
       ▼
your function receives it
```

Example:

```cpp
connect(button, &QPushButton::clicked,
        this, &MainWindow::buttonClicked);
```

Add your function.

### MainWindow.h

```cpp
private:
    void buttonClicked();
```

### MainWindow.cpp

```cpp
void MainWindow::buttonClicked()
{
    setWindowTitle("Button Clicked!");
}
```

Complete idea:

```cpp
connect(button, &QPushButton::clicked,
        this, &MainWindow::buttonClicked);
```

means:

> When `button` is clicked, call `MainWindow::buttonClicked()`.

---

# 9. Your own helper class

You don't have to make every class a GUI class.

For example, create a calculator class:

### Calculator.h

```cpp
#ifndef CALCULATOR_H
#define CALCULATOR_H

class Calculator
{
public:
    int add(int a, int b);
    int subtract(int a, int b);
};

#endif
```

### Calculator.cpp

```cpp
#include "Calculator.h"

int Calculator::add(int a, int b)
{
    return a + b;
}

int Calculator::subtract(int a, int b)
{
    return a - b;
}
```

Now `MainWindow` can use your class.

```cpp
#include "Calculator.h"
```

Then:

```cpp
Calculator calculator;
```

and:

```cpp
int result = calculator.add(10, 20);
```

---

# 10. Important Qt class categories

For learning, divide classes into these groups.

### A. Application

```cpp
QApplication
```

Controls the GUI application's event loop.

---

### B. Window

```cpp
QWidget
QMainWindow
QDialog
```

Used to create windows.

---

### C. Widgets

```cpp
QPushButton
QLabel
QLineEdit
QTextEdit
QCheckBox
QComboBox
```

These are GUI components.

---

### D. Layouts

```cpp
QVBoxLayout
QHBoxLayout
QGridLayout
```

Used to arrange widgets.

Example:

```text
┌─────────────────────┐
│       QLabel        │
│                     │
│    QPushButton      │
│                     │
│    QLineEdit        │
└─────────────────────┘
```

---

### E. Utility / Core classes

```cpp
QString
QList
QFile
QTimer
QDateTime
QJsonObject
QJsonDocument
```

These handle data and application logic.

---

# 11. The most important OOP pattern for your Qt projects

For a larger application, think like this:

```text
MyApplication
│
├── MainWindow
│      │
│      ├── QPushButton
│      ├── QLabel
│      └── QLineEdit
│
├── Calculator
│
├── SerialManager
│
├── SettingsManager
│
└── Logger
```

For example, your future **Qt Serial/Bluetooth application** could be:

```text
MainWindow
     │
     └── SerialManager
             │
             └── QSerialPort
```

`MainWindow` handles the GUI.

`SerialManager` handles communication.

That is much cleaner than putting serial communication, GUI, buttons, and application logic into `main.cpp`.

---

# 12. Simple rule to remember

When creating a Qt application, ask:

```text
What is this class responsible for?
```

For example:

```text
MainWindow
    → GUI

SerialManager
    → Serial communication

Logger
    → Logging

Calculator
    → Calculations

Settings
    → Configuration
```

**One class = one clear responsibility.**

That's the foundation for building a clean Qt5 OOP application.
