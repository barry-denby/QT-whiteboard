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
#include "mainwindow.hpp"
#include "whiteboard.hpp"

// constructor for the class
Whiteboard::Whiteboard(QWidget* parent)
: QWidget(parent), current_colour(0, 0, 0), pen(QColor(0, 0, 0)), tool(OP_POINT_SQUARE), current_line_thickness(2), current_point_size(6), image_current(0), image_max(16), image_total(1), on_preview(false), font(QString("Arial"), 20), text_size(20), text_rotation(0), text(QString("Placeholder text to draw"))
{
    // add in a shortcut that will allow us to quit the application
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(quitApplication()));

    // add a shortcut that will allow us to undo operations
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoLastDrawOp()));

    // add a shortcut to trigger an advance image and go back an image
    new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(advanceImageShortcut()));
    new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(goBackImageShortcut()));

    // set the cursor on the application
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));

    // set a strong focus policy so we can get keyboard events
    setFocusPolicy(Qt::StrongFocus);

    // allocate space for 16 images
    images = (DrawOperations **) new DrawOperations *[16];
    for(unsigned int i = 0; i < 16; i++)
        images[i] = new DrawOperations();
}

// destructor for the class
Whiteboard::~Whiteboard() {
    // delete the memory for the draw operations. this core dumps so will need to fix this
    for(unsigned int i = 0; i < image_max; i++)
        delete images[i];
    delete images;
}

// function that will add a new image in the current place
void Whiteboard::addNewImage() {
    // if we have used up our full set of images so we will need to allocate memory for more by doubling it
    if(image_total == image_max) {
        // we will need to double up the memory before going any further. start off by allocating a new
        // array of draw operations that is twice as large
        unsigned int larger_size = image_max * 2;
        DrawOperations **larger_array = (DrawOperations **) new DrawOperations*[larger_size];

        // move all of the previous draw operations into the new array
        for(unsigned int i = 0; i < image_max; i++)
            larger_array[i] = images[i];

        // allocate new draw operations in the remaining space
        for(unsigned int i = image_max; i < larger_size; i++)
            larger_array[i] = new DrawOperations();

        // replace the old array with the new one
        delete images;
        images = larger_array;
        image_max = larger_size;

    }

    // see what situation we have now
    if (image_total == image_current + 1) {
        // we are adding a new image to the end of the list so just increment the current by 1
        image_current++;
        image_total++;
    } else {
        // we are adding a new image in between another image so we will need to take an unused one off the end
        // and insert it before after the current image we will start by getting a reference to the end image
        DrawOperations *temp = images[image_max - 1];

        // shift everything up one place from the current image
        for(unsigned int i = image_max - 2; i > image_current; i--)
            images[i + 1] = images[i];

        // put the temp image in the current place and increment the current counter
        image_current++;
        images[image_current] = temp;
        image_total++;
    }

    // force a repaint after a new image has been added
    repaint();
}

// function that will return the full list of draw images
DrawOperations **Whiteboard::drawOperations() {
    return images;
}

// function that will delete the currently selected image
void Whiteboard::deleteImage() {
    // see if we are at the last image or an earlier one
    if(image_current == image_total - 1) {
        // we are at the last image so reset it
        images[image_current]->reset();
    } else {
        // we are earlier in the list meaning we will need to reset the image and move all images after back one
        DrawOperations *temp = images[image_current];
        temp->reset();

        // shift all images back one and then put temp at the end
        for(unsigned int i = image_current; i < image_total - 1; i++)
            images[i] = images[i + 1];
        images[image_total - 1] = temp;
    }

    // reduce the number of images
    image_total--;
}

// function that will run the draw commands on a QImage and will return it
// this is for exporting purposes
QImage *Whiteboard::exportBoard(const unsigned int board) {
    // the QImage that we will return
    QImage *image = new QImage(1920, 1080, QImage::Format_RGB32);

    // take a copy of the current image value and replace it with board
    unsigned int temp = image_current;
    image_current = board;

    // begin the painter object and set the current paint colour
    QPainter painter;
    painter.begin(image);

    // draw the board
    drawBoard(painter);

    // end the current painting
    painter.end();

    // reset the current image and return the image when finished
    image_current = temp;
    return image;
}

// function that will return the title of the current image
const QString &Whiteboard::imageTitleCurrent() {
    return images[image_current]->title;
}

// function that will return the maximum number of images in this whiteboard
const unsigned int Whiteboard::maxImages() {
    return image_max;
}

