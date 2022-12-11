#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "./view.h"
#include <fstream>
#include <string>
#include <sstream>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::string readfile(const std::string& filename);
    ~MainWindow();

private:
    QPushButton *nextButton;
    QPushButton *fileDirButton;
    QLabel *header;
    QStackedWidget *stackedWidget;
    QString fileDir;
private slots:
   void onNextButtonClicked();
   void onFileDirButtonClicked();
};
#endif // MAINWINDOW_H
