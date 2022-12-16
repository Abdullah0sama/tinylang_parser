#include "./view.h"
#include"./parser/Scanner.h"
#include "convertSyntaxTree.cpp"

View::View (QStackedWidget *stackedWidget, QWidget* parent, const std::string& content) : QWidget{parent} {
    this -> stackedWidget = stackedWidget;
    this -> fileContent = content;
    initialize();
}

View::~View () {
    delete imageWindow;
}
void View::initialize () {

    QHBoxLayout* tophbox = new QHBoxLayout();
    codeText = new QTextEdit();
    codeText -> setText(QString(fileContent.c_str()));
    tokensText = new QTextEdit();
    codeText -> setReadOnly(false);
    tokensText -> setReadOnly(true);
    codeText -> acceptRichText();
    tokensText -> setStyleSheet("background-color: #FAF8F1");
    codeText -> setStyleSheet("background-color: #FAF8F1");

    tophbox -> addWidget(codeText);
    tophbox -> addWidget(tokensText);

    scan();

    nextBtn = new QPushButton("Generate Syntax Tree");
    scanBtn = new QPushButton("Scan Changes");
    backBtn = new QPushButton("back");
    QHBoxLayout* bottomhbox = new QHBoxLayout();
    bottomhbox -> setAlignment(Qt::AlignRight);
    bottomhbox -> addWidget(backBtn);
    bottomhbox -> addWidget(scanBtn);
    bottomhbox -> addWidget(nextBtn);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox -> addLayout(tophbox);
    vbox -> addLayout(bottomhbox);

    connect(backBtn, SIGNAL (clicked()), this, SLOT (onBackBtnClicked()));
    connect(nextBtn, SIGNAL (clicked()), this, SLOT (onNextBtnClicked()));
    connect(scanBtn, SIGNAL (clicked()), this, SLOT (onScanBtnClicked()));


    imageLabel = new QLabel();
    imageWindow = new QWidget();
    QVBoxLayout* vb = new QVBoxLayout();
    vb -> addWidget(imageLabel);
//    vb -> setMargin(0);
    imageWindow -> setLayout(vb);
    vb -> addWidget(imageLabel);
}

void View::onBackBtnClicked () {
    stackedWidget -> removeWidget(this);
    delete this;
}

void View::onNextBtnClicked() {

    displayImage();
}

void View::scan() {
    fileContent = (codeText -> toPlainText()).toStdString();
    std::vector<std::vector<std::string>> tokens = scannerFunction(fileContent);
    std::string tokensStr = tokensString(tokens);
    preprocess(tokens);
    tokensText ->setText(tokensStr.c_str());
    preprocess(tokens);
}

void View::onScanBtnClicked() {
    scan();
}

void View::preprocess(vector<vector<string>>& tokens) {
    vector<string> tokenT{};
    vector<string> tokenV{};

    for (size_t i = 0; i < tokens.size(); i++) {

        tokenT.push_back(tokens.at(i).at(0));
        tokenV.push_back(tokens.at(i).at(1));
    }

    Parser syntaxTree (tokenT, tokenV);
    if(syntaxTree.errorFlag) {
        isParseSuccess =false;
    } else {
        isParseSuccess = true;
        dotFileContent = convertTree(syntaxTree.getRoot());
    }

}
void View::displayImage() {

    if (fileContent.empty()) {
        showError("Input is empty");
        return;
    }

    if (tokensText->toPlainText().isEmpty()) {
        showError("No tokens");
        return;
    }

    if (!isParseSuccess) {
        QMessageBox* somethingWrong = new QMessageBox(QMessageBox::Warning, "Warning", "", QMessageBox::Close);
        somethingWrong -> setText("Something went wrong during parsing!");
        somethingWrong -> exec();

        return;
    }
    std::ofstream out;
    out.open("syntax_tree.dot");
    out << dotFileContent;
    out.close();
    system("gen_syntaxtree.bat");

    QPixmap syntax_tree_image = QPixmap("./syntax_tree.png");
    imageLabel -> setPixmap(syntax_tree_image);
    imageLabel-> setScaledContents(true);
    imageLabel ->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    imageWindow -> resize(syntax_tree_image.size());
    imageWindow -> show();
}

void View::showError(QString str) {
    QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, "Warning", "", QMessageBox::Close);
    msgBox -> setText(str);
    msgBox -> exec();
}
