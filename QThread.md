# Qt5 Multithreading — Zero to Hero Notes

Qt5 gives you several classes for doing work concurrently. The important thing is to understand **which class is for what**, because `QThread`, `QtConcurrent`, `QMutex`, etc. solve different problems.

![Image](https://images.openai.com/static-rsc-4/2wbDvX-ZWJGfEasaq8Lsh2O-mAaJ9x8_yfpeEuCf1En-bsZU3J_lGUxJWoIldE3u88zYdOiNS2rCTtwEygU0SHNCp3QHyXWRNqrx7IiymLLvJq6yndwkIAJoqdfoIWJgzQYbN5i-Lq0_79hy6wEIOALjqV7Y9EJU8Ptv_4SEoqIErv_q6xXHmLZwO7HdF3Bx?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/qpVR6gF4Gbpxe5fkBgTzQqa6kmEOBuhHVaPqgF3qH1pPj9YxigwfzdhQmc9tgH6OboMuJplOGCupbDj4EYs5LD3SBK6nyJIv7dTjNbQFBDqrRnUhFs2_dl4EQJ11B_beyyLFbSVWjGvLIncOAtYG_k4-cbtCX64sqazo1Aw5uqUAYfF57ujYXv229Jm4WcN2?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/EViofHVLFkh3GOlDcuFhR59zfzO_t-W4yZpaKs8DnR6i7dRHyMiz9cqWLaea4ea1am90Zpxcpk4tU83vBvHe0dpj_LwzQLyTPt-V-lDgqbOt-v61l1f8fF_E6nDYV8bwSjNOX0lAiGSfq1GWtcYN6If5tKPSEeeCMgSnbuKTIAj1uq_X1RxGGLqD5wGA1F_R?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/5Bc59QdkuVi6sdF3ZJKXgMbRX-TTyGgDqENy0JNuEJwy21LI_lBBDzHECuGYmEPiD-PoCh62QvvWMPNv7o1xCl5HPPEUgeJu_IFlR3Clbuz0HyCoFLXGUZ5GXcL-a6sG8tjQIcxU3cEhm2-G44LcW0gdLgBAtk0xLHzTdWqQoX4ghEg3Y_fyXDKnv3e-o8Wu?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/6yWtycqpeGjeu7g2_jVYYkRmwa1svwMQsS2eeQz0dLUyQ-7gNJ_fO1eZHpdzG8K0OBNVjlHEHuZa7T-nSVzQIMxxeB3Ini9LpJD6Tn0iGzj9Q2s5cA7hWZMDPwlyWlvduOGWsVFSM74QV2iw6bmOM1twSuUK22hq9gKy7MBu_Qw6SCydooWlegu3Skk4n80x?purpose=fullsize)

![Image](https://images.openai.com/static-rsc-4/GdQdYwv5Safc0TtuioXpw84fe7b_SwlfDGgSN2TMWQy2IYn99dwsNEiMG-qUF-mHxtKx_m9lTlk0hBHQf-UNtXereTwtReQP9aIAKlJ51ampVuh8J5SNFjVfDMsbyMzuAEH0DvBUgkrENqrMNWRtg4gO0j3jyeLMCooxAAoyG_nXdc9jut3IoCHj-fdnM5p7?purpose=fullsize)

---

# 1. First: What is a thread?

Normally your program starts with one thread:

```text
Application
     │
     ▼
 Main Thread
     │
     ├── GUI
     ├── Buttons
     ├── Events
     └── ...
```

If you perform a long operation in the GUI thread:

```cpp
while (true)
{
    // very long work
}
```

the GUI can stop responding.

```text
GUI Thread
│
├── Button
├── Window
├── Paint
└── LONG WORK  ← blocks everything
```

So we can move long-running work to another thread:

```text
                 Application
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
     Main Thread            Worker Thread
          │                     │
       GUI work             Heavy work
```

---

# 2. Main Qt multithreading classes

These are the important ones:

| Class            | Main purpose                              |
| ---------------- | ----------------------------------------- |
| `QThread`        | Create/manage a thread                    |
| `QObject`        | Worker object that can live in a thread   |
| `QMutex`         | Protect shared data                       |
| `QMutexLocker`   | Automatically lock/unlock mutex           |
| `QReadWriteLock` | Multiple readers / exclusive writer       |
| `QReadLocker`    | Read lock helper                          |
| `QWriteLocker`   | Write lock helper                         |
| `QSemaphore`     | Limit/count concurrent access             |
| `QWaitCondition` | Thread waits until another thread signals |
| `QThreadPool`    | Manage a pool of reusable threads         |
| `QRunnable`      | A task executed by `QThreadPool`          |
| `QtConcurrent`   | High-level parallel operations            |
| `QFuture`        | Represents asynchronous computation       |
| `QFutureWatcher` | Monitor a `QFuture`                       |

You don't need all of them immediately.

The most important learning path is:

```text
QThread
   ↓
Worker QObject
   ↓
Signals / Slots
   ↓
QMutex
   ↓
QThreadPool / QRunnable
   ↓
QtConcurrent
```

---

# 3. `QThread`

`QThread` represents a thread of execution.

Basic example:

```cpp
#include <QThread>

QThread thread;

thread.start();
```

Stop:

```cpp
thread.quit();
thread.wait();
```

But there is an important concept:

> **Don't think of `QThread` as your worker class.**

A common beginner mistake is putting all the work directly inside a subclass of `QThread`.

Qt's recommended pattern is generally:

```text
QThread
   │
   └── Worker QObject
```

---

# 4. Worker `QObject` + `QThread`

This is the most important Qt5 multithreading pattern.

Imagine:

```text
MainWindow
    │
    │ signal
    ▼
 Worker
    │
    │ performs work
    ▼
Background thread
```

---

## Worker.h

```cpp
#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);

public slots:
    void doWork();

signals:
    void resultReady(int result);
};

#endif
```

Notice:

```cpp
class Worker : public QObject
```

Worker is a normal Qt object.

And:

```cpp
Q_OBJECT
```

enables Qt's meta-object features, including signals and slots.

---

# 5. Worker.cpp

```cpp
#include "Worker.h"
#include <QThread>

Worker::Worker(QObject *parent)
    : QObject(parent)
{
}

void Worker::doWork()
{
    int result = 0;

    for (int i = 0; i < 100000000; ++i)
    {
        result += i;
    }

    emit resultReady(result);
}
```

The important part:

```cpp
emit resultReady(result);
```

Worker sends the result back through a Qt signal.

---

# 6. MainWindow creates the thread

```cpp
QThread *thread = new QThread;

Worker *worker = new Worker;

worker->moveToThread(thread);
```

This is extremely important:

```cpp
worker->moveToThread(thread);
```

It means:

> The Worker QObject now belongs to that thread's execution context.

Then:

```cpp
connect(thread,
        &QThread::started,
        worker,
        &Worker::doWork);
```

When the thread starts:

```text
QThread started
      ↓
Worker::doWork()
```

Then:

```cpp
thread->start();
```

---

# 7. Complete architecture

```text
                 MAIN THREAD
              ┌───────────────┐
              │   MainWindow  │
              │      GUI      │
              └───────┬───────┘
                      │
                  signal
                      │
                      ▼
              ┌───────────────┐
              │    Worker     │
              └───────┬───────┘
                      │
                background
                  thread
                      │
                      ▼
                 Heavy Work
                      │
                   result
                      │
                  signal
                      │
                      ▼
              ┌───────────────┐
              │   MainWindow  │
              └───────────────┘
```

This is the architecture you should understand very well.

---

# 8. Why Signals/Slots are important

Instead of doing this:

```cpp
worker->result = 100;
```

you can do:

```cpp
emit resultReady(100);
```

Qt can deliver that signal to another object's slot.

For example:

```cpp
connect(worker,
        &Worker::resultReady,
        this,
        &MainWindow::showResult);
```

Now:

```text
Worker thread
     │
     │ resultReady(100)
     ▼
Qt signal/slot system
     │
     ▼
MainWindow
```

This is much safer than randomly sharing GUI objects between threads.

---

# 9. GUI rule — VERY IMPORTANT

**Qt GUI widgets should normally be accessed only from the main GUI thread.**

Don't do:

```cpp
// Worker thread
ui->label->setText("Hello");
```

Instead:

```text
Worker thread
     │
     │ emit resultReady()
     ▼
Main thread
     │
     ▼
label->setText()
```

Example:

```cpp
void MainWindow::showResult(int result)
{
    ui->label->setText(
        QString::number(result)
    );
}
```

---

# 10. `QMutex`

Now imagine two threads share:

```cpp
int counter;
```

Thread A:

```cpp
counter++;
```

Thread B:

```cpp
counter++;
```

Both can access the same memory.

This can cause a **race condition**.

```text
Thread A ──┐
           ├──> counter
Thread B ──┘
```

A `QMutex` protects the shared resource.

```cpp
#include <QMutex>

QMutex mutex;
```

Then:

```cpp
mutex.lock();

counter++;

mutex.unlock();
```

Only one thread can hold the mutex at a time.

---

# 11. `QMutexLocker`

Better:

```cpp
QMutexLocker locker(&mutex);

counter++;
```

When `locker` is created:

```text
LOCK
```

When it goes out of scope:

```text
UNLOCK
```

This is safer because you don't accidentally forget:

```cpp
mutex.unlock();
```

Concept:

```text
{
    QMutexLocker locker(&mutex);

    // protected code

} // automatically unlocks
```

---

# 12. `QReadWriteLock`

Sometimes you have:

```text
many readers
+
one writer
```

For example:

```text
Thread 1 → READ
Thread 2 → READ
Thread 3 → READ
Thread 4 → WRITE
```

Use:

```cpp
QReadWriteLock lock;
```

Read:

```cpp
lock.lockForRead();

// read data

lock.unlock();
```

Write:

```cpp
lock.lockForWrite();

// modify data

lock.unlock();
```

Concept:

```text
READ
READ
READ
```

can happen together.

But:

```text
WRITE
```

needs exclusive access.

---

# 13. `QSemaphore`

A semaphore controls how many threads can access something.

Example:

```cpp
QSemaphore semaphore(3);
```

This allows up to **3 resources/operations** at once.

Acquire:

```cpp
semaphore.acquire();
```

Release:

```cpp
semaphore.release();
```

Concept:

```text
Semaphore = 3

Thread A → resource 1
Thread B → resource 2
Thread C → resource 3

Thread D → WAIT
```

When one releases:

```text
Thread A → release

Thread D → can continue
```

---

# 14. `QWaitCondition`

This allows a thread to sleep until another thread tells it:

> Something is ready.

Example concept:

```text
Producer
   │
   │ creates data
   ▼
Shared buffer
   │
   │ wake
   ▼
Consumer
```

Classes:

```cpp
QWaitCondition
QMutex
```

Producer:

```cpp
condition.wakeOne();
```

Consumer:

```cpp
condition.wait(&mutex);
```

This is useful for **producer/consumer** designs.

---

# 15. `QThreadPool`

Creating and destroying threads repeatedly can be expensive.

Instead:

```text
QThreadPool
    │
    ├── Thread 1
    ├── Thread 2
    ├── Thread 3
    └── Thread 4
```

You submit small tasks.

```cpp
QThreadPool *pool =
    QThreadPool::globalInstance();
```

Then run a `QRunnable`.

---

# 16. `QRunnable`

`QRunnable` represents a task.

Example:

```cpp
class MyTask : public QRunnable
{
public:

    void run() override
    {
        // background work
    }
};
```

Run it:

```cpp
QThreadPool::globalInstance()->start(
    new MyTask
);
```

Concept:

```text
                QThreadPool
                     │
          ┌──────────┼──────────┐
          ▼          ▼          ▼
       Task A     Task B     Task C
          │          │          │
       Thread 1   Thread 2   Thread 3
```

This is excellent for **many small independent jobs**.

---

# 17. `QtConcurrent`

`QtConcurrent` gives you a higher-level way to perform parallel operations.

For example:

```cpp
#include <QtConcurrent>

QtConcurrent::run([]()
{
    // background work
});
```

You don't manually create the thread.

Concept:

```text
You
 │
 ▼
QtConcurrent
 │
 ▼
QThreadPool
 │
 ▼
Worker thread
```

This is convenient when you don't need a custom worker object's signals/slots architecture.

---

# 18. `QFuture`

`QFuture` represents the result of an asynchronous operation.

Conceptually:

```text
Start calculation
       │
       ▼
    QFuture
       │
       │ calculation running
       ▼
    Result
```

For example:

```cpp
QFuture<int> future =
    QtConcurrent::run([]()
{
    return 100 + 200;
});
```

Later:

```cpp
int result = future.result();
```

Result:

```text
300
```

---

# 19. `QFutureWatcher`

`QFutureWatcher` allows your application to monitor a `QFuture`.

It can notify you when:

```text
started
finished
progress changed
result ready
```

Concept:

```text
QtConcurrent
      │
      ▼
   QFuture
      │
      ▼
QFutureWatcher
      │
      ├── started
      ├── progress
      └── finished
```

This is useful for GUI applications where you want a progress bar.

---

# 20. `QThread::sleep()`

You may see:

```cpp
QThread::sleep(1);
```

It pauses the current thread for one second.

Also:

```cpp
QThread::msleep(100);
```

100 milliseconds.

Important:

**Do not use sleep in the GUI thread just to create delays**, because it freezes the GUI.

Bad:

```cpp
// GUI thread
QThread::sleep(5);
```

The window can become unresponsive.

---

# 21. `QThread::msleep()` in a worker

Inside a worker thread, it can sometimes be appropriate:

```cpp
void Worker::doWork()
{
    for (int i = 0; i < 10; ++i)
    {
        QThread::msleep(100);

        emit progress(i);
    }
}
```

But for event-driven Qt applications, timers and asynchronous APIs are often preferable to blocking sleeps.

---

# 22. Thread affinity

This is an important Qt concept.

Every `QObject` has **thread affinity**.

You can think:

```text
QObject
   │
   └── belongs to a thread
```

Check:

```cpp
QThread *thread =
    object->thread();
```

Move:

```cpp
object->moveToThread(workerThread);
```

This is one of the key ideas behind Qt's worker-object pattern.

---

# 23. Parent-child ownership and threads

Qt objects can have parents:

```cpp
QPushButton *button =
    new QPushButton(this);
```

`this` becomes the parent.

Qt automatically manages the child object's lifetime.

But there is an important threading restriction:

> A QObject with a parent cannot simply be moved to another thread while retaining that parent relationship.

So worker objects are commonly created without a GUI-thread parent and then moved:

```cpp
Worker *worker = new Worker;

worker->moveToThread(thread);
```

---

# 24. `QThread` lifecycle

A clean pattern looks like:

```text
Create QThread
      ↓
Create Worker
      ↓
move Worker to thread
      ↓
connect signals/slots
      ↓
thread.start()
      ↓
Worker works
      ↓
Worker finishes
      ↓
thread.quit()
      ↓
thread.wait()
      ↓
cleanup
```

For example:

```cpp
connect(worker,
        &Worker::finished,
        thread,
        &QThread::quit);

connect(worker,
        &Worker::finished,
        worker,
        &QObject::deleteLater);

connect(thread,
        &QThread::finished,
        thread,
        &QObject::deleteLater);
```

This is a common Qt pattern.

---

# 25. `QThread` vs `QRunnable`

This is an important comparison.

| `QThread` + Worker      | `QRunnable`                |
| ----------------------- | -------------------------- |
| Long-running worker     | Short task                 |
| Signals/slots           | Usually simpler task       |
| Custom worker object    | Just `run()`               |
| More control            | Easy thread-pool execution |
| Good for serial manager | Good for many small jobs   |

For example:

### Serial communication

```text
QThread
   ↓
SerialWorker
   ↓
QSerialPort
```

### Image processing jobs

```text
QThreadPool
   ↓
QRunnable
   ↓
Image processing
```

---

# 26. `QThread` vs `QtConcurrent`

Another important comparison:

```text
QThread
   ↓
You manage the worker architecture
```

while:

```text
QtConcurrent
   ↓
Qt manages much of the threading
```

Example:

```cpp
QtConcurrent::run([]()
{
    calculateSomething();
});
```

Very simple.

But if you need:

```text
start
stop
pause
resume
progress
signals
multiple operations
serial communication
state
```

a dedicated `QObject` worker + `QThread` architecture is often easier to structure.

---

# 27. Your future Qt Serial application

For your hardware/serial project, a clean architecture could be:

```text
                    Main Thread
                ┌─────────────────┐
                │   MainWindow    │
                │                 │
                │ Button          │
                │ QLabel          │
                │ QTextEdit       │
                └────────┬────────┘
                         │
                    signals/slots
                         │
                         ▼
                ┌─────────────────┐
                │ SerialWorker    │
                │                 │
                │ QSerialPort     │
                └────────┬────────┘
                         │
                    Worker Thread
                         │
                         ▼
                  /dev/ttyUSB0
                         │
                         ▼
                     Arduino
```

This is exactly the kind of architecture worth learning before building a large Qt application.

---

# 28. The golden rules

Remember these **7 rules**:

### Rule 1

```text
GUI → Main thread
```

### Rule 2

```text
Heavy work → Worker thread
```

### Rule 3

```text
Worker → Signals
MainWindow → Slots
```

### Rule 4

Don't directly modify GUI widgets from a worker thread.

### Rule 5

Shared data needs synchronization.

```text
QMutex
QReadWriteLock
QSemaphore
```

### Rule 6

Use:

```text
QThread + Worker QObject
```

when you need a persistent/custom worker.

### Rule 7

Use:

```text
QThreadPool + QRunnable
```

or:

```text
QtConcurrent
```

when you have independent tasks that don't need a dedicated worker object.

---

# 29. Zero → Hero learning order

For your Qt5 OOP learning, I would learn multithreading in this exact order:

```text
01. What is a thread?
        ↓
02. Main GUI thread
        ↓
03. QThread
        ↓
04. QObject
        ↓
05. moveToThread()
        ↓
06. Signals / Slots between threads
        ↓
07. Worker-object pattern
        ↓
08. Thread cleanup
        ↓
09. QMutex
        ↓
10. QMutexLocker
        ↓
11. QReadWriteLock
        ↓
12. QSemaphore
        ↓
13. QWaitCondition
        ↓
14. QThreadPool
        ↓
15. QRunnable
        ↓
16. QtConcurrent
        ↓
17. QFuture
        ↓
18. QFutureWatcher
```

**Most important milestone:** once you understand **`QThread + QObject worker + moveToThread() + signals/slots`**, you understand the core Qt5 multithreading architecture.
