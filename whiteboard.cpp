// whiteboard.cpp
//
// implements the class in whiteboard.hpp

// includes
#include <QApplication>
#include <QKeySequence>
#include <QObject>
#include <QShortcut>
#include "whiteboard.hpp"

// constructor for the class
Whiteboard::Whiteboard(QWidget* parent)
: QWidget(parent)
{
    // add in a shortcut that will allow us to quit the application
    QShortcut* quit_shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(quitApplication()));
}

// destructor for the class
Whiteboard::~Whiteboard() {

}

// function that will call on QT to quit the application
void Whiteboard::quitApplication() {
    // call the quit function
    QApplication::quit();
}
