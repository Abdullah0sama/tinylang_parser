#include "./view.h"
#include"./Scanner_V1.cpp"
View::View (QStackedWidget *stackedWidget, QWidget* parent, const std::string& content) : QWidget{parent} {
    this -> stackedWidget = stackedWidget;
    this -> fileContent = content;
    initialize();
}

View::~View () {
    delete imageWindow;
}
void View::initialize () {
    std::string tokensStr = scannerFunction(fileContent);
    QHBoxLayout* tophbox = new QHBoxLayout();
    QTextEdit* codeText = new QTextEdit();
    codeText -> setText(QString(fileContent.c_str()));
    QTextEdit* tokensText = new QTextEdit();
    tokensText -> setText(QString(tokensStr.c_str()));
    codeText -> setReadOnly(true);
    tokensText -> setReadOnly(true);
    codeText -> acceptRichText();
    tokensText -> setStyleSheet("background-color: #FAF8F1");
    codeText -> setStyleSheet("background-color: #FAF8F1");

    tophbox -> addWidget(codeText);
    tophbox -> addWidget(tokensText);

    nextBtn = new QPushButton("Generate Syntax Tree");
    backBtn = new QPushButton("back");
    QHBoxLayout* bottomhbox = new QHBoxLayout();
    bottomhbox -> setAlignment(Qt::AlignRight);
    bottomhbox -> addWidget(backBtn);
    bottomhbox -> addWidget(nextBtn);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox -> addLayout(tophbox);
    vbox -> addLayout(bottomhbox);

    connect(backBtn, SIGNAL (clicked()), this, SLOT (onBackBtnClicked()));
    connect(nextBtn, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));

    imageWindow = new QWidget();
    imageLabel = new QLabel(imageWindow);
}

void View::onBackBtnClicked () {
    stackedWidget -> removeWidget(this);
    delete this;
}

void View::onNextBtnClicked() {

    displayImage();
//    stackedWidget -> addWidget(new View(stackedWidget, this, content));
//    stackedWidget -> setCurrentIndex(stackedWidget -> count() - 1);
}

void View::displayImage() {
    system("./gen_syntaxtree.bash");

    QPixmap syntax_tree_image = QPixmap("./syntax_tree.png");
//    QLabel* display_image = new QLabel(imageWindow);
    imageLabel -> setPixmap(syntax_tree_image);
    imageWindow -> setFixedSize(syntax_tree_image.size());
    imageWindow -> show();
}
