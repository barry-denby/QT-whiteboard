// drawoperations.cpp
//
// implenents everything described in drawoperations.hpp

// includes
#include <iostream>
#include "constants.hpp"
#include "drawoperations.hpp"

// default constructor for the class that will initialise a 4K sized draw operations object
DrawOperations::DrawOperations()
: total_ops(0), max_ops(1024), total_strings(0), max_strings(8), title(QString("")), total_images(0), max_images(8)
{
    // allocate the array of draw operations
    operations = new DrawOp[max_ops];
}

DrawOperations::DrawOperations(const unsigned int max_ops, const unsigned int max_strings, const unsigned int max_images)
: total_ops(0), max_ops(max_ops), total_strings(0), max_strings(max_strings), title(QString("")), total_images(0), max_images(max_images)
{
    // allocate the array of draw operations
    operations = new DrawOp[max_ops];
}

// destructor for the class
DrawOperations::~DrawOperations() {
    // delete all of our arrays
    delete operations;
}

// refactored function that will allocate all of our arrays
void DrawOperations::allocateArrays() {
    // initialise support for max_ops drawing ops
    // draw_operation = (unsigned int*) new unsigned int[max_ops];
    // draw_x = (unsigned int*) new unsigned int[max_ops];
    // draw_y = (unsigned int*) new unsigned int[max_ops];
    // draw_red = (int *) new int[max_ops];
    // draw_green = (int *) new int[max_ops];
    // draw_blue = (int *) new int[max_ops];
    // draw_sizes = (int *) new int[max_ops];
    // draw_string_index = new int[max_ops];
    // draw_text_rotations = new int[max_ops];
    // for(unsigned int i = 0; i < max_ops; i++) {
    //     draw_operation[i] = 0;
    //     draw_x[i] = 0;
    //     draw_y[i] = 0;
    //     draw_red[i] = 0;
    //     draw_green[i] = 0;
    //     draw_blue[i] = 0;
    //     draw_sizes[i] = 0;
    //     draw_string_index[i] = 0;
    //     draw_text_rotations[i] = 0;
    // }
}

