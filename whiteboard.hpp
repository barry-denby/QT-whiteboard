#ifndef _WHITEBOARD_HPP
#define _WHITEBOARD_HPP

// whiteboard.hpp
//
// header file for the whiteboard

// includes needed to make this work
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

// class defintion
class Whiteboard : public QWidget {
    // needed to get access to the signals and slots mechanism
    Q_OBJECT

// public section of the class
public:
    // constructor for the class
    Whiteboard(QWidget* parent = 0);
    // destructor for the class
    virtual ~Whiteboard();
// protected methods of the class
protected:
    // overriddent mousepressevent method so we can take user drawing on screen
    void mousePressEvent(QMouseEvent* event);
    // overridden mouseMoveEvent function to do user drawing
    void mouseMoveEvent(QMouseEvent* event);
    // overridden mouseReleaseEvent
    void mouseReleaseEvent(QMouseEvent* event);
    // overridden paintevent method so we can draw on the screen
    void paintEvent(QPaintEvent* event);
// private slots section of the class
private slots:
    // slot for quitting the application
    void quitApplication();
    // slot that will undo the last drawing operation
    void undoLastDrawOp();
// private fields of the class
private:
    // the current drawing colour
    QColor current_colour;
    // the image that will contain all of our data and will be rendered to screen
    QImage current_screen;
    // values for the old x and old y
    int old_x, old_y;
    // array that contains ids for each item in the list of the draw history. we need this
    // to recreate the drawing each time so we can do an undo or dump it to file if needs be
    // and the x and y coordinates associated with each op
    unsigned int *draw_operation;
    unsigned int *draw_x;
    unsigned int *draw_y;
    // index to the next free draw op
    unsigned int next_draw_op;
    // indicates if we are doing a point or a line op
    bool line_draw;
    // painter for this widget
    //QPainter painter;
};

#endif // _WHITEBOARD_HPP
