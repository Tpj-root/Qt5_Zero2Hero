# Qt5 Data Types — Zero to Hero Notes

Qt has its own data types in addition to normal C++ types.

The most important thing to understand is:

```text
C++ types                    Qt types

int          ─────────────→  int
double       ─────────────→  double
bool         ─────────────→  bool

char/string  ─────────────→  QChar / QString
array        ─────────────→  QList / QVector
map          ─────────────→  QMap
set          ─────────────→  QSet

date/time    ─────────────→  QDate / QTime / QDateTime
file         ─────────────→  QFile
JSON         ─────────────→  QJsonObject / QJsonArray
byte data    ─────────────→  QByteArray
```

Let's learn them one by one.

---

# 1. `QString` — Qt's main string type

This is probably the **most important Qt data type**.

```cpp
#include <QString>

QString name = "When";
```

You can use it with Qt widgets:

```cpp
QLabel *label = new QLabel;

label->setText(name);
```

### Concatenation

```cpp
QString first = "Hello";
QString second = "World";

QString message = first + " " + second;
```

Result:

```text
Hello World
```

### Convert number → QString

```cpp
int age = 25;

QString text = QString::number(age);
```

Now:

```text
text = "25"
```

### QString → int

```cpp
QString text = "123";

int number = text.toInt();
```

### QString → double

```cpp
QString text = "123.45";

double value = text.toDouble();
```

---

# 2. `QChar` — one character

`QChar` represents one Unicode character.

```cpp
#include <QChar>

QChar letter = 'A';
```

You can check it:

```cpp
if (letter == 'A')
{
    // ...
}
```

Example:

```cpp
QChar symbol = '₹';
```

So:

```text
QChar   → one character
QString → multiple characters
```

---

# 3. `QByteArray` — raw bytes

Very important for:

* Serial communication
* Bluetooth
* TCP/IP
* Files
* Binary protocols

```cpp
#include <QByteArray>

QByteArray data = "HELLO";
```

For serial communication:

```cpp
QByteArray data = serialPort->readAll();
```

Send data:

```cpp
serialPort->write("HELLO");
```

You can also build bytes:

```cpp
QByteArray data;

data.append(0x01);
data.append(0x02);
data.append(0x03);
```

Think:

```text
QString
   ↓
human-readable text

QByteArray
   ↓
raw bytes
```

---

# 4. `QList`

`QList` stores multiple values.

```cpp
#include <QList>

QList<int> numbers;
```

Add values:

```cpp
numbers.append(10);
numbers.append(20);
numbers.append(30);
```

Now:

```text
10
20
30
```

Access:

```cpp
int value = numbers[0];
```

Result:

```text
10
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

`QVector` is a dynamic array.

```cpp
#include <QVector>

QVector<int> numbers;

numbers.append(10);
numbers.append(20);
numbers.append(30);
```

Access:

```cpp
qDebug() << numbers[1];
```

Output:

```text
20
```

For learning Qt5, remember:

```text
QList
QVector
```

are both containers for multiple values.

---

# 6. `QMap`

`QMap` stores:

```text
KEY → VALUE
```

Example:

```cpp
#include <QMap>

QMap<QString, int> ages;

ages["When"] = 30;
ages["John"] = 25;
ages["Bob"] = 40;
```

Now:

```text
When → 30
John → 25
Bob  → 40
```

Access:

```cpp
int age = ages["When"];
```

Result:

```text
30
```

This is very useful for configuration/data lookup.

---

# 7. `QHash`

Similar to `QMap`:

```cpp
QHash<QString, int> ages;

ages["When"] = 30;
ages["John"] = 25;
```

Basic idea:

```text
QMap
 → ordered key/value container

QHash
 → hash-based key/value container
```

For basic Qt learning, learn `QMap` first.

---

# 8. `QSet`

`QSet` stores **unique values**.

```cpp
#include <QSet>

QSet<int> numbers;

