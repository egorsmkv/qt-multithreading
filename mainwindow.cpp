#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "worker.h"

#include <QDebug>

#include <QThreadPool>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  emit sendStopThreads();

  delete ui;

  qDebug() << "UI destroyed";
}

void MainWindow::on_start_clicked() {

  // change buttons state
  ui->threadsDone->setValue(0);
  ui->start->setEnabled(false);
  ui->pause->setEnabled(true);
  ui->stop->setEnabled(true);

  // Set maximum threads
  QThreadPool::globalInstance()->setMaxThreadCount(ui->maxThreads->value());

  Worker *work = new Worker(ui->threads->value());

  connect(work, SIGNAL(incrThreadDone()), this, SLOT(incrThreadDoneChange()));

  connect(this, SIGNAL(sendPause()), work, SLOT(pauseThreads()));
  connect(this, SIGNAL(sendResume()), work, SLOT(resumeThreads()));

  connect(this, SIGNAL(sendStopThreads()), work, SLOT(stopThreads()));

  connect(work, SIGNAL(finished()), this, SLOT(threadFinished()));

  work->start();
}

void MainWindow::on_pause_clicked() {
  qDebug() << "Set pause";

  // change buttons state
  ui->pause->setEnabled(false);
  ui->resume->setEnabled(true);

  emit sendPause();
}

void MainWindow::threadFinished() {
  qDebug() << "All threads done";

  // change buttons state
  ui->start->setEnabled(true);
  ui->pause->setEnabled(false);
  ui->resume->setEnabled(false);

  ui->stop->setEnabled(false);
}

void MainWindow::on_resume_clicked() {
  qDebug() << "Resume `s";

  ui->pause->setEnabled(true);
  ui->resume->setEnabled(false);

  emit sendResume();
}

void MainWindow::on_stop_clicked() {
  // change buttons state

  ui->resume->setEnabled(false);
  ui->start->setEnabled(false);
  ui->pause->setEnabled(false);
  ui->stop->setEnabled(false);

  emit sendStopThreads();
}

void MainWindow::incrThreadDoneChange() { ui->threadsDone->stepUp(); }
