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
    // initialise support for 1K drawing ops
    //allocateArrays();

    // allocate an array of QString objects
    //draw_text_strings = new QString[max_strings];
    //image_locations = new QString[max_images];

    // allocate the array of draw operations
    operations = new DrawOp[max_ops];
}

DrawOperations::DrawOperations(const unsigned int max_ops, const unsigned int max_strings, const unsigned int max_images)
: total_ops(0), max_ops(max_ops), total_strings(0), max_strings(max_strings), title(QString("")), total_images(0), max_images(max_images)
{
    // initialise all arrays
    //allocateArrays();

    // allocate an array of QString objects
    //draw_text_strings = new QString[max_strings];
    //image_locations = new QString[max_images];

    // allocate the array of draw operations
    operations = new DrawOp[max_ops];
}

// destructor for the class
DrawOperations::~DrawOperations() {
    // delete all of our arrays
    //deallocateArrays();
    //delete[] draw_text_strings;
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

// refactored function that will add in draw data to the necessary arrays
void DrawOperations::addDrawData(unsigned int operation, int x, int y, QColor& colour, int draw_size) {
    // add in the draw data
    // draw_operation[total_ops] = operation;
    // draw_x[total_ops] = x;
    // draw_y[total_ops] = y;
    // draw_red[total_ops] = colour.red();
    // draw_green[total_ops] = colour.green();
    // draw_blue[total_ops] = colour.blue();
    // draw_sizes[total_ops] = draw_size;
    // draw_text_rotations[total_ops] = 0;
    // draw_string_index[total_ops] = 0;
    // total_ops++;
    //
    // // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    // if(total_ops == max_ops)
    //     doubleArrays();
}

// adds in drawn text to the draw operations as this needs to be handled differently to the other operations
void DrawOperations::addDrawText(const QString &text, int x, int y, QColor &colour, int draw_size, int draw_rotation) {
    // add in the text draw data
    // draw_operation[total_ops] = DRAW_TEXT;
    // draw_x[total_ops] = x;
    // draw_y[total_ops] = y;
    // draw_red[total_ops] = colour.red();
    // draw_green[total_ops] = colour.green();
    // draw_blue[total_ops] = colour.blue();
    // draw_sizes[total_ops] = draw_size;
    // draw_text_rotations[total_ops] = draw_rotation;
    // draw_string_index[total_ops] = total_strings;
    // draw_text_strings[total_strings] = text;
    // total_ops++;
    // total_strings++;
    //
    // // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    // // do similar for the strings as well
    // if(total_ops == max_ops)
    //     doubleArrays();
    // if(total_strings == max_strings)
    //     doubleStringArray();
}

// adds in a drawn raster image to the draw operations as this needs to be handled differently to other operations
void DrawOperations::addDrawRasterImage(const QString &file, int x, int y, int width, int height) {
    // add in the text draw data we will reuse the red and green data for the width and height
    // we will also reuse the text strings to store the file reference
    // draw_operation[total_ops] = DRAW_RASTER;
    // draw_x[total_ops] = x;
    // draw_y[total_ops] = y;
    // draw_red[total_ops] = width;
    // draw_green[total_ops] = height;
    // draw_blue[total_ops] = 0;
    // draw_sizes[total_ops] = 0;
    // draw_text_rotations[total_ops] = 0;
    // draw_string_index[total_ops] = 0;
    // draw_text_strings[total_strings] = file;
    // total_ops++;
    // total_strings++;
    //
    // // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    // // do similar for the strings as well
    // if(total_ops == max_ops)
    //     doubleArrays();
    // if(total_strings == max_strings)
    //     doubleStringArray();
}

// adds in a drawn vector image to the draw operations as this needs to be handled differently to other operations
void DrawOperations::addDrawSVGImage(const QString &file, int x, int y, int width, int height) {
    // add in the text draw data we will reuse the red and green data for the width and height
    // we will also reuse the text strings to store the file reference
    // draw_operation[total_ops] = DRAW_SVG;
    // draw_x[total_ops] = x;
    // draw_y[total_ops] = y;
    // draw_red[total_ops] = width;
    // draw_green[total_ops] = height;
    // draw_blue[total_ops] = 0;
    // draw_sizes[total_ops] = 0;
    // draw_text_rotations[total_ops] = 0;
    // draw_string_index[total_ops] = 0;
    // draw_text_strings[total_strings] = file;
    // total_ops++;
    // total_strings++;
    //
    // // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    // // do similar for the strings as well
    // if(total_ops == max_ops)
    //     doubleArrays();
    // if(total_strings == max_strings)
    //     doubleStringArray();
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
    // if(total_ops == 0)
    //     return;
    //
    // // reduce total_ops by one and remove the data that is there
    // total_ops--;
    // if(draw_operation[total_ops] == DRAW_TEXT) {
    //     unsigned int string_index = draw_string_index[total_ops];
    //     draw_text_strings[string_index] = QString("");
    //     total_strings--;
    // }
    // draw_operation[total_ops] = NO_DRAW;
    // draw_x[total_ops] = 0;
    // draw_y[total_ops] = 0;
    // draw_red[total_ops] = 0;
    // draw_green[total_ops] = 0;
    // draw_blue[total_ops] = 0;
    // draw_sizes[total_ops] = 0;
    // draw_text_rotations[total_ops] = 0;
    // draw_string_index[total_ops] = 0;
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
