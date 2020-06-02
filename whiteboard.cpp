// whiteboard.cpp
//
// implements the class in whiteboard.hpp

// includes
#include <QApplication>
#include <QCursor>
#include <QKeySequence>
#include <QObject>
#include <QShortcut>
#include "whiteboard.hpp"

// constructor for the class
Whiteboard::Whiteboard(QWidget* parent)
: QWidget(parent)
{
    // add in a shortcut that will allow us to quit the application
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(quitApplication()));

    // set the cursor on the application
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
}

// destructor for the class
Whiteboard::~Whiteboard() {

}

// function that will call on QT to quit the application
void Whiteboard::quitApplication() {
    // reset the cursor to the default before going any further
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

    // call the quit function
    QApplication::quit();
}
