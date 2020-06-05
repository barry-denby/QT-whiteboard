// whiteboard.cpp
//
// implements the class in whiteboard.hpp

// includes
#include <iostream>
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QKeySequence>
#include <QPainter>
#include <QObject>
#include <QRect>
#include <QShortcut>
#include "constants.hpp"
#include "whiteboard.hpp"

// constructor for the class
Whiteboard::Whiteboard(QWidget* parent)
: QWidget(parent), current_colour(0, 0, 0), point_pen(QColor(0, 0, 0)), line_pen(QColor(0, 0, 0)), tool(OP_POINT_VARIABLE_SIZE), current_line_thickness(1), current_point_size(3), image_current(0), image_max(16)
{
    // add in a shortcut that will allow us to quit the application
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(quitApplication()));

    // add a shortcut that will allow us to undo operations
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoLastDrawOp()));

    // set the cursor on the application
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));

    // set a strong focus policy so we can get keyboard events
    setFocusPolicy(Qt::StrongFocus);

    // allocate space for 16 images
    images = (DrawOperations *) new DrawOperations[16];
}

// destructor for the class
Whiteboard::~Whiteboard() {
    // delete the memory for the draw operations. this core dumps so will need to fix this
    delete[] images;
}

// function that will reset the state of the whiteboard to its original state
void Whiteboard::resetWhiteBoard() {

}

// public slot that will change the current draw colour
void Whiteboard::changeColour(int red, int green, int blue) {
    current_colour.setRed(red);
    current_colour.setGreen(green);
    current_colour.setBlue(blue);
}

// public slot taht will change the current draw tool
void Whiteboard::changeTool(unsigned int tool) {
    this->tool = tool;
}

// slot that will change the point size
void Whiteboard::changePointSize(int point_size) {
    current_point_size = point_size;
}

// slot that will change the line thickness
void Whiteboard::changeLineThickness(int line_thickness) {
    current_line_thickness = line_thickness;
}

// overridden mousePressEvent function that will start a user's drawing
void Whiteboard::mousePressEvent(QMouseEvent* event) {
    // see what operation we are doing
    if (tool == OP_LINE_VARIABLE_THICKNESS) {
        // we have the starting point of a line so store this in the draw operations
        //addDrawData(LINE_START, event->x(), event->y(), current_line_thickness);
        images[image_current].addDrawData(LINE_START, event->x(), event->y(), current_colour, current_line_thickness);
    }
}

// overridden mouseMoveEvent function that will continue a user's drawing
void Whiteboard::mouseMoveEvent(QMouseEvent* event) {
    // see what operation we are doing
    if (tool == OP_LINE_VARIABLE_THICKNESS) {
        // we have the continiouing point of a line so store this in the draw operations and repaint
        //addDrawData(LINE_POINT, event->x(), event->y(), current_line_thickness);
        images[image_current].addDrawData(LINE_POINT, event->x(), event->y(), current_colour, current_line_thickness);
        repaint();
    }
}

// overridden mouse release event that will finish drawing events
void Whiteboard::mouseReleaseEvent(QMouseEvent* event) {
    // do a different action depending on the event type
    if(tool == OP_POINT_VARIABLE_SIZE) {
        // we have a fixed point then just save it and update the draw ops
        //addDrawData(POINT, event->x(), event->y(), current_point_size);
        images[image_current].addDrawData(POINT, event->x(), event->y(), current_colour, current_point_size);
        repaint();
    } else if(tool == OP_LINE_VARIABLE_THICKNESS) {
        // we have the end point of a line so store this in the draw operations and repaint
        //addDrawData(LINE_END, event->x(), event->y(), current_line_thickness);
        images[image_current].addDrawData(LINE_END, event->x(), event->y(), current_colour, current_line_thickness);
        repaint();
    }

}

// overridden paint event class that will paint the screen
void Whiteboard::paintEvent(QPaintEvent* event) {
    // begin the painter object and set the current paint colour
    QPainter painter;
    painter.begin(this);

    // draw a white background on the view
    painter.setPen(QColor(255, 255, 255));
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRect(0, 0, 1920, 1080);

    painter.setPen(current_colour);
    painter.setBrush(current_colour);

    // go through all of the draw operations that are in the list
    for(unsigned int i = 0; i < images[image_current].total_ops; i++) {
        // set the colour of the current position
        current_colour.setRgb(images[image_current].draw_red[i], images[image_current].draw_green[i], images[image_current].draw_blue[i]);
        point_pen.setColor(current_colour);
        line_pen.setColor(current_colour);

        //std::cout << "i, op, x, y: " << i << ", "  << draw_operation[i] << ", "  << draw_x[i] << ", "  << draw_y[i] << std::endl;
        // go through each of the draw ops and draw the necessary action
        if(images[image_current].draw_operation[i] == POINT) {
            // we have a single point so draw that
            point_pen.setWidth(images[image_current].draw_sizes[i]);
            painter.setPen(point_pen);
            painter.drawPoint(images[image_current].draw_x[i], images[image_current].draw_y[i]);
        } else if(images[image_current].draw_operation[i] == LINE_POINT || images[image_current].draw_operation[i] == LINE_END) {
            // draw this line segment
            line_pen.setWidth(images[image_current].draw_sizes[i]);
            painter.setPen(line_pen);
            painter.drawLine(images[image_current].draw_x[i - 1], images[image_current].draw_y[i - 1], images[image_current].draw_x[i], images[image_current].draw_y[i]);
        }
    }

    // end the current painting
    painter.end();

}

// function that will call on QT to quit the application
void Whiteboard::quitApplication() {
    // reset the cursor to the default before going any further
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    this->setVisible(false);

    // call the quit function
    QApplication::quit();
}

// function that will undo the last draw operation
void Whiteboard::undoLastDrawOp() {
    // if the next op is already zero then we cant remove anything
    if(images[image_current].total_ops == 0)
        return;

    // see what kind of draw op we have and take appropriate action
    if(images[image_current].draw_operation[images[image_current].total_ops - 1] == POINT) {
        // remove the point operation
        images[image_current].removeLastDrawData();
    } else if(images[image_current].draw_operation[images[image_current].total_ops - 1] == LINE_END) {
        // keep going back until we hit the line start, then remove the line start
        while(images[image_current].draw_operation[images[image_current].total_ops - 1] != LINE_START)
            images[image_current].removeLastDrawData();
        images[image_current].removeLastDrawData();
    }

    // redraw the screen
    repaint();
}
