#ifndef _WHITEBOARD_HPP
#define _WHITEBOARD_HPP

// whiteboard.hpp
//
// header file for the whiteboard

// includes needed to make this work
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include "drawoperations.hpp"

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
    // function that will reset the whiteboard to its starting conditions
    void resetWhiteBoard();
// public slots of the class
public slots:
    // slot that will change the current draw colour to the indicated colour
    void changeColour(int red, int green, int blue);
    // slot that will change the tool to the indicated tool
    void changeTool(unsigned int tool);
    // slot that will change the point size
    void changePointSize(int point_size);
    // slot that will change the line thickness
    void changeLineThickness(int line_thickness);
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
    // pen for drawing a point, and pen draw drawing lines
    QPen point_pen;
    QPen line_pen;
    // the current tool that is being used
    unsigned int tool;
    // the current line thickness and point sizes
    int current_line_thickness, current_point_size;
    // draw operations array that will hold all of the images for this whiteboard
    DrawOperations *images;
    // the current image we are looking at and the maximum number of images we have
    unsigned int image_current;
    unsigned int image_max;
};

#endif // _WHITEBOARD_HPP
