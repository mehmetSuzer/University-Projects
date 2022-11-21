#ifndef DISPLAY_H
#define DISPLAY_H

#include <QLineEdit>
#include <QPushButton>

class Display : public QWidget {
    Q_OBJECT
public:
    Display(int width);
    QLineEdit *lineEdit;
    QString first; // first operand
    QString op;
    QString mem_op; // Helper variable
    QString second; // Second operand
    bool equal_button_pressed; // Helper variable

public slots:
    void add_digit();
    void change_op();
    void display_result();
    void clear();

public:
    QPushButton *createButton(const QString &text, const char *slot);
};

#endif // DISPLAY_H
