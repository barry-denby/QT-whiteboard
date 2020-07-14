// whiteboard.cpp
//
// implements the class in whiteboard.hpp

// includes
#include <cstdlib>
#include <iostream>
#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QKeySequence>
#include <QPainter>
#include <QObject>
#include <QRect>
#include <QtSvg>
#include <QShortcut>
#include "constants.hpp"
#include "mainwindow.hpp"
#include "whiteboard.hpp"

// constructor for the class
Whiteboard::Whiteboard(QWidget* parent)
: QWidget(parent), current_colour(0, 0, 0), pen(QColor(0, 0, 0)), tool(OP_POINT_SQUARE), current_line_thickness(2), current_point_size(6), image_current(0), image_max(16), image_total(1), on_preview(false), font(QString("Arial"), 20), text_size(20), text_rotation(0), text(QString("Placeholder text to draw")), image_import_filename("")
{
    // add in a shortcut that will allow us to quit the application
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(quitApplication()));

    // add a shortcut that will allow us to undo operations
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, SLOT(undoLastDrawOp()));

    // add a shortcut to trigger an advance image and go back an image
    new QShortcut(QKeySequence(Qt::Key_Right), this, SLOT(advanceImageShortcut()));
    new QShortcut(QKeySequence(Qt::Key_Left), this, SLOT(goBackImageShortcut()));

    // add in shortcuts to trigger an increase or decrease in the size of the draw op
    new QShortcut(QKeySequence(Qt::Key_Up), this, SLOT(increaseSizeShortcut()));
    new QShortcut(QKeySequence(Qt::Key_Down), this, SLOT(decreaseSizeShortcut()));

    // add in a short cut to request for the whiteboard to be saved
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(requestSaveShortcut()));

    // set the shortcuts for advancing/goback on the colours and tools
    new QShortcut(QKeySequence(Qt::Key_E), this, SLOT(advanceColourShortcut()));
    new QShortcut(QKeySequence(Qt::Key_W), this, SLOT(goBackColourShortcut()));
    new QShortcut(QKeySequence(Qt::Key_D), this, SLOT(advanceToolShortcut()));
    new QShortcut(QKeySequence(Qt::Key_S), this, SLOT(goBackToolShortcut()));

    // set shortcuts for requesting keyboard focus for the title and the text
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_T), this, SLOT(requestTitleFocusShortcut()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G), this, SLOT(requestTextFocusShortcut()));

    // set the shortcuts for requesting rotation to the right and rotation to the left
    new QShortcut(QKeySequence(Qt::Key_C), this, SLOT(requestRotateRightShortcut()));
    new QShortcut(QKeySequence(Qt::Key_X), this, SLOT(requestRotateLeftShortcut()));

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

    // at the bottom left of the image draw some text denoting the position of this image in the set
    QFont tempfont(QString("Arial"), 20);
    painter.save();
    painter.setFont(tempfont);
    painter.translate(32, 1052);
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(0, 0, QString("(%1 / %2)").arg(image_current + 1).arg(image_total));
    painter.restore();

    // at the bottom middle of the image draw some text denoting the title of the image
    painter.save();
    painter.setFont(tempfont);
    painter.translate(256, 1052);
    painter.setPen(QColor(0, 0, 0));
    painter.drawText(0, 0, images[image_current]->title);
    painter.restore();

    // end the current painting
    painter.end();

    // reset the current image and return the image when finished
    image_current = temp;
    return image;
}

// function that will tell us if the current image is locked
const bool Whiteboard::imageLocked() {
    // return the locked state of the current image
    return images[image_current]->locked;
}

// function that will return the title of the current image
const QString &Whiteboard::imageTitleCurrent() {
    return images[image_current]->title;
}