// adds draw data for the start point of a freehand line
void DrawOperations::addDrawFreehandStart(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a freehand line point and fill in the data
    LineStart *temp = (LineStart *) &operations[total_ops];
    temp->draw_operation = LINE_START;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for a mid point of the middle of a freehand line
void DrawOperations::addDrawFreehandMid(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a freehand line mid point and fill in the data
    LinePoint *temp = (LinePoint *) &operations[total_ops];
    temp->draw_operation = LINE_POINT;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for the end point of a freehand line
void DrawOperations::addDrawFreehandEnd(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a freehand line point and fill in the data
    LineEnd *temp = (LineEnd *) &operations[total_ops];
    temp->draw_operation = LINE_END;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for a circle point
void DrawOperations::addDrawPointCircle(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a circle point and fill in the data
    PointCircle *temp = (PointCircle *) &operations[total_ops];
    temp->draw_operation = POINT_CIRCLE;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for a square point
void DrawOperations::addDrawPointSquare(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a square point and fill in the data
    PointSquare *temp = (PointSquare *) &operations[total_ops];
    temp->draw_operation = POINT_SQUARE;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for an x point
void DrawOperations::addDrawPointX(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a square point and fill in the data
    PointX *temp = (PointX *) &operations[total_ops];
    temp->draw_operation = POINT_X;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for a straight line end
void DrawOperations::addDrawStraightLineEnd(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a straight line start and fill in the data
    StraightLineEnd *temp = (StraightLineEnd *) &operations[total_ops];
    temp->draw_operation = STRAIGHT_LINE_END;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds draw data for a straight line start
void DrawOperations::addDrawStraightLineStart(int x, int y, unsigned int colour, int draw_size) {
    // set the current draw operation to a straight line start and fill in the data
    StraightLineStart *temp = (StraightLineStart *) &operations[total_ops];
    temp->draw_operation = STRAIGHT_LINE_START;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;

    // update the total ops after we are done
    total_ops++;
}

// adds in drawn text to the draw operations as this needs to be handled differently to the other operations
void DrawOperations::addDrawText(const QString &text, int x, int y, unsigned int colour, int draw_size, int draw_rotation) {
    // set the current draw operation to a text operation and fill in the data
    Text *temp = (Text *) &operations[total_ops];
    temp->draw_operation = DRAW_TEXT;
    temp->x = x;
    temp->y = y;
    temp->colour = colour;
    temp->size = draw_size;
    temp->rotation = draw_rotation;
    temp->string = new QString(text);

    // update the total ops after we are done
    total_ops++;
}

// adds in a drawn raster image to the draw operations as this needs to be handled differently to other operations
void DrawOperations::addDrawRasterImage(const QString &file, int x, int y, int width, int height) {
    // set the current draw operation to a raster image and fill in the data
    RasterImage *temp = (RasterImage *) &operations[total_ops];
    temp->draw_operation = DRAW_RASTER;
    temp->x = x;
    temp->y = y;
    temp->width = width;
    temp->height = height;
    temp->filename = new QString(file);
    temp->image = new QImage(file);

    // update the total ops after we are done
    total_ops++;
}

// adds in a drawn vector image to the draw operations as this needs to be handled differently to other operations
void DrawOperations::addDrawSVGImage(const QString &file, int x, int y, int width, int height) {
    // set the current draw operation to an SVG image and fill in the data
    // set the current draw operation to a raster image and fill in the data
    SVGImage *temp = (SVGImage *) &operations[total_ops];
    temp->draw_operation = DRAW_SVG;
    temp->x = x;
    temp->y = y;
    temp->width = width;
    temp->height = height;
    temp->filename = new QString(file);
    temp->image = new QSvgRenderer(file);

    // update the total ops after we are done
    total_ops++;
}

// function that will deallocate all of the arrays of this draw operations
void DrawOperations::deallocateArrays() {
    // delete all of our arrays
    // delete draw_operation;
    // delete draw_x;
    // delete draw_y;
    // delete draw_red;
    // delete draw_green;
    // delete draw_blue;
    // delete draw_sizes;
    // delete draw_text_rotations;
    // delete draw_string_index;
}

// refactored function that will remove the last set of draw data from the arrays
void DrawOperations::removeLastDrawData() {
    // if the next op is already zero then we cant remove anything
    if(total_ops == 0)
         return;

    // reduce total_ops by one to line up on the data we are removing
    total_ops--;

    // remove the data depending on what we have
    if(operations[total_ops].draw_operation == POINT_CIRCLE)
        removePointCircleData();
    else if(operations[total_ops].draw_operation == POINT_SQUARE)
        removePointSquareData();
    else if(operations[total_ops].draw_operation == POINT_X)
        removePointXData();
    else if(operations[total_ops].draw_operation == LINE_END)
        removeFreehandLine();
    else if(operations[total_ops].draw_operation == STRAIGHT_LINE_END)
        removeStraightLine();
    else if(operations[total_ops].draw_operation == DRAW_TEXT)
        removeText();
    else if(operations[total_ops].draw_operation == DRAW_SVG)
        removeSVGImage();
}

// removes the current point circle data
void DrawOperations::removePointCircleData() {
    // get the data and clear it out
    PointCircle *temp = (PointCircle *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;
}

// removes the current point square data
void DrawOperations::removePointSquareData() {
    // get the data and clear it out
    PointSquare *temp = (PointSquare *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;
}

// removes the current point X data
void DrawOperations::removePointXData() {
    // get the data and clear it out
    PointX *temp = (PointX *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;
}

// removes the current freehand line data
void DrawOperations::removeFreehandLine() {
    // get the line end and clear its data
    LineEnd *temp = (LineEnd *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;

    // move back an op and keep deleting ops while we have line points
    total_ops--;
    while(operations[total_ops].draw_operation == LINE_POINT) {
        LinePoint *tempb = (LinePoint *) &operations[total_ops];
        tempb->draw_operation = NO_DRAW;
        tempb->x = 0;
        tempb->y = 0;
        tempb->colour = 0;
        tempb->size = 0;
        total_ops--;
    }

    // get rid of the line start as well
    LineStart *tempc = (LineStart *) &operations[total_ops];
    tempc->draw_operation = NO_DRAW;
    tempc->x = 0;
    tempc->y = 0;
    tempc->colour = 0;
    tempc->size = 0;
}

// removes the current raster image data
void DrawOperations::removeRasterImage() {
    // get the raster data and clear it
    RasterImage *temp = (RasterImage *) &operations[total_ops];
    temp->x = 0;
    temp->y = 0;
    temp->width = 0;
    temp->height = 0;

    // delete the file name and image and set them to null
    delete temp->filename;
    delete temp->image;
    temp->filename = NULL;
    temp->image = NULL;
}

// removes the current straight line data
void DrawOperations::removeStraightLine() {
    // get the straight line end and clear its data
    StraightLineEnd *temp = (StraightLineEnd *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;

    // move back an op and get rid of the start point
    total_ops--;
    StraightLineStart *temp2 = (StraightLineStart *) &operations[total_ops];
    temp2->draw_operation = NO_DRAW;
    temp2->x = 0;
    temp2->y = 0;
    temp2->colour = 0;
    temp2->size = 0;
}

// removes the current SVG image data
void DrawOperations::removeSVGImage() {
    // get the raster data and clear it
    SVGImage *temp = (SVGImage *) &operations[total_ops];
    temp->x = 0;
    temp->y = 0;
    temp->width = 0;
    temp->height = 0;

    // delete the file name and image and set them to null
    delete temp->filename;
    delete temp->image;
    temp->filename = NULL;
    temp->image = NULL;
}

// removes the current text data
void DrawOperations::removeText() {
    // get the text data and clear its data
    Text *temp = (Text *) &operations[total_ops];
    temp->draw_operation = NO_DRAW;
    temp->x = 0;
    temp->y = 0;
    temp->colour = 0;
    temp->size = 0;
    temp->rotation = 0;

    // deallocate the string and set it as a null reference
    delete temp->string;
    temp->string = NULL;
}

// function that will double up the size of the arrays
void DrawOperations::doubleArrays() {
    // calculate the double of the current max ops
    // unsigned int new_max_ops = max_ops * 2;
    //
    // // allocate the arrays of the new size
    // unsigned int *new_draw_operation = (unsigned int*) new unsigned int[new_max_ops];
    // unsigned int *new_draw_x = (unsigned int*) new unsigned int[new_max_ops];
    // unsigned int *new_draw_y = (unsigned int*) new unsigned int[new_max_ops];
    // int *new_draw_red = (int*) new int[new_max_ops];
    // int *new_draw_green = (int*) new int[new_max_ops];
    // int *new_draw_blue = (int*) new int[new_max_ops];
    // int *new_draw_sizes = (int*) new int[new_max_ops];
    // int *new_draw_text_rotations = new int[new_max_ops];
    // int *new_draw_string_index = new int[new_max_ops];
    //
    // // copy accross the old arrays to the new arrays
    // for(unsigned int i = 0; i < max_ops; i++) {
    //     new_draw_operation[i] = draw_operation[i];
    //     new_draw_x[i] = draw_x[i];
    //     new_draw_y[i] = draw_y[i];
    //     new_draw_red[i] = draw_red[i];
    //     new_draw_green[i] = draw_green[i];
    //     new_draw_blue[i] = draw_blue[i];
    //     new_draw_sizes[i] = draw_sizes[i];
    //     new_draw_text_rotations[i] = draw_text_rotations[i];
    //     new_draw_string_index[i] = draw_string_index[i];
    // }
    //
    // // initialise everything after max ops
    // for(unsigned int i = max_ops; i < new_max_ops; i++) {
    //     new_draw_operation[i] = NO_DRAW;
    //     new_draw_x[i] = 0;
    //     new_draw_y[i] = 0;
    //     new_draw_red[i] = 0;
    //     new_draw_green[i] = 0;
    //     new_draw_blue[i] = 0;
    //     new_draw_sizes[i] = 0;
    //     new_draw_text_rotations[i] = 0;
    //     new_draw_string_index[i] = 0;
    // }
    //
    // // delete the old arrays
    // deallocateArrays();
    //
    // // assign the new arrays to the new names
    // draw_operation = new_draw_operation;
    // draw_x = new_draw_x;
    // draw_y = new_draw_y;
    // draw_red = new_draw_red;
    // draw_green = new_draw_green;
    // draw_blue = new_draw_blue;
    // draw_sizes = new_draw_sizes;
    // draw_text_rotations = new_draw_text_rotations;
    // draw_string_index = new_draw_string_index;
    //
    // // update the max ops for the new arrays
    // max_ops = new_max_ops;
}

// private function that will increase the size of the string array
void DrawOperations::doubleStringArray() {
    // calculate the size of the new array and allocate the array
    // unsigned int new_max_strings = max_strings * 2;
    // QString *new_draw_text_strings = new QString[new_max_strings];
    //
    // // copy the strings from the old array to the new array and delete the old array
    // for(unsigned int i = 0; i < total_strings; i++)
    //     new_draw_text_strings[i] = draw_text_strings[i];
    // delete[] draw_text_strings;
    // draw_text_strings = new_draw_text_strings;
    //
    // // also update the number of strings for the newly doubled array
    // max_strings = new_max_strings;
}

// function that will reset the entire drawoperations back to the starting state
void DrawOperations::reset() {
    // delete all of the arrays that are there
    // deallocateArrays();
    // delete[] draw_text_strings;
    //
    // // put new standard arrays in their place
    // total_ops = 0;
    // max_ops = 1024;
    // total_strings = 0;
    // max_strings = 8;
    // allocateArrays();
    //
    // // allocate an array of 32 QString objects
    // draw_text_strings = new QString[max_strings];
}

// function that will set the title of this image
void DrawOperations::setTitle(const QString& new_title) {
    title = new_title;
}
