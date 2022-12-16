#include "mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    setStyleSheet("background: #FAEAB1; color: #000000 ");
    header = new QLabel("Tiny Lang Parser");
    header -> setAlignment(Qt::AlignCenter);
    header -> setStyleSheet("QLabel { font-size: 100px; font-weight:bold; \
                            margin: 70px; color: #FFB72B; }");



    fileDirButton = new QPushButton("Choose a file.");
    nextButton = new QPushButton("Next");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout -> addWidget(header);
    mainLayout -> addWidget(fileDirButton);
    mainLayout -> addWidget(nextButton);
    mainLayout -> setAlignment(Qt::AlignCenter);

    QWidget *wid = new QWidget(this);
    wid -> setLayout(mainLayout);
    stackedWidget = new QStackedWidget();
    stackedWidget -> addWidget(wid);
    setCentralWidget(stackedWidget);
    setMinimumSize(1000, 600);

    connect(nextButton, SIGNAL (clicked()), this, SLOT (onNextButtonClicked()));
    connect(fileDirButton, SIGNAL (clicked()), this, SLOT (onFileDirButtonClicked()));

}

MainWindow::~MainWindow()
{
}


void MainWindow::onNextButtonClicked() {

    if (fileDir == "") {
        qDebug("Something went wrong!");
        QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, "Warning", "", QMessageBox::Close);
        msgBox -> setText("No file was chosen!");
        msgBox -> exec();

        return;
    }
    qDebug("Next is clicked");
    try {
        std::string content = readfile(fileDir.toStdString());
//      qDebug() << (QString(content.c_str()));
        stackedWidget -> addWidget(new View(stackedWidget, this, content));
        stackedWidget -> setCurrentIndex(stackedWidget -> count() - 1);
    } catch (...) {
        QMessageBox* somethingWrong = new QMessageBox(QMessageBox::Warning, "Warning", "", QMessageBox::Close);
        somethingWrong -> setText("Something went wrong during while reading the file!");
        somethingWrong -> exec();
    }



}

void MainWindow::onFileDirButtonClicked() {
    fileDir = (QFileDialog::getOpenFileName());
    if(fileDir == "") fileDirButton -> setText("Choose a file.");
    else fileDirButton -> setText(fileDir);
    qDebug() << fileDir;
}

std::string MainWindow::readfile (const std::string& filename) {
    std::ifstream in;
    in.open(filename);
    std::stringstream stream;
    stream << in.rdbuf();
    std::string content = stream.str();
    in.close();
    return content;
}

