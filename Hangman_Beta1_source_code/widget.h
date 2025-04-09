#ifndef WIDGET_H
#define WIDGET_H
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QStringList>
#include <QString>
#include <QLabel>
#include <QListWidget>
#include <QRandomGenerator>
class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void setupUI();
    void loadDictionary();
    QStringList loadWordsFromFile(const QString &filename);

    QString random_word();
    QLabel* current;
    QLineEdit* g_letter;
    QPushButton* guess_button;
    QLabel* remaining_att;
    QLabel* status;
    QListWidget* wrong_ans;
    QPushButton* start;
    QString word;
    QStringList wordList;
    int ct=7;

private slots:
    void guess_button_f();
    void new_button_f();

};
#endif