// function that will set the lock on the current image
void Whiteboard::lockImage() {
    images[image_current]->lockImage();
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

// function that will set the filename of the image to be imported on the next draw operation
void Whiteboard::setImportImageFilename(const QString &filename) {
    image_import_filename = filename;
    if(QString::compare(filename, QString("")) == 0)
        return;

    // get the width and height of the image for the preview purposes
    if(tool == OP_DRAW_RASTER) {
        // generate a QImage out of this file and get the width and height
        QImage preview(image_import_filename);
        preview_image_width = preview.width();
        preview_image_height = preview.height();
    } else {
        QSvgRenderer preview(image_import_filename);
        QSize preview_size = preview.defaultSize();
        preview_image_width = preview_size.width();
        preview_image_height = preview_size.height();
    }
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

// function that unlocks the current image
void Whiteboard::unlockImage() {
    images[image_current]->unlockImage();
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

    // if the tool is anything other than the draw images then set the image_preview sizes to zero
    if(tool != OP_DRAW_RASTER && tool != OP_DRAW_SVG) {
        preview_image_width = 0;
        preview_image_height = 0;
    }
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
        images[image_current]->addDrawFreehandStart(event->x(), event->y(), current_colour.rgba(), current_line_thickness);
    } else if(tool == OP_LINE_STRAIGHT) {
        // we have the starting point of a straight line so store this in the draw operations
        images[image_current]->addDrawStraightLineStart(event->x(), event->y(), current_colour.rgba(), current_line_thickness);
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
        images[image_current]->addDrawFreehandMid(event->x(), event->y(), current_colour.rgba(), current_line_thickness);
    } else if(tool == OP_LINE_STRAIGHT && event->modifiers() == Qt::ShiftModifier) {
        // we want to snap a straight line to one of the cardinal directions
        snapStraightLine();
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
        images[image_current]->addDrawPointSquare(event->x(), event->y(), current_colour.rgba(), current_point_size);
    } else if(tool == OP_POINT_CIRCLE) {
        // we have a circle point then so store it and repaint it
        images[image_current]->addDrawPointCircle(event->x(), event->y(), current_colour.rgba(), current_point_size);
    } else if(tool == OP_POINT_X) {
        // we have a circle point then so store it and repaint it
        images[image_current]->addDrawPointX(event->x(), event->y(), current_colour.rgba(), current_point_size);
    } else if(tool == OP_LINE_FREEFORM) {
        // we have the end point of a line so store this in the draw operations and repaint
        images[image_current]->addDrawFreehandEnd(event->x(), event->y(), current_colour.rgba(), current_line_thickness);
    }  else if(tool == OP_LINE_STRAIGHT) {
        // if the shift modifier is present then snap the final line
        if(event->modifiers() == Qt::ShiftModifier) {
            snapStraightLine();

            // we have the end point of a straight line so store this in the draw operations
            images[image_current]->addDrawStraightLineEnd(preview_end_x, preview_end_y, current_colour.rgba(), current_line_thickness);
        } else {
            // we have the end point of a straight line so store this in the draw operations
            images[image_current]->addDrawStraightLineEnd(event->x(), event->y(), current_colour.rgba(), current_line_thickness);
        }
    } else if(tool == OP_DRAW_TEXT) {
        // if there is no text entered then do nothing
        if(QString::compare(text, QString("")) == 0)
            return;

        // we have the final point of a text string so add in its draw data
        images[image_current]->addDrawText(text, event->x(), event->y(), current_colour.rgba(), text_size, text_rotation);
    } else if(tool == OP_DRAW_RASTER) {
        // if there is no image set then do nothing
        if(QString::compare(image_import_filename, QString("")) == 0)
            return;

        // we have the final draw positions of an image so add in the draw data
        unsigned int preview_width = preview_end_x - preview_start_x;
        unsigned int preview_height = preview_width * ((float) preview_image_height / preview_image_width);
        images[image_current]->addDrawRasterImage(image_import_filename, preview_start_x, preview_start_y, preview_width, preview_height);
    } else if(tool == OP_DRAW_SVG) {
        // if there is no image set then do nothing
        if(QString::compare(image_import_filename, QString("")) == 0)
            return;

        // we have the final draw positions of an image so add in the draw data
        unsigned int preview_width = preview_end_x - preview_start_x;
        unsigned int preview_height = preview_width * ((float) preview_image_height / preview_image_width);
        images[image_current]->addDrawSVGImage(image_import_filename, preview_start_x, preview_start_y, preview_width, preview_height);
    }

    // repaint the view and state the board has been modified
    repaint();
    emit modified();
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

// slot that will emit a signal to advance the colour
void Whiteboard::advanceColourShortcut() {
    emit advanceColour();
}

// slot that will emit a signal to advance the image on the whiteboard
void Whiteboard::advanceImageShortcut() {
    // emit the advance image signal
    emit advanceImage();
}

// slot that will emit a signal to advance the tool
void Whiteboard::advanceToolShortcut() {
    emit advanceTool();
}

// slot that will emit a signal to decreate the size of thickness of the current draw op
void Whiteboard::decreaseSizeShortcut() {
    emit decreaseSize();
}

// slot that will emit a signal to go back a colour
void Whiteboard::goBackColourShortcut() {
    emit goBackColour();
}

// slot that will emit a signal to go back an image on the whiteboard
void Whiteboard::goBackImageShortcut() {
    // emmit the go back signal
    emit goBackImage();
}

// slot that will emit a signal to go back a tool
void Whiteboard::goBackToolShortcut() {
    emit goBackTool();
}

// slot that will emit a signal to increase the size or thickness of the current draw op
void Whiteboard::increaseSizeShortcut() {
    emit increaseSize();
}

// slot that will request the application increases rotation to the right
void Whiteboard::requestRotateRightShortcut() {
    emit requestRotateRight();
}

// slot that will request the application decreases rotation to the left
void Whiteboard::requestRotateLeftShortcut() {
    emit requestRotateLeft();
}

// slot that will request the application to save the whiteboard
void Whiteboard::requestSaveShortcut() {
    emit requestSave();
}

// function that will call on QT to quit the application
void Whiteboard::quitApplication() {
    // reset the cursor to the default before going any further
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    this->setVisible(false);

    // call the quit function
    QApplication::quit();
}

// slot that will request the application to put keyboard focus on the text to be inserted
void Whiteboard::requestTextFocusShortcut() {
    // emit the relevant signal
    emit requestTextFocus();
}

// slot that will request the application to put keyboard focus on the title of the current image
void Whiteboard::requestTitleFocusShortcut() {
    // emit the relevant signal
    emit requestTitleFocus();
}

// function that will undo the last draw operation
void Whiteboard::undoLastDrawOp() {
    // if the next op is already zero then we cant remove anything
    if(images[image_current]->total_ops == 0)
        return;

    // remove the last operation and redraw the screen
    images[image_current]->removeLastDrawData();
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
    for(unsigned int i = 0; i < images[image_current]->total_ops; i++) {
        // go through each of the draw ops and perform the necessary action
        if(images[image_current]->operations[i].draw_operation == POINT_CIRCLE) {
            drawPointCircle(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == POINT_SQUARE) {
            drawPointSquare(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == POINT_X) {
            drawPointX(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == STRAIGHT_LINE_END) {
            drawStraightLine(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == LINE_POINT) {
            // get a reference to the current line point
            LinePoint *end = (LinePoint *) &images[image_current]->operations[i];

            // check if the previous point is a LINE_START or a LINE_POINT
            if(images[image_current]->operations[i - 1].draw_operation == LINE_POINT) {
                // get reference to the previous line point
                LinePoint *start = (LinePoint *) &images[image_current]->operations[i - 1];

                // set teh pen with the right thickness and the brush
                QPen pen(QColor(end->colour));
                pen.setWidth(end->size);
                painter.setPen(pen);
                painter.setBrush(QColor(end->colour));

                // draw the line
                painter.drawLine(start->x, start->y, end->x, end->y);

            } else if(images[image_current]->operations[i - 1].draw_operation == LINE_START) {
                // get reference to the previous line point
                LineStart *start = (LineStart *) &images[image_current]->operations[i - 1];

                // set teh pen with the right thickness and the brush
                QPen pen(QColor(end->colour));
                pen.setWidth(end->size);
                painter.setPen(pen);
                painter.setBrush(QColor(end->colour));

                // draw the line
                painter.drawLine(start->x, start->y, end->x, end->y);
            }
        } else if(images[image_current]->operations[i].draw_operation == DRAW_TEXT) {
            drawText(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == DRAW_RASTER) {
            drawRasterImage(painter, i);
        } else if(images[image_current]->operations[i].draw_operation == DRAW_SVG) {
            drawSVGImage(painter, i);
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
        } else if(tool == OP_DRAW_RASTER && preview_image_width != 0) {
            // draw a preview rectangle with with the same aspect ration as the image. lock the preview to
            // the width
            painter.setPen(QColor(0, 255, 255));
            painter.setBrush(QColor(0, 255, 255));
            int preview_width = preview_end_x - preview_start_x;
            int preview_height = preview_width * ((float) preview_image_height / preview_image_width);
            painter.drawRect(preview_start_x, preview_start_y, preview_width, preview_height);
        } else if(tool == OP_DRAW_SVG && preview_image_width != 0) {
            painter.setPen(QColor(0, 255, 255));
            painter.setBrush(QColor(0, 255, 255));
            int preview_width = preview_end_x - preview_start_x;
            int preview_height = preview_width * ((float) preview_image_height / preview_image_width);
            painter.drawRect(preview_start_x, preview_start_y, preview_width, preview_height);
        }
    }
}

// refactored private function that will draw a point circle, and an index into the current image that contains the data
void Whiteboard::drawPointCircle(QPainter &painter, unsigned int index) {
    // get the circle point structure set the pen and draw the point
    PointCircle *temp = (PointCircle *) &images[image_current]->operations[index];
    painter.setPen(QColor(temp->colour));
    painter.setBrush(QColor(temp->colour));
    painter.drawEllipse(temp->x - (temp->size / 2), temp->y - (temp->size / 2), temp->size, temp->size);
}

// refactored private function that will draw a point square, and an index into the current image that contains the data
void Whiteboard::drawPointSquare(QPainter &painter, unsigned int index) {
    // get the square point structure set teh pen and draw the point
    PointSquare *temp = (PointSquare *) &images[image_current]->operations[index];
    painter.setPen(QColor(temp->colour));
    painter.setBrush(QColor(temp->colour));
    painter.drawRect(temp->x - (temp->size / 2), temp->y - (temp->size / 2), temp->size, temp->size);
}

// refactored private function that will draw a point x
void Whiteboard::drawPointX(QPainter &painter, unsigned int index) {
    // get the x point structure set teh pen and draw the point
    PointX *temp = (PointX *) &images[image_current]->operations[index];
    QPen pen(QColor(temp->colour));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(QColor(temp->colour));

    // draw the two lines to make the x point
    painter.drawLine(temp->x - (temp->size / 2), temp->y - (temp->size / 2), temp->x + (temp->size / 2), temp->y + (temp->size / 2));
    painter.drawLine(temp->x + (temp->size / 2), temp->y - (temp->size / 2), temp->x - (temp->size / 2), temp->y + (temp->size / 2));
}

// private function that will draw a raster image
void Whiteboard::drawRasterImage(QPainter &painter, unsigned int index) {
    // get a reference to the image for drawing
    RasterImage *temp = (RasterImage *) &images[image_current]->operations[index];

    // draw the image on the board by first specifiing the rects that match the source size and the request destination size
    QRectF source(0, 0, temp->image->width(), temp->image->height());
    QRectF destination(temp->x, temp->y, temp->width, temp->height);
    painter.drawImage(destination, *temp->image, source);
}

// private function that will draw a straight line
void Whiteboard::drawStraightLine(QPainter &painter, unsigned int index) {
    // get references to the start and end of the straight line
    StraightLineStart *start = (StraightLineStart *) &images[image_current]->operations[index - 1];
    StraightLineEnd *end = (StraightLineEnd *) &images[image_current]->operations[index];

    // set teh pen with the right thickness and the brush
    QPen pen(QColor(end->colour));
    pen.setWidth(end->size);
    painter.setPen(pen);
    painter.setBrush(QColor(end->colour));

    // draw the line
    painter.drawLine(start->x, start->y, end->x, end->y);
}

// private function that will draw an SVG image
void Whiteboard::drawSVGImage(QPainter &painter, unsigned int index) {
    // get a reference to the image for drawing
    SVGImage *temp = (SVGImage *) &images[image_current]->operations[index];

    // draw the image on the board by first specifiing the rects that match the source size and the request destination size
    QRectF destination(temp->x, temp->y, temp->width, temp->height);
    temp->image->render(&painter, destination);
}

// refactored private function that will draw text
void Whiteboard::drawText(QPainter &painter, unsigned int index) {
    // get a reference to the text for drawing
    Text *temp = (Text *) &images[image_current]->operations[index];

    // get the font metrics and determine the width of the string
    QFontMetrics metrics = painter.fontMetrics();
    int width = metrics.horizontalAdvance(*temp->string);
    int height = metrics.height();

    // we will need to save, translate to the position, and rotate by the given angle
    painter.save();
    painter.translate(temp->x, temp->y);
    painter.rotate(temp->rotation);

    // draw the text on the board
    QFont tempfont("Arial", temp->size);
    painter.setPen(QColor(temp->colour));
    painter.setFont(tempfont);
    painter.drawText(-width, height / 2, *temp->string);

    // restore our painter state
    painter.restore();
}

// private function that will snap the straight line to one of the 8 caridnal directions
void Whiteboard::snapStraightLine() {
    // we need the differences between the start and end points
    float x_diff = (float)(preview_end_x) - (float)(preview_start_x);
    float y_diff = (float)(preview_end_y) - (float)(preview_start_y);
    float ratio = y_diff / x_diff;

    // if the x and y values fall within certian ranges then adjust the preview to snap to that direction
    if(x_diff > 0 && ratio >= -0.414214 && ratio <= 0.414214) {
        // we have a tangent less than 22.5 degrees so snap to the right
        preview_end_y = preview_start_y;
    } else if(x_diff > 0 && ratio > 0.414214 && ratio <= 2.414214) {
        // we have a tangent betwee 22.5 degrees and 67.5 degrees so snap to right and down
        preview_end_y = preview_start_y + (preview_end_x - preview_start_x);
    } else if(x_diff > 0 && ratio < -0.414214 && ratio >= -2.414214) {
        // we have a tangent betwee 22.5 degrees and 67.5 degrees so snap to right and up
        preview_end_y = preview_start_y - (preview_end_x - preview_start_x);
    } else if(x_diff < 0 && ratio >= -0.414214 && ratio <= 0.414214) {
        // we have a tangent less than 22.5 degrees so snap to the left
        preview_end_y = preview_start_y;
    } else if(x_diff < 0 && ratio >= 0.414214 && ratio <= 2.414214) {
        // we have a tanget greater than 22.5 and less than 67.5 so snap up and left
        preview_end_y = preview_start_y + (preview_end_x - preview_start_x);
    } else if(x_diff < 0 && ratio <= -0.414214 && ratio >= -2.414214) {
        // we have a tangent greater than 22.5 and less than 67.5 so snap down and left
        preview_end_y = preview_start_y - (preview_end_x - preview_start_x);
    } else if(y_diff > 0 && (ratio > 2.414214 || ratio < -2.414214)) {
        // we have a tangent greater than 67.5 either positive or negative degrees so snap down
        preview_end_x = preview_start_x;
    } else if(y_diff < 0 && (ratio > 2.414214 || ratio < -2.414214)) {
        // we have a tangent great then 67.5 either positive or negative so snap up
        preview_end_x = preview_start_x;
    }

}
