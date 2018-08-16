#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void threadFinished();

  void on_start_clicked();
  void on_pause_clicked();
  void on_resume_clicked();
  void on_stop_clicked();

public slots:
  void incrThreadDoneChange();

signals:
  void sendPause();
  void sendResume();

  void sendKillThreads();
  void sendStopThreads();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
