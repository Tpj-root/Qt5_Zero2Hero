# Qt5 Code Modules — Zero to Hero

In Qt5, **modules** are groups of related Qt classes.

Instead of one huge Qt library containing everything, Qt is divided into modules:

```text
Qt5
│
├── QtCore       → basic/core functionality
├── QtGui        → GUI foundation
├── QtWidgets    → buttons, windows, layouts...
├── QtNetwork    → TCP, UDP, network
├── QtSerialPort → serial communication
├── QtSql        → databases
├── QtXml        → XML
└── QtMultimedia → audio/video
```

The most important thing is understanding:

```text
Module
  ↓
contains classes
  ↓
your program uses those classes
```

---

# 1. QtCore

`QtCore` is the **foundation of Qt**.

It contains things like:

```text
QString
QByteArray
QList
QVector
QMap
QTimer
QDateTime
QFile
QObject
QThread
QDebug
```

You can think:

```text
QtCore
   │
   ├── QString
   ├── QList
   ├── QMap
   ├── QFile
   ├── QTimer
   ├── QObject
   └── ...
```

### Example

```cpp
#include <QCoreApplication>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString name = "When";

    qDebug() << "Hello" << name;

    return app.exec();
}
```

This is a **QtCore-only application**.

There is no GUI.

---

# 2. QtGui

`QtGui` provides the lower-level GUI foundation.

It contains things such as:

```text
QColor
QFont
QImage
QPixmap
QPainter
QClipboard
QKeyEvent
QMouseEvent
```

Example:

```cpp
#include <QImage>
#include <QColor>

QImage image(400, 300, QImage::Format_RGB32);

image.fill(Qt::white);
```

Here:

```cpp
QImage
```

comes from the QtGui module.

---

# 3. QtWidgets

This is the module you will use heavily for **desktop GUI applications**.

It contains:

```text
QWidget
QMainWindow
QDialog
QPushButton
QLabel
QLineEdit
QTextEdit
QCheckBox
QComboBox
QListWidget
QVBoxLayout
QHBoxLayout
QGridLayout
```

Example:

```cpp
#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton button("Hello Qt5");

    button.resize(300, 100);
    button.show();

    return app.exec();
}
```

Architecture:

```text
QtWidgets
    │
    ├── QWidget
    │     │
    │     ├── QPushButton
    │     ├── QLabel
    │     ├── QLineEdit
    │     └── ...
    │
    └── QMainWindow
```

---

# 4. QtNetwork

Used for networking.

Important classes:

```text
QTcpSocket
QTcpServer
QUdpSocket
QNetworkAccessManager
QNetworkRequest
QNetworkReply
```

For example, a TCP client might use:

```cpp
QTcpSocket socket;
```

Then:

```cpp
socket.connectToHost("192.168.1.100", 5000);
```

Concept:

```text
Your Application
       │
       ▼
QtNetwork
       │
       ├── TCP
       ├── UDP
       └── HTTP
```

---

# 5. QtSerialPort

This one is especially useful for your hardware projects.

It provides:

```text
QSerialPort
QSerialPortInfo
```

Example:

```cpp
#include <QSerialPort>

QSerialPort serial;

serial.setPortName("/dev/ttyUSB0");
serial.setBaudRate(QSerialPort::Baud9600);

serial.open(QIODevice::ReadWrite);
```

Read data:

```cpp
QByteArray data = serial.readAll();
```

Write:

```cpp
serial.write("HELLO");
```

Architecture:

```text
QtSerialPort
      │
      ├── QSerialPort
      │
      └── QSerialPortInfo
```

This is the module you'll want for your **Arduino / HC-05 / USB serial** Qt applications.

---

# 6. QtSql

Used for databases.

Important classes:

```text
QSqlDatabase
QSqlQuery
QSqlTableModel
QSqlQueryModel
```

Example:

```cpp
#include <QSqlDatabase>

QSqlDatabase db =
    QSqlDatabase::addDatabase("QSQLITE");

db.setDatabaseName("data.db");

if (db.open())
{
    // Database opened
}
```

Then queries:

```cpp
QSqlQuery query;

query.exec(
    "SELECT * FROM users"
);
```

Architecture:

