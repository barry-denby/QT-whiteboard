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
    // function that will add the following draw data to the draw ops
    void addDrawData(unsigned int operation, int x, int y, int draw_size);
    // function that will remove the last draw op from the data
    void removeLastDrawData();
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
    // arrays that contain the draw colours for each drawing op
    int *draw_red;
    int *draw_green;
    int *draw_blue;
    // array that contains the size for each draw op
    int *draw_sizes;
    // index to the next free draw op
    unsigned int next_draw_op;
    // indicates if we are doing a point or a line op
    bool line_draw;
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
