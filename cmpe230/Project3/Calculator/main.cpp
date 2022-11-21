#include <QApplication>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include "display.h"


#define WINDOW_HEIGHT 200
#define WINDOW_WIDTH  360


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget *cw = new QWidget; // main widget
    QVBoxLayout *down_vb = new QVBoxLayout(cw); // for the grid consisting of buttons
    QVBoxLayout *up_vb = new QVBoxLayout(cw); // for the line edit
    QGridLayout *grid = new QGridLayout(); // for buttons
    QPushButton *button;
    Display *display = new Display(WINDOW_WIDTH-23);
    up_vb->addWidget(display->lineEdit);

    // Create buttons for +, -, =, and Clr operations
    button = new QPushButton("+");
    grid->addWidget(button, 0, 0, 1, 1);
    QObject::connect(button, SIGNAL(clicked()), display, SLOT(change_op()));
    button = new QPushButton("-");
    grid->addWidget(button, 0, 1, 1, 1);
    QObject::connect(button, SIGNAL(clicked()), display, SLOT(change_op()));
    button = new QPushButton("=");
    grid->addWidget(button, 0, 2, 1, 1);
    QObject::connect(button, SIGNAL(clicked()), display, SLOT(display_result()));
    button = new QPushButton("Clr");
    grid->addWidget(button, 0, 3, 1, 1);
     QObject::connect(button, SIGNAL(clicked()), display, SLOT(clear()));

    // Create buttons for hexadecimal digits
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int value = 4*row+col;
            QString digit = (value == 10)?"A":
                            (value == 11)?"B":
                            (value == 12)?"C":
                            (value == 13)?"D":
                            (value == 14)?"E":
                            (value == 15)?"F":
                            QString::number(value);
            button = display->createButton(digit, SLOT(add_digit()));
            grid->addWidget(button, row+1, col, 1, 1); // add the button to the grid
        }
    }

    down_vb->addLayout(up_vb); // add upper vertical box containing the line edit to the other
    down_vb->addLayout(grid);
    QSpacerItem *si = new QSpacerItem(0, 30, QSizePolicy::Expanding, QSizePolicy::Expanding); // add space between buttons
    down_vb->addSpacerItem(si);

    cw->setWindowTitle("Calculator");
    cw->setFixedHeight(WINDOW_HEIGHT);
    cw->setFixedWidth(WINDOW_WIDTH);
    cw->show();

    return app.exec();
}

