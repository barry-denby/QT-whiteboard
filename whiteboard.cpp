// whiteboard.cpp
//
// implements the class in whiteboard.hpp

// includes
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QKeySequence>
#include <QPainter>
#include <QObject>
#include <QRect>
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

    // allocate space for the current image. default it to 1920 x 1080 until we figure out how
    // to get the screen dimensions dynamically. and fill the image with white to make sure we
    // have our whiteboard
    current_screen = QImage(1920, 1080, QImage::Format_RGB32);
    current_screen.fill(QColor(255, 255, 255));
}

// destructor for the class
Whiteboard::~Whiteboard() {
    // clear up the memory for the current screen
    //delete current_screen;
}

// overridden paint event class that will paint the screen
void Whiteboard::paintEvent(QPaintEvent* event) {
    // begin the painter object
    QPainter painter(this);

    // define a rectangle to encompass the entire view and then draw the image onto the view
    painter.drawImage(0, 0, current_screen, 0, 0, 1920, 1080);

}

// function that will call on QT to quit the application
void Whiteboard::quitApplication() {
    // reset the cursor to the default before going any further
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));

    // call the quit function
    QApplication::quit();
}
