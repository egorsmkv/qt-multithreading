#include "worker.h"
#include "mainwindow.h"

#include <QDebug>

#include <QtConcurrent/QtConcurrent>
#include <QTime>

Worker::Worker(int threads_count) { this->threads = threads_count; }

void Worker::run() {
  qDebug() << "Worker start";

  for (int i = 0; i < threads; ++i)
    pool.addFuture(QtConcurrent::run(this, &Worker::task));

  pool.waitForFinished();

  qDebug() << "Worker end";
}

void Worker::task() {
  forever {
    // pause block
    sync.lock();
    if (pause.load()) {
      qDebug() << "Task[" << QThread::currentThreadId() << "] paused";
      cond.wait(&sync);
    }
    sync.unlock();

    qDebug() << "Task[" << QThread::currentThreadId() << "] start";

    taskBody();

    qDebug() << "Task[" << QThread::currentThreadId() << "] end";
    break;
  }
}

void Worker::taskBody() {
  qsrand(qrand());

  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());
  int randInt = rand() % 11;

  qDebug() << "Task[" << QThread::currentThreadId() << "] sleep" << randInt
           << "seconds";

  QThread::sleep(randInt);

  emit incrThreadDone();
}

void Worker::pauseThreads() { pause = 1; }

void Worker::resumeThreads() {
  pause = 0;
  cond.wakeAll();
}

void Worker::stopThreads() {
  sync.lock();
  if (!stopped.load()) {
    stopped = 1;

    qDebug() << "Stop threads: start";

    if (pause)
      resumeThreads();

    for (auto f : pool.futures())
      f.cancel();

    qDebug() << "Stop threads: end";
  }
  sync.unlock();
}