numbers.insert(10);
numbers.insert(20);
numbers.insert(10);
```

The result contains:

```text
10
20
```

The duplicate `10` isn't stored twice.

Use it when uniqueness matters.

---

# 9. `QVariant`

`QVariant` is a very interesting Qt type.

It can hold different types of values.

```cpp
#include <QVariant>

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
int number = value.toInt();
```

or:

```cpp
QString text = value.toString();
```

It is useful when the exact type isn't fixed ahead of time.

---

# 10. `QDate`

Stores a calendar date.

```cpp
#include <QDate>

QDate date(2026, 9, 21);
```

Get today's date:

```cpp
QDate today = QDate::currentDate();
```

Print:

```cpp
qDebug() << today;
```

Format:

```cpp
QString text = today.toString("dd-MM-yyyy");
```

Example:

```text
21-09-2026
```

---

# 11. `QTime`

Stores a time.

```cpp
#include <QTime>

QTime time(19, 30, 0);
```

Current time:

```cpp
QTime now = QTime::currentTime();
```

Format:

```cpp
QString text = now.toString("HH:mm:ss");
```

Example:

```text
19:30:00
```

---

# 12. `QDateTime`

Combines date + time.

```cpp
#include <QDateTime>

QDateTime now = QDateTime::currentDateTime();
```

Format:

```cpp
QString text =
    now.toString("yyyy-MM-dd HH:mm:ss");
```

Example:

```text
2026-09-21 19:30:00
```

This is particularly useful for **logs, timestamps, trading data, serial logs, etc.**

---

# 13. `QUrl`

Stores URLs.

```cpp
#include <QUrl>

QUrl url("https://example.com");
```

Check:

```cpp
if (url.isValid())
{
    qDebug() << "Valid URL";
}
```

Useful with:

* HTTP
* WebSocket
* network applications

---

# 14. `QRegularExpression`

Used for pattern matching.

```cpp
#include <QRegularExpression>

QRegularExpression regex("\\d+");

QString text = "Price 1234";

QRegularExpressionMatch match =
    regex.match(text);
```

You can use it to find things such as:

```text
numbers
emails
specific patterns
```

---

# 15. `QJsonObject`

Very important when working with APIs.

Example JSON:

```json
{
    "symbol": "XAUUSD",
    "price": 4354.50
}
```

Qt:

```cpp
#include <QJsonObject>

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

# 16. `QJsonArray`

Stores multiple JSON values.

```cpp
#include <QJsonArray>

QJsonArray prices;

prices.append(4350.10);
prices.append(4351.20);
prices.append(4352.30);
```

Conceptually:

```text
[
    4350.10,
    4351.20,
    4352.30
]
```

---

# 17. `QJsonDocument`

Used to convert JSON structures into JSON text/bytes and back.

```cpp
#include <QJsonDocument>

QJsonObject object;

object["symbol"] = "XAUUSD";
object["price"] = 4354.50;

QJsonDocument document(object);

QByteArray json =
    document.toJson();
```

Now `json` contains JSON data.

This combination is important:

```text
QJsonObject
     ↓
QJsonDocument
     ↓
QByteArray
```

Very useful for APIs.

---

# 18. `QFile`

`QFile` isn't technically a simple "data type"; it's a Qt class for files.

But you will use it constantly.

```cpp
#include <QFile>

QFile file("log.txt");

if (file.open(QIODevice::WriteOnly))
{
    file.write("Hello Qt\n");
    file.close();
}
```

Reading:

```cpp
QFile file("log.txt");

if (file.open(QIODevice::ReadOnly))
{
    QByteArray data = file.readAll();

    file.close();
}
```

Notice the relationship:

```text
QFile
 ↓
QByteArray
 ↓
raw file data
```

---

# 19. `qint8`, `qint16`, `qint32`, `qint64`

Qt also provides fixed-width integer types.

```cpp
qint8   a;
qint16  b;
qint32  c;
qint64  d;
```

Unsigned versions:

```cpp
quint8
quint16
quint32
quint64
```

For example:

```cpp
quint8 value = 255;
```

This is especially useful in:

* embedded systems
* serial protocols
* binary data
* networking

---

# 20. `qsizetype`

Used for sizes and indexes in newer Qt APIs.

For example:

```cpp
qsizetype size = list.size();
```

For basic Qt5 learning, you don't need to focus heavily on it yet.

---

# 21. `QObject`

`QObject` is **not really a data type** like `int` or `QString`.

It is one of the most important Qt classes.

Many Qt classes inherit from it:

```text
QObject
   │
   ├── QWidget
   │     └── QMainWindow
   │
   ├── QTimer
   │
   └── QSerialPort
```

It provides important Qt features such as:

```text
signals
slots
events
parent-child ownership
```

---

# 22. Qt containers — important family

You should recognize these:

```text
QList
QVector
QMap
QHash
QSet
```

Think:

```text
QList<T>
    ↓
list of values

QVector<T>
    ↓
dynamic array

QMap<K,V>
    ↓
key → value

QHash<K,V>
    ↓
key → value

QSet<T>
    ↓
unique values
```

---

# 23. A practical example

Let's combine several Qt types.

```cpp
#include <QDebug>
#include <QString>
#include <QList>
#include <QMap>
#include <QDateTime>

int main()
{
    QString symbol = "XAUUSD";

    double price = 4354.50;

    QList<double> prices;

    prices.append(4350.10);
    prices.append(4352.20);
    prices.append(4354.50);

    QMap<QString, double> market;

    market["XAUUSD"] = 4354.50;
    market["EURUSD"] = 1.1720;

    QDateTime time =
        QDateTime::currentDateTime();

    qDebug() << "Symbol:" << symbol;
    qDebug() << "Price:" << price;
    qDebug() << "Prices:" << prices;
    qDebug() << "Market:" << market;
    qDebug() << "Time:" << time;
}
```

You now have:

```text
QString
    ↓
XAUUSD

double
    ↓
4354.50

QList<double>
    ↓
[4350.10, 4352.20, 4354.50]

QMap<QString, double>
    ↓
XAUUSD → 4354.50
EURUSD → 1.1720

QDateTime
    ↓
current date + time
```

---

# 24. What should you learn first?

Don't try to memorize every Qt type at once.

### Level 1 — absolutely important

```text
QString
QByteArray
QList
QVector
QMap
QVariant
```

### Level 2 — application data

```text
QDate
QTime
QDateTime
QUrl
QRegularExpression
```

### Level 3 — JSON/API

```text
QJsonObject
QJsonArray
QJsonDocument
QJsonValue
```

### Level 4 — fixed-size/binary data

```text
qint8
qint16
qint32
qint64

quint8
quint16
quint32
quint64
```

### Level 5 — Qt architecture

```text
QObject
QWidget
QMainWindow
QDialog
QTimer
```

---

# 25. The big picture

For your Qt5 learning, keep this mental map:

```text
                    Qt5
                     │
       ┌─────────────┼──────────────┐
       │             │              │
     Text          Containers      Data
       │             │              │
   QString         QList          QDate
   QChar           QVector         QTime
                   QMap            QDateTime
                   QHash           QUrl
                   QSet
       │
       └──────────────┐
                      │
                    Raw
                      │
                 QByteArray
                      │
              ┌───────┴────────┐
              │                │
           Serial             Network
              │                │
        QSerialPort          QTcpSocket
```

And for your future Qt applications:

```text
                 MainWindow
                     │
          ┌──────────┼──────────┐
          │          │          │
       QString     QList      QMap
          │          │          │
       QLabel     prices      settings
          │
      QPushButton
          │
       signals
          │
        slots
          │
     Your own class
```

**The key idea:** Qt data types are not separate from C++. They are C++ classes/types designed to work naturally with Qt's GUI, signals/slots, networking, files, JSON, and other framework features.