```text
QtSql
  │
  ├── SQLite
  ├── MySQL
  ├── PostgreSQL
  └── other SQL drivers
```

---

# 7. QtXml

Used for XML.

Example XML:

```xml
<person>
    <name>When</name>
    <age>30</age>
</person>
```

Qt provides XML-related classes for reading/writing XML.

For modern applications, you may encounter JSON more often than XML.

---

# 8. QtMultimedia

Used for multimedia:

```text
audio
video
camera
microphone
```

Examples of classes include:

```text
QMediaPlayer
QAudioOutput
QCamera
```

Concept:

```text
QtMultimedia
      │
      ├── Audio
      ├── Video
      └── Camera
```

---

# 9. QtTest

Used for testing your application.

For example, testing:

```text
Calculator
SerialManager
Data processing
Math calculations
```

Instead of manually checking:

```text
10 + 20 = 30
```

you can create automated tests.

---

# 10. How modules appear in CMake

This is **very important**.

Suppose you need:

```text
QtWidgets
QtSerialPort
```

Your `CMakeLists.txt` might contain:

```cmake
find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    SerialPort
)

target_link_libraries(MyApp
    Qt5::Widgets
    Qt5::SerialPort
)
```

The relationship is:

```text
find_package()
      ↓
find Qt module
      ↓
target_link_libraries()
      ↓
link module to your program
```

---

# 11. Complete simple Qt5 project

Let's create:

```text
MyQtApp/
│
├── CMakeLists.txt
├── main.cpp
├── MainWindow.h
└── MainWindow.cpp
```

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)

project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 REQUIRED COMPONENTS
    Widgets
)

add_executable(MyQtApp
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(MyQtApp
    Qt5::Widgets
)
```

Here:

```text
Qt5::Widgets
```

means:

> Link my application with the Qt5 Widgets module.

---

# 12. MainWindow.h

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

`QMainWindow` comes from:

```text
QtWidgets
```

---

# 13. MainWindow.cpp

```cpp
#include "MainWindow.h"

#include <QPushButton>

MainWindow::MainWindow()
{
    resize(600, 400);

    setWindowTitle("Qt5 Module Example");

    button = new QPushButton(
        "Click Me",
        this
    );

    button->setGeometry(
        200,
        150,
        200,
        50
    );
}
```

Both:

```cpp
QMainWindow
QPushButton
```

belong to **QtWidgets**.

---

# 14. main.cpp

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

Now you have:

```text
                MyQtApp
                   │
                   ▼
              Qt5::Widgets
                   │
          ┌────────┴────────┐
          ▼                 ▼
    QApplication       QMainWindow
                            │
                            ▼
                       QPushButton
```

---

# 15. Multiple modules together

Now imagine your application needs:

```text
GUI
+
Serial
+
Network
+
Database
```

Your CMake becomes:

```cmake
find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    SerialPort
    Network
    Sql
)

target_link_libraries(MyApp
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Network
    Qt5::Sql
)
```

Architecture:

```text
                    MyApp
                      │
       ┌──────────────┼──────────────┐
       │              │              │
       ▼              ▼              ▼
   Widgets       SerialPort       Network
       │              │              │
       ▼              ▼              ▼
   GUI widgets    Arduino/HC05     TCP/UDP
                      │
                      │
                      ▼
                    Sql
                      │
                      ▼
                  Database
```

---

# 16. Module vs Class

This distinction is **very important**.

### Module

A collection/group of functionality:

```text
QtWidgets
QtNetwork
QtSerialPort
QtSql
```

### Class

A specific object/type:

```text
QPushButton
QMainWindow
QSerialPort
QTcpSocket
QSqlDatabase
```

For example:

```text
QtSerialPort             ← MODULE
      │
      ├── QSerialPort     ← CLASS
      └── QSerialPortInfo ← CLASS
```

And:

```text
QtWidgets                ← MODULE
      │
      ├── QMainWindow     ← CLASS
      ├── QPushButton     ← CLASS
      └── QLabel          ← CLASS
```

---

# 17. What you should learn first

For your **Qt5 Zero → Hero** path, I recommend this order:

```text
1. QtCore
      ↓
2. QtGui
      ↓
3. QtWidgets
      ↓