// function that will replace the current set of images with this set of images
void Whiteboard::setDrawOperations(DrawOperations **operations, const unsigned int total, const unsigned int max) {
    // delete the current array of objects and put a new one in its place
    for(unsigned int i = 0; i < image_max; i++)
        delete images[i];
    delete images;

    // take a copy of the images update the total and set the current image to one
    images = operations;
    image_current = 0;
    image_total = total;
    image_max = max;

    // force a repaint when the images have been replaced
    repaint();
}

// function that will reset the state of the whiteboard to its original state
void Whiteboard::resetWhiteBoard() {
    // delete the current array of objects and put a new one in its place
    for(unsigned int i = 0; i < image_max; i++)
        delete images[i];
    delete images;

    images = (DrawOperations **) new DrawOperations *[16];
    for(unsigned int i = 0; i < 16; i++)
        images[i] = new DrawOperations();
    image_current = 0;
    image_max = 16;
    image_total = 1;
}

// function that states how many images in total this whiteboard has thus far
const unsigned int Whiteboard::totalImages() {
    return image_total;
}

// public slot that will change the current draw colour
void Whiteboard::changeColour(int red, int green, int blue) {
    current_colour.setRed(red);
    current_colour.setGreen(green);
    current_colour.setBlue(blue);
}

// public slot that will change the current image. note that we decrement the value provided here
// by one to account for indices starting at zero
void Whiteboard::changeImage(int number) {
    // change the image index and force a repaint
    image_current = (unsigned int)(number) - 1;
    repaint();
}

// slot that will change the title of the current image
void Whiteboard::changeImageTitle(const QString& title) {
    // as we dont store the title here we will need to pass it through to the draw operations
    images[image_current]->setTitle(title);
}

// slot that will change the line thickness
void Whiteboard::changeLineThickness(int line_thickness) {
    current_line_thickness = line_thickness;
}

// slot that will change the point size
void Whiteboard::changePointSize(int point_size) {
    current_point_size = point_size;
}

// slot that will change the text to be displayed
void Whiteboard::changeText(const QString &text) {
    this->text = text;
}

// slot that will change the rotation of the text
void Whiteboard::changeTextRotation(int text_rotation) {
    this->text_rotation = text_rotation;
}

// slot that will change the size of the text
void Whiteboard::changeTextSize(int text_size) {
    // take a copy of the text_size and upate the font
    this->text_size = text_size;
    font.setPointSize(text_size);
}

// public slot taht will change the current draw tool
void Whiteboard::changeTool(unsigned int tool) {
    this->tool = tool;
}

// overridden mousePressEvent function that will start a user's drawing
void Whiteboard::mousePressEvent(QMouseEvent* event) {
    // start preview mode and take the current pixel values
    on_preview = true;
    preview_start_x = event->x();
    preview_start_y = event->y();
    preview_end_x = event->x();
    preview_end_y = event->y();

    // see what operation we are doing
    if (tool == OP_LINE_FREEFORM) {
        // we have the starting point of a line so store this in the draw operations
        images[image_current]->addDrawData(LINE_START, event->x(), event->y(), current_colour, current_line_thickness);
    } else if(tool == OP_LINE_STRAIGHT) {
        // we have the starting point of a straight line so store this in the draw operations
        images[image_current]->addDrawData(STRAIGHT_LINE_START, event->x(), event->y(), current_colour, current_line_thickness);
    }

    // repaint the view
    repaint();
}

// overridden mouseMoveEvent function that will continue a user's drawing
void Whiteboard::mouseMoveEvent(QMouseEvent* event) {
    // take a copy of the x and y values
    preview_end_x = event->x();
    preview_end_y = event->y();

    // see what operation we are doing
    if (tool == OP_LINE_FREEFORM) {
        // we have the continiouing point of a line so store this in the draw operations and repaint
        images[image_current]->addDrawData(LINE_POINT, event->x(), event->y(), current_colour, current_line_thickness);
    }

    // repaint the view
    repaint();
}

