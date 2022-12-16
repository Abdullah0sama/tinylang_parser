#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <string>
#include <QTextEdit>
#include <QPushButton>
#include <QDebug>
#include <QPixmap>
#include <vector>
#include <fstream>
#include <QMessageBox>

class View: public QWidget {
    Q_OBJECT    
public:
    View (QStackedWidget* stackedWidget, QWidget* parent, const std::string& content);
    void initialize();
    void displayImage();
    void preprocess(std::vector<std::vector<std::string>>& tokens);
    void scan();
    void showError(QString str);
    ~View();
private:
    QStackedWidget *stackedWidget;
    std::string fileContent;
    QPushButton* backBtn;
    QPushButton* nextBtn;
    QPushButton* scanBtn;
    QWidget* imageWindow;
    QLabel* imageLabel;
    QTextEdit* codeText;
    QTextEdit* tokensText;
    std::string dotFileContent;
    bool isParseSuccess = false;
private slots:
    void onBackBtnClicked();
    void onNextBtnClicked();
    void onScanBtnClicked();
};

#endif // VIEW_H