4. Signals & Slots
      ↓
5. Layouts
      ↓
6. QtSerialPort
      ↓
7. QtNetwork
      ↓
8. QtSql
      ↓
9. JSON
      ↓
10. Threads / QThread
```

Especially for the kind of applications you're interested in:

```text
QtCore
   ↓
QtWidgets
   ↓
OOP
   ↓
Signals / Slots
   ↓
QtSerialPort
   ↓
QtNetwork
   ↓
JSON
```

That gives you a very strong foundation for building a **real Qt5 desktop application**, rather than just learning individual buttons and windows.



# Qt5 QtCore Utility Classes — Zero to Hero

`QtCore` is the foundation of Qt5. It contains many **non-GUI utility classes** that you use for strings, files, time, containers, processes, threads, settings, JSON, debugging, and application logic.

Think of it like this:

```text
                         QtCore
                           │
        ┌──────────────────┼──────────────────┐
        │                  │                  │
      Data              System             Utility
        │                  │                  │
   QString             QFile              QDebug
   QByteArray          QDir               QTimer
   QList               QProcess           QSettings
   QMap                QThread            QRegularExpression
   QVariant            QStandardPaths     QCommandLineParser
```

---

# 1. `QString` — Text

Probably the most-used QtCore class.

```cpp
#include <QString>
#include <QDebug>

QString name = "When";

qDebug() << name;
```

### Combine strings

```cpp
QString first = "Hello";
QString second = "Qt5";

QString message = first + " " + second;
```

Result:

```text
Hello Qt5
```

### Convert number to text

```cpp
int value = 123;

QString text = QString::number(value);
```

### Convert text to number

```cpp
QString text = "123";

int value = text.toInt();
```

Other conversions:

```cpp
text.toDouble();
text.toFloat();
text.toLongLong();
```

### Useful functions

```cpp
name.length();
name.isEmpty();
name.contains("he");
name.startsWith("W");
name.endsWith("n");
name.toUpper();
name.toLower();
name.trimmed();
```

---

# 2. `QByteArray` — Raw binary data

Very important for your serial/Bluetooth/network work.

```cpp
#include <QByteArray>

QByteArray data = "HELLO";
```

Append:

```cpp
data.append(" WORLD");
```

Result:

```text
HELLO WORLD
```

Read bytes:

```cpp
char first = data[0];
```

Hex representation:

```cpp
QByteArray data;

data.append(char(0x01));
data.append(char(0x02));
data.append(char(0xFF));

qDebug() << data.toHex();
```

Useful mental model:

```text
QString
   ↓
Human-readable text

QByteArray
   ↓
Raw bytes
```

---

# 3. `QVariant` — Generic value container

`QVariant` can hold different types.

```cpp
QVariant value;

value = 100;
```

Later:

```cpp
value = "Hello";
```

Later:

```cpp
value = 12.5;
```

Retrieve:

```cpp
QString text = value.toString();
```

or:

```cpp
int number = value.toInt();
```

It's especially useful in Qt APIs where values may have different types.

---

# 4. `QList`

A list of values:

```cpp
QList<int> numbers;

numbers.append(10);
numbers.append(20);
numbers.append(30);
```

Access:

```cpp
qDebug() << numbers[0];
```

Loop:

```cpp
for (int number : numbers)
{
    qDebug() << number;
}
```

---

# 5. `QVector`

Dynamic array:

```cpp
QVector<double> prices;

prices.append(4350.10);
prices.append(4352.50);
prices.append(4355.20);
```

Access:

```cpp
double price = prices[1];
```

Useful for numerical data.

---

# 6. `QMap`

Stores key → value.

```cpp
QMap<QString, double> prices;

prices["XAUUSD"] = 4354.50;
prices["EURUSD"] = 1.1720;
```

Read:

```cpp
double gold = prices["XAUUSD"];
```

Concept:

```text
XAUUSD → 4354.50
EURUSD → 1.1720
```

Very useful for configuration and lookup tables.

---

# 7. `QHash`

Also key → value:

```cpp
QHash<QString, int> users;

users["When"] = 100;
users["John"] = 200;
```

Think:

```text
QMap
 → ordered key/value container

QHash
 → hash-based key/value container
