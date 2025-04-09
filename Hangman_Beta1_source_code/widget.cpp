#include "widget.h"
#include <QDebug>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

Widget::Widget(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    loadDictionary();
}

Widget::~Widget() {}

void Widget::loadDictionary() {
    qDebug() << "Current Working Directory: " << QDir::currentPath();
    QString dictionaryPath = QCoreApplication::applicationDirPath() + "/words.txt";

    qDebug() << "Loading words from:" << dictionaryPath;
    wordList = loadWordsFromFile(dictionaryPath);

    if (wordList.isEmpty()) {
        qDebug() << "Error: No words loaded! Check if words.txt exists in the same folder as the executable.";
    } else {
        qDebug() << "Loaded" << wordList.size() << "words.";
    }
}

QStringList Widget::loadWordsFromFile(const QString &filename) {
    QStringList words;
    QFile file(filename);

    if (!file.exists()) {
        qDebug() << "Error: words.txt not found at" << filename;
        return words;
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                words.append(line);
            }
        }
        file.close();
    } else {
        qDebug() << "Error: Could not open words.txt at" << filename;
    }

    return words;
}

QString Widget::random_word() {
    if (wordList.isEmpty()) {
        qDebug() << "Warning: wordList is empty! Using fallback word.";
        return "default";
    }
    int index = QRandomGenerator::global()->bounded(wordList.size());
    return wordList[index];
}

void Widget::guess_button_f() {
    qDebug() << "Guess Clicked";
    ct--;
    QString g_word = g_letter->text().trimmed().toLower();
    qDebug() << "Guessed word: " << g_word;
    qDebug() << "Actual word: " << word;
    remaining_att->setText(QString("Attempts Left: %1").arg(ct));
    if (word == g_word) {
        current->setText(word);
        status->setText("You won!");
        g_letter->clear();
        return;
    } else if (ct == 0) {
        current->setText(word);
        status->setText("You Lost!");
        return;
    }
    QString aux = current->text();
    bool found = false;
    if (g_word.size() == 1) {
        QChar guessedLetter = g_word[0];
        for (qsizetype i = 0; i < word.size(); ++i) {
            if (word[i] == guessedLetter) {
                aux[i] = guessedLetter;
                found = true;
            }
        }

        if (!found) {
            wrong_ans->addItem(g_word);
        }
    } else {
        for (qsizetype i = 0; i < word.size(); ++i) {
            if (i < g_word.size() && word[i] == g_word[i]) {
                aux[i] = word[i];
            }
        }
        wrong_ans->addItem(g_word);
    }

    current->setText(aux);
    g_letter->clear();
}

void Widget::new_button_f() {
    qDebug() << "New Game Clicked";
    ct = 7;
    word = random_word();
    qDebug() << "New word: " << word;
    QString aux(word.size(), '*');
    current->setText(aux);
    status->setText("Keep guessing...");
    remaining_att->setText(QString("Attempts Left: %1").arg(ct));
    wrong_ans->clear();
}

void Widget::setupUI() {
    QWidget *centralwidget = new QWidget(this);
    setCentralWidget(centralwidget);
    QVBoxLayout *mainLayout = new QVBoxLayout();

    current = new QLabel("Hello!", this);
    mainLayout->addWidget(current);

    g_letter = new QLineEdit(this);
    mainLayout->addWidget(g_letter);

    guess_button = new QPushButton("Guess", this);
    mainLayout->addWidget(guess_button);

    remaining_att = new QLabel("Attempts Left: 7", this);
    mainLayout->addWidget(remaining_att);

    wrong_ans = new QListWidget(this);
    mainLayout->addWidget(wrong_ans);

    status = new QLabel("Start the game!", this);
    mainLayout->addWidget(status);

    start = new QPushButton("New game", this);
    mainLayout->addWidget(start);

    centralwidget->setLayout(mainLayout);
    connect(guess_button, &QPushButton::clicked, this, &Widget::guess_button_f);
    connect(start, &QPushButton::clicked, this, &Widget::new_button_f);
}
