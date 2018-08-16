#ifndef WORKER_H
#define WORKER_H

#include "mainwindow.h"

#include <QThread>

#include <QMutex>
#include <QWaitCondition>
#include <QFutureSynchronizer>

class Worker : public QThread {
  Q_OBJECT
public:
  explicit Worker(int);

private:
  int threads = 0;

  QAtomicInt stopped = 0;
  QAtomicInt pause = 0;
  bool exit = false;

  QMutex sync;
  QWaitCondition cond;

  QFutureSynchronizer<void> pool;

  void run();

  void task();
  void taskBody();

private slots:
  void stopThreads();

  void pauseThreads();
  void resumeThreads();

signals:
  void incrThreadDone();
};

#endif // WORKER_H
