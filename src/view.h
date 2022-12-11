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
class View: public QWidget {
    Q_OBJECT    
public:
    View (QStackedWidget* stackedWidget, QWidget* parent, const std::string& content);
    void initialize();
    void displayImage();
    ~View();
private:
    QStackedWidget *stackedWidget;
    std::string fileContent;
    QPushButton* backBtn;
    QPushButton* nextBtn;
    QWidget* imageWindow;
    QLabel* imageLabel;

private slots:
    void onBackBtnClicked();
    void onNextBtnClicked();
};

#endif // VIEW_H
