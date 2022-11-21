

#include "display.h"

Display::Display(int width) { // Constructor
    lineEdit = new QLineEdit();
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setFixedWidth(width);
    first = "0";
    op = "";
    mem_op = "";
    second = "";
    equal_button_pressed = false;
    this->lineEdit->setText(first);
}

void Display::add_digit() {
    QPushButton *clickedButton = qobject_cast<QPushButton* >(sender());
    QString digit = clickedButton->text(); // get the string corresponding to the button pressed
    if (op == "" && mem_op == "") { // if the operator is not selected then the number entered goes to first
        if (first == "0")
            first = digit;
        else
            first = first + digit;
        this->lineEdit->setText(first);
    }
    else if (op == "" && mem_op != "") { // if the operator is selected then the number entered goes to second
        second = second + digit;
        this->lineEdit->setText(second);
    }
    else if (op != "") { // if all selected, then the number entered goes to second
        second = second + digit;
        mem_op = op;
        op = "";
        this->lineEdit->setText(second);
    }
    equal_button_pressed = false;
}

void Display::change_op() { // Triggered when + or - is pressed
    QPushButton *clickedButton = qobject_cast<QPushButton* >(sender());
    QString operation = clickedButton->text();
    op = operation;
    if (!equal_button_pressed) { // if the equal button is not pressed, then previous operation is done
        if (mem_op == "+")
            first = QString::number(first.toLong(NULL, 16)+second.toLong(NULL, 16), 16);
        else if (mem_op == "-")
            first = QString::number(first.toLong(NULL, 16)-second.toLong(NULL, 16), 16);
        equal_button_pressed = false;
    }
    first = first.toUpper();
    this->lineEdit->setText(first); // replace first with the result of the previous operation
    mem_op = "";
    second = "";
}

void Display::display_result() { // Triggered when = is pressed
    if (mem_op == "+")
        first = QString::number(first.toLong(NULL, 16)+second.toLong(NULL, 16), 16);
    else if (mem_op == "-")
        first = QString::number(first.toLong(NULL, 16)-second.toLong(NULL, 16), 16);
    first = first.toUpper();
    this->lineEdit->setText(first); // calculate the result and update first
    equal_button_pressed = true;
}

void Display::clear() { // Triggered when Clr is pressed
    first = "0";        // Reset all the variable
    op = "";
    mem_op = "";
    second = "";
    this->lineEdit->setText(first);
}

QPushButton *Display::createButton(const QString &text, const char *slot) { // This function is written to handle connections of buttons
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, slot);
    return button;
}