```

Learn `QMap` first.

---

# 8. `QSet`

Stores unique values.

```cpp
QSet<QString> symbols;

symbols.insert("XAUUSD");
symbols.insert("EURUSD");
symbols.insert("XAUUSD");
```

The duplicate doesn't create another entry.

Concept:

```text
QSet
 ↓
unique values
```

---

# 9. `QDate`

Represents a date.

```cpp
QDate date(2026, 9, 21);
```

Current date:

```cpp
QDate today = QDate::currentDate();
```

Format:

```cpp
QString text =
    today.toString("dd-MM-yyyy");
```

Example:

```text
21-09-2026
```

---

# 10. `QTime`

Represents time.

```cpp
QTime time(19, 30, 0);
```

Current time:

```cpp
QTime now = QTime::currentTime();
```

Format:

```cpp
QString text =
    now.toString("HH:mm:ss");
```

---

# 11. `QDateTime`

Date + time together.

```cpp
QDateTime now =
    QDateTime::currentDateTime();
```

Format:

```cpp
QString timestamp =
    now.toString("yyyy-MM-dd HH:mm:ss");
```

Very useful for:

```text
logs
CSV
timestamps
events
database records
trading data
```

---

# 12. `QTimer`

Runs something after a period of time.

Example:

```cpp
#include <QTimer>

QTimer timer;

timer.setInterval(1000);

timer.start();
```

Every 1000 ms:

```text
1000 ms = 1 second
```

Connect timeout:

```cpp
connect(&timer,
        &QTimer::timeout,
        []()
{
    qDebug() << "Timer tick";
});
```

Output:

```text
Timer tick
Timer tick
Timer tick
...
```

For Qt applications, `QTimer` is generally preferable to manually sleeping the GUI thread.

---

# 13. `QFile`

File handling.

Write:

```cpp
QFile file("test.txt");

if (file.open(QIODevice::WriteOnly))
{
    file.write("Hello Qt5\n");
    file.close();
}
```

Read:

```cpp
QFile file("test.txt");

if (file.open(QIODevice::ReadOnly))
{
    QByteArray data = file.readAll();

    file.close();
}
```

Relationship:

```text
QFile
  ↓
QByteArray
  ↓
file contents
```

---

# 14. `QDir`

Works with directories.

```cpp
#include <QDir>

QDir dir(".");
```

Get files:

```cpp
QStringList files =
    dir.entryList();
```

Create directory:

```cpp
dir.mkdir("data");
```

Check:

```cpp
if (dir.exists("data"))
{
    qDebug() << "Directory exists";
}
```

Useful for:

```text
file browsers
logs
configuration directories
project data
```

---

# 15. `QFileInfo`

Provides information about a file.

```cpp
QFileInfo info("test.txt");
```

Check:

```cpp
info.exists();
info.isFile();
info.isDir();
```

Get size:

```cpp
qint64 size = info.size();
```

Get filename:

```cpp
QString name =
    info.fileName();
```

Get absolute path:

```cpp
QString path =
    info.absoluteFilePath();
```

---

# 16. `QStandardPaths`

This is useful for finding the correct system directories.

For example:

```cpp
QString path =
    QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation
    );
```

Other locations include:

```text
HomeLocation
TempLocation
DocumentsLocation
DownloadLocation
AppDataLocation
```

Instead of hardcoding:

```text
/home/when/...
```

you can ask Qt for the appropriate location.

---

# 17. `QSettings`

Stores application settings.

Example:

```cpp
QSettings settings(
    "MyCompany",
    "MyApplication"
);
```

Save:

```cpp
settings.setValue(
    "username",
    "When"
);
```

Save number:

```cpp
settings.setValue(
    "windowWidth",
    800
);
```

Read:

```cpp
QString username =
    settings.value(
        "username"
    ).toString();
```

Read with default:

```cpp
int width =
    settings.value(
        "windowWidth",
        600
    ).toInt();
```

Excellent for:

```text
window size
user preferences
serial port settings
baud rate
application configuration
```

---

# 18. `QRegularExpression`

Pattern matching.

```cpp
QRegularExpression regex("\\d+");