// overridden mouse release event that will finish drawing events
void Whiteboard::mouseReleaseEvent(QMouseEvent* event) {
    // end preview mode
    on_preview = false;

    // do a different action depending on the event type
    if(tool == OP_POINT_SQUARE) {
        // we have a square point then just save it and update the draw ops
        images[image_current]->addDrawData(POINT_SQUARE, event->x(), event->y(), current_colour, current_point_size);
    } else if(tool == OP_POINT_CIRCLE) {
        // we have a circle point then so store it and repaint it
        images[image_current]->addDrawData(POINT_CIRCLE, event->x(), event->y(), current_colour, current_point_size);
    } else if(tool == OP_POINT_X) {
        // we have a circle point then so store it and repaint it
        images[image_current]->addDrawData(POINT_X, event->x(), event->y(), current_colour, current_point_size);
    } else if(tool == OP_LINE_FREEFORM) {
        // we have the end point of a line so store this in the draw operations and repaint
        images[image_current]->addDrawData(LINE_END, event->x(), event->y(), current_colour, current_line_thickness);
    }  else if(tool == OP_LINE_STRAIGHT) {
        // we have the end point of a straight line so store this in the draw operations
        images[image_current]->addDrawData(STRAIGHT_LINE_END, event->x(), event->y(), current_colour, current_line_thickness);
    } else if(tool == OP_DRAW_TEXT) {
        // if there is no text entered then do nothing
        if(QString::compare(text, QString("")) == 0)
            return;

        // we have the final point of a text string so add in its draw data
        images[image_current]->addDrawText(text, event->x(), event->y(), current_colour, text_size, text_rotation);
    }

    // repaint the view
    repaint();

}

// overridden paint event class that will paint the screen
void Whiteboard::paintEvent(QPaintEvent* event) {
    // begin the painter object and set the current paint colour
    QPainter painter;
    painter.begin(this);

    // draw the board
    drawBoard(painter);

    // end the current painting
    painter.end();

}

// slot that will emit a signal to advance the image on the whiteboard
void Whiteboard::advanceImageShortcut() {
    // emit the advance image signal
    emit advanceImage();
}

// slot that will emit a signal to go back an image on the whiteboard
void Whiteboard::goBackImageShortcut() {
    // emmit the go back signal
    emit goBackImage();
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
    if(images[image_current]->total_ops == 0)
        return;

    // see what kind of draw op we have and take appropriate action
    if(images[image_current]->draw_operation[images[image_current]->total_ops - 1] == POINT_SQUARE) {
        // remove the point operation
        images[image_current]->removeLastDrawData();
    } else if(images[image_current]->draw_operation[images[image_current]->total_ops - 1] == POINT_CIRCLE) {
        // remove the point operation
        images[image_current]->removeLastDrawData();
    } else if(images[image_current]->draw_operation[images[image_current]->total_ops - 1] == POINT_X) {
        // remove the point operation
        images[image_current]->removeLastDrawData();
    } else if(images[image_current]->draw_operation[images[image_current]->total_ops - 1] == LINE_END) {
        // keep going back until we hit the line start, then remove the line start
        while(images[image_current]->draw_operation[images[image_current]->total_ops - 1] != LINE_START)
            images[image_current]->removeLastDrawData();
        images[image_current]->removeLastDrawData();
    } else if(images[image_current]->draw_operation[images[image_current]->total_ops - 1] == DRAW_TEXT) {
        // we have a text operation so just remove it
        images[image_current]->removeLastDrawData();
    }

    // redraw the screen
    repaint();
}