QString text = "Price 4354";
```

Find a match:

```cpp
QRegularExpressionMatch match =
    regex.match(text);
```

Check:

```cpp
if (match.hasMatch())
{
    qDebug() << "Number found";
}
```

Useful for parsing:

```text
numbers
emails
commands
protocol messages
structured text
```

---

# 19. `QDebug`

Qt's debugging output.

```cpp
#include <QDebug>

qDebug() << "Hello";
```

Multiple values:

```cpp
QString symbol = "XAUUSD";
double price = 4354.50;

qDebug() << symbol << price;
```

Output:

```text
"XAUUSD" 4354.5
```

Very useful while learning Qt.

---

# 20. `QElapsedTimer`

Measures elapsed time.

```cpp
QElapsedTimer timer;

timer.start();
```

Do some work:

```cpp
// some operation
```

Measure:

```cpp
qDebug() << timer.elapsed() << "ms";
```

Example output:

```text
125 ms
```

Useful for performance testing.

---

# 21. `QProcess`

Runs another program/process.

For example:

```cpp
QProcess process;

process.start("ls");
```

Wait:

```cpp
process.waitForFinished();
```

Read output:

```cpp
QByteArray output =
    process.readAllStandardOutput();
```

This is useful when your Qt application needs to interact with command-line programs.

For example:

```text
Qt GUI
   ↓
QProcess
   ↓
Python / Bash / external program
```

---

# 22. `QCommandLineParser`

Useful when your Qt application is also a CLI application.

Suppose:

```bash
./MyApp --port /dev/ttyUSB0
```

Qt can parse:

```text
--port
/dev/ttyUSB0
```

This is useful for building applications that support:

```bash
./MyApp --help
./MyApp --version
./MyApp --port /dev/ttyUSB0
```

---

# 23. `QCoreApplication`

This is the foundation of a **non-GUI Qt application**.

```cpp
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    return app.exec();
}
```

For GUI:

```cpp
QApplication
```

For non-GUI:

```cpp
QCoreApplication
```

Concept:

```text
QCoreApplication
       │
       ├── Event loop
       ├── arguments
       ├── application information
       └── QtCore functionality
```

---

# 24. `QObject`

One of the most important Qt classes.

```cpp
class MyClass : public QObject
{
    Q_OBJECT

public:
    MyClass();
};
```

`QObject` provides important Qt functionality such as:

```text
signals
slots
events
properties
object hierarchy
thread affinity
```

Many Qt classes inherit from `QObject`.

```text
QObject
   │
   ├── QTimer
   ├── QProcess
   ├── QThread
   ├── QSerialPort
   └── QWidget
```

---

# 25. `QThread`

QtCore also contains multithreading classes.

```cpp
QThread thread;

thread.start();
```

For serious work, you normally use:

```text
QThread
   +
Worker QObject
   +
Signals / Slots
```

rather than putting all your application logic directly into `QThread`.

---

# 26. `QMutex`

Protects shared data between threads.

```cpp
QMutex mutex;

mutex.lock();

// shared data

mutex.unlock();
```

Better:

```cpp
QMutexLocker locker(&mutex);
```

The lock automatically releases when the locker goes out of scope.

---

# 27. `QJsonObject`

QtCore also provides JSON classes.

```cpp
QJsonObject object;

object["symbol"] = "XAUUSD";
object["price"] = 4354.50;
```

Read:

```cpp
QString symbol =
    object["symbol"].toString();

double price =
    object["price"].toDouble();
```

---

# 28. `QJsonArray`

Multiple JSON values:

```cpp
QJsonArray prices;

prices.append(4350.10);
prices.append(4352.20);
prices.append(4354.50);
```

Conceptually:

```json
[
    4350.10,
    4352.20,
    4354.50
]
```

---

# 29. `QJsonDocument`

Converts JSON objects/arrays to JSON data.

```cpp
QJsonObject object;

object["symbol"] = "XAUUSD";
object["price"] = 4354.50;

QJsonDocument document(object);

QByteArray data =
    document.toJson();
```

Architecture:

```text
QJsonObject
     ↓
QJsonDocument
     ↓
QByteArray
```

This combination is very important when working with APIs.

---

# 30. `QUrl`

Represents a URL.

```cpp
QUrl url(
    "https://example.com/api"
);
```

Useful with networking.

You can inspect:

```cpp
url.scheme();
url.host();
url.path();
```

For example:

```text
https://example.com/api
│       │           │
│       │           └── path
│       └────────────── host
└────────────────────── scheme
```

---

# 31. `QStringList`

A list specifically designed for strings.

```cpp
QStringList names;

names << "When"
      << "John"
      << "Bob";
```

Or:

```cpp
names.append("Alice");
```

Loop:

```cpp
for (const QString &name : names)
{
    qDebug() << name;
}
```

Very useful with:

```text
QDir
QProcess
command-line arguments
text processing
```

---

# 32. One practical example

Let's combine the most useful QtCore utilities:

```cpp
#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString symbol = "XAUUSD";

    double price = 4354.50;

    QList<double> prices;

    prices.append(4350.10);
    prices.append(4352.20);
    prices.append(price);

    QDateTime now =
        QDateTime::currentDateTime();

    QString message =
        QString("%1 %2 %3")
        .arg(symbol)
        .arg(price)
        .arg(now.toString());

    qDebug() << message;

    QFile file("market.log");

    if (file.open(QIODevice::Append))
    {
        file.write(
            message.toUtf8()
        );

        file.write("\n");

        file.close();
    }

    return 0;
}
```

This small program uses:

```text
QCoreApplication
QString
QList
QDateTime
QFile
QDebug
```

That's already a useful little QtCore program.

---

# 33. QtCore Utility Classes — cheat sheet

| Class                | Purpose                     |
| -------------------- | --------------------------- |
| `QString`            | Text                        |
| `QChar`              | Character                   |
| `QByteArray`         | Raw bytes                   |
| `QVariant`           | Generic value               |
| `QList`              | List                        |
| `QVector`            | Dynamic array               |
| `QStringList`        | String list                 |
| `QMap`               | Key/value                   |
| `QHash`              | Hash key/value              |
| `QSet`               | Unique values               |
| `QDate`              | Date                        |
| `QTime`              | Time                        |
| `QDateTime`          | Date + time                 |
| `QTimer`             | Timed events                |
| `QFile`              | Files                       |
| `QDir`               | Directories                 |
| `QFileInfo`          | File information            |
| `QSettings`          | Application settings        |
| `QStandardPaths`     | Standard system paths       |
| `QRegularExpression` | Pattern matching            |
| `QDebug`             | Debug output                |
| `QElapsedTimer`      | Measure elapsed time        |
| `QProcess`           | Start external programs     |
| `QUrl`               | URLs                        |
| `QJsonObject`        | JSON object                 |
| `QJsonArray`         | JSON array                  |
| `QJsonDocument`      | JSON document               |
| `QCoreApplication`   | Core application/event loop |
| `QObject`            | Qt object foundation        |
| `QThread`            | Thread                      |
| `QMutex`             | Thread synchronization      |

---

# 34. The QtCore mental map

If you're learning Qt5 seriously, remember this:

```text
                         QtCore
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
      DATA                FILES              TIME
       │                   │                   │
   QString              QFile              QDate
   QByteArray           QDir               QTime
   QVariant             QFileInfo          QDateTime
       │
       ├── Containers
       │     ├── QList
       │     ├── QVector
       │     ├── QMap
       │     ├── QHash
       │     └── QSet
       │
       ├── JSON
       │     ├── QJsonObject
       │     ├── QJsonArray
       │     └── QJsonDocument
       │
       ├── Application
       │     ├── QObject
       │     ├── QCoreApplication
       │     └── QTimer
       │
       ├── System
       │     ├── QProcess
       │     ├── QStandardPaths
       │     └── QSettings
       │
       └── Threading
             ├── QThread
             ├── QMutex
             ├── QSemaphore
             └── QWaitCondition
```

### The 10 QtCore classes I'd learn first

```text
1.  QString
2.  QByteArray
3.  QList
4.  QMap
5.  QVariant
6.  QDateTime
7.  QFile
8.  QTimer
9.  QObject
10. QJsonObject
```

Once these are comfortable, **QtWidgets + QtCore + signals/slots** gives you the foundation for building serious Qt5 desktop applications.