// refactored private function that will draw the board with the provided painter object. function
// will assume that painter has been started before calling and will be ended after calling
void Whiteboard::drawBoard(QPainter &painter) {
    // draw a white background on the view
    painter.setPen(QColor(255, 255, 255));
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRect(0, 0, 1920, 1080);

    // go through all of the draw operations that are in the list
    QColor draw_colour;
    for(unsigned int i = 0; i < images[image_current]->total_ops; i++) {
        // set the colour of the current position
        draw_colour.setRgb(images[image_current]->draw_red[i], images[image_current]->draw_green[i], images[image_current]->draw_blue[i]);
        painter.setPen(draw_colour);
        painter.setBrush(draw_colour);

        // go through each of the draw ops and draw the necessary action
        if(images[image_current]->draw_operation[i] == POINT_SQUARE) {
            // we have a single point so draw that
            int point_size = images[image_current]->draw_sizes[i];
            painter.drawRect(images[image_current]->draw_x[i] - (point_size / 2), images[image_current]->draw_y[i] - (point_size / 2), point_size, point_size);
        } else if(images[image_current]->draw_operation[i] == POINT_CIRCLE) {
            // draw the circle point
            int point_size = images[image_current]->draw_sizes[i];
            painter.drawEllipse(images[image_current]->draw_x[i] - (point_size / 2), images[image_current]->draw_y[i] - (point_size / 2), point_size, point_size);
        } else if(images[image_current]->draw_operation[i] == POINT_X) {
            // set the pen size to 2
            QPen pen(draw_colour);
            pen.setWidth(2);
            painter.setPen(pen);

            // draw the x point
            int point_size = images[image_current]->draw_sizes[i];
            painter.drawLine(images[image_current]->draw_x[i] - (point_size / 2), images[image_current]->draw_y[i] - (point_size / 2), images[image_current]->draw_x[i] + (point_size / 2), images[image_current]->draw_y[i] + (point_size / 2));
            painter.drawLine(images[image_current]->draw_x[i] - (point_size / 2), images[image_current]->draw_y[i] + (point_size / 2), images[image_current]->draw_x[i] + (point_size / 2), images[image_current]->draw_y[i] - (point_size / 2));

        } else if(images[image_current]->draw_operation[i] == LINE_POINT || images[image_current]->draw_operation[i] == LINE_END) {
            QPen pen(draw_colour);
            pen.setWidth(images[image_current]->draw_sizes[i]);
            painter.setPen(pen);

            // draw this line segment
            painter.drawLine(images[image_current]->draw_x[i - 1], images[image_current]->draw_y[i - 1], images[image_current]->draw_x[i], images[image_current]->draw_y[i]);
        } else if(images[image_current]->draw_operation[i] == STRAIGHT_LINE_END) {
            QPen pen(draw_colour);
            pen.setWidth(images[image_current]->draw_sizes[i]);
            painter.setPen(pen);

            // draw this straight line
            painter.drawLine(images[image_current]->draw_x[i - 1], images[image_current]->draw_y[i - 1], images[image_current]->draw_x[i], images[image_current]->draw_y[i]);
        } else if(images[image_current]->draw_operation[i] == DRAW_TEXT) {
            // get the font metrics and determine the width of the string
            unsigned int string_index = images[image_current]->draw_string_index[i];
            QFontMetrics metrics = painter.fontMetrics();
            int width = metrics.horizontalAdvance(images[image_current]->draw_text_strings[string_index]);
            int height = metrics.height();

            // we will need to save, translate to the position, and rotate by the given angle
            painter.save();
            painter.translate(images[image_current]->draw_x[i], images[image_current]->draw_y[i]);
            painter.rotate(images[image_current]->draw_text_rotations[i]);

            // draw the preview text on the board
            QFont tempfont("Arial", images[image_current]->draw_sizes[i]);
            painter.setFont(tempfont);
            painter.drawText(-width, height / 2, images[image_current]->draw_text_strings[string_index]);

            // restore our painter state
            painter.restore();
        }
    }

    // draw the preview in a cyan colour for all operations bar the free form line
    painter.setPen(QColor(0, 255, 255));
    painter.setBrush(QColor(0, 255, 255));
    if(on_preview) {
        if(tool == OP_POINT_SQUARE) {
            // draw a preview square at the last known location
            painter.drawRect(preview_end_x - current_point_size / 2, preview_end_y - current_point_size / 2, current_point_size, current_point_size);
        } else if(tool == OP_POINT_CIRCLE) {
            // draw a preview circle at the last known location
            painter.drawEllipse(preview_end_x - current_point_size / 2, preview_end_y - current_point_size / 2, current_point_size, current_point_size);
        } else if(tool == OP_POINT_X) {
            // set the pen size to 2
            QPen pen(QColor(0, 255, 255));
            pen.setWidth(2);
            painter.setPen(pen);

            // draw the x point
            painter.drawLine(preview_end_x - (current_point_size / 2), preview_end_y - (current_point_size / 2), preview_end_x + (current_point_size / 2), preview_end_y + (current_point_size / 2));
            painter.drawLine(preview_end_x - (current_point_size / 2), preview_end_y + (current_point_size / 2), preview_end_x + (current_point_size / 2), preview_end_y - (current_point_size / 2));
        } else if(tool == OP_LINE_STRAIGHT) {
            // set the pen size to 2
            QPen pen(QColor(0, 255, 255));
            pen.setWidth(current_line_thickness);
            painter.setPen(pen);

            // draw the line point
            painter.drawLine(preview_start_x, preview_start_y, preview_end_x, preview_end_y);
        } else if(tool == OP_DRAW_TEXT) {
            // get the font metrics and determine the width of the string
            QFontMetrics metrics = painter.fontMetrics();
            int width = metrics.horizontalAdvance(text);
            int height = metrics.height();

            // we will need to save, translate to the position, and rotate by the given angle
            painter.save();
            painter.translate(preview_end_x, preview_end_y);
            painter.rotate(text_rotation);

            // draw the preview text on the board
            painter.setPen(QColor(0, 255, 255));
            painter.setBrush(QColor(0, 255, 255));
            painter.setFont(font);
            painter.drawText(-width, height / 2, text);

            // restore our painter state
            painter.restore();
        }
    }
}
