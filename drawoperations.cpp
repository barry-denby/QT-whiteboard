// drawoperations.cpp
//
// implenents everything described in drawoperations.hpp

// includes
#include <iostream>
#include "constants.hpp"
#include "drawoperations.hpp"

// default constructor for the class that will initialise a 4K sized draw operations object
DrawOperations::DrawOperations()
: total_ops(0), max_ops(1024), total_strings(0), max_strings(8)
{
    // initialise support for 1K drawing ops
    draw_operation = (unsigned int*) new unsigned int[max_ops];
    draw_x = (unsigned int*) new unsigned int[max_ops];
    draw_y = (unsigned int*) new unsigned int[max_ops];
    draw_red = (int *) new int[max_ops];
    draw_green = (int *) new int[max_ops];
    draw_blue = (int *) new int[max_ops];
    draw_sizes = (int *) new int[max_ops];
    draw_text_rotations = (int *) new int[max_ops];
    draw_string_index = (int *) new int[max_ops];
    for(unsigned int i = 0; i < max_ops; i++) {
        draw_operation[i] = 0;
        draw_x[i] = 0;
        draw_y[i] = 0;
        draw_red[i] = 0;
        draw_green[i] = 0;
        draw_blue[i] = 0;
        draw_sizes[i] = 0;
        draw_string_index[i] = 0;
        draw_text_rotations[i] = 0;
    }

    // allocate an array of 32 QString objects
    draw_text_strings = new QString[max_strings];
}

DrawOperations::DrawOperations(const unsigned int max_ops)
: total_ops(0), max_ops(max_ops), total_strings(0), max_strings(8)
{
    // initialise support for 4K drawing ops
    draw_operation = (unsigned int*) new unsigned int[max_ops];
    draw_x = (unsigned int*) new unsigned int[max_ops];
    draw_y = (unsigned int*) new unsigned int[max_ops];
    draw_red = (int *) new int[max_ops];
    draw_green = (int *) new int[max_ops];
    draw_blue = (int *) new int[max_ops];
    draw_sizes = (int *) new int[max_ops];
    draw_string_index = new int[max_ops];
    draw_text_rotations = new int[max_ops];
    for(unsigned int i = 0; i < max_ops; i++) {
        draw_operation[i] = 0;
        draw_x[i] = 0;
        draw_y[i] = 0;
        draw_red[i] = 0;
        draw_green[i] = 0;
        draw_blue[i] = 0;
        draw_sizes[i] = 0;
        draw_string_index[i] = 0;
        draw_text_rotations[i] = 0;
    }

    // allocate an array of 32 QString objects
    draw_text_strings = new QString[max_strings];
}

// destructor for the class
DrawOperations::~DrawOperations() {
    // delete all of our arrays
    delete draw_operation;
    delete draw_x;
    delete draw_y;
    delete draw_red;
    delete draw_green;
    delete draw_blue;
    delete draw_sizes;
    delete draw_text_rotations;
    delete draw_string_index;
    delete[] draw_text_strings;
}

// refactored function that will add in draw data to the necessary arrays
void DrawOperations::addDrawData(unsigned int operation, int x, int y, QColor& colour, int draw_size) {
    // add in the draw data
    draw_operation[total_ops] = operation;
    draw_x[total_ops] = x;
    draw_y[total_ops] = y;
    draw_red[total_ops] = colour.red();
    draw_green[total_ops] = colour.green();
    draw_blue[total_ops] = colour.blue();
    draw_sizes[total_ops] = draw_size;
    draw_text_rotations[total_ops] = 0;
    draw_string_index[total_ops] = 0;
    total_ops++;

    // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    if(total_ops == max_ops)
        doubleArrays();
}

// adds in drawn text to the draw operations as this needs to be handled differently to the other operations
void DrawOperations::addDrawText(const QString &text, int x, int y, QColor &colour, int draw_size, int draw_rotation) {
    // add in the text draw data
    draw_operation[total_ops] = DRAW_TEXT;
    draw_x[total_ops] = x;
    draw_y[total_ops] = y;
    draw_red[total_ops] = colour.red();
    draw_green[total_ops] = colour.green();
    draw_blue[total_ops] = colour.blue();
    draw_sizes[total_ops] = draw_size;
    draw_text_rotations[total_ops] = draw_rotation;
    draw_string_index[total_ops] = total_strings;
    draw_text_strings[total_strings] = text;
    total_ops++;
    total_strings++;

    // if the total ops is equal the the max ops then we don't have any more room so double the memory.
    // do similar for the strings as well
    if(total_ops == max_ops)
        doubleArrays();
    if(total_strings == max_strings)
        doubleStringArray();
}

// refactored function that will remove the last set of draw data from the arrays
void DrawOperations::removeLastDrawData() {
    // if the next op is already zero then we cant remove anything
    if(total_ops == 0)
        return;

    // reduce total_ops by one and remove the data that is there
    total_ops--;
    if(draw_operation[total_ops] == DRAW_TEXT) {
        unsigned int string_index = draw_string_index[total_ops];
        draw_text_strings[string_index] = QString("");
        total_strings--;
    }
    draw_operation[total_ops] = NO_DRAW;
    draw_x[total_ops] = 0;
    draw_y[total_ops] = 0;
    draw_red[total_ops] = 0;
    draw_green[total_ops] = 0;
    draw_blue[total_ops] = 0;
    draw_sizes[total_ops] = 0;
    draw_text_rotations[total_ops] = 0;
    draw_string_index[total_ops] = 0;
}

// function that will double up the size of the arrays
void DrawOperations::doubleArrays() {
    // calculate the double of the current max ops
    unsigned int new_max_ops = max_ops * 2;

    // allocate the arrays of the new size
    unsigned int *new_draw_operation = (unsigned int*) new unsigned int[new_max_ops];
    unsigned int *new_draw_x = (unsigned int*) new unsigned int[new_max_ops];
    unsigned int *new_draw_y = (unsigned int*) new unsigned int[new_max_ops];
    int *new_draw_red = (int*) new int[new_max_ops];
    int *new_draw_green = (int*) new int[new_max_ops];
    int *new_draw_blue = (int*) new int[new_max_ops];
    int *new_draw_sizes = (int*) new int[new_max_ops];
    int *new_draw_text_rotations = new int[new_max_ops];
    int *new_draw_string_index = new int[new_max_ops];

    // copy accross the old arrays to the new arrays
    for(unsigned int i = 0; i < max_ops; i++) {
        new_draw_operation[i] = draw_operation[i];
        new_draw_x[i] = draw_x[i];
        new_draw_y[i] = draw_y[i];
        new_draw_red[i] = draw_red[i];
        new_draw_green[i] = draw_green[i];
        new_draw_blue[i] = draw_blue[i];
        new_draw_sizes[i] = draw_sizes[i];
        new_draw_text_rotations[i] = draw_text_rotations[i];
        new_draw_string_index[i] = draw_string_index[i];
    }

    // delete the old arrays
    delete draw_operation;
    delete draw_x;
    delete draw_y;
    delete draw_red;
    delete draw_green;
    delete draw_blue;
    delete draw_sizes;
    delete draw_text_rotations;
    delete draw_string_index;

    // assign the new arrays to the new names
    draw_operation = new_draw_operation;
    draw_x = new_draw_x;
    draw_y = new_draw_y;
    draw_red = new_draw_red;
    draw_green = new_draw_green;
    draw_blue = new_draw_blue;
    draw_sizes = new_draw_sizes;
    draw_text_rotations = new_draw_text_rotations;
    draw_string_index = new_draw_string_index;
}

// private function that will increase the size of the string array
void DrawOperations::doubleStringArray() {
    // calculate the size of the new array and allocate the array
    unsigned int new_max_strings = max_strings * 2;
    QString *new_draw_text_strings = new QString[new_max_strings];

    // copy the strings from the old array to the new array and delete the old array
    for(unsigned int i = 0; i < total_strings; i++)
        new_draw_text_strings[i] = draw_text_strings[i];
    delete[] draw_text_strings;
    draw_text_strings = new_draw_text_strings;
}

// function that will reset the entire drawoperations back to the starting state
void DrawOperations::reset() {
    // delete all of the arrays that are there
    delete draw_operation;
    delete draw_x;
    delete draw_y;
    delete draw_red;
    delete draw_green;
    delete draw_blue;
    delete draw_sizes;
    delete draw_text_rotations;
    delete draw_string_index;
    delete[] draw_text_strings;

    // put new standard arrays in their place
    total_ops = 0;
    max_ops = 1024;
    total_strings = 0;
    max_strings = 8;
    draw_operation = (unsigned int*) new unsigned int[max_ops];
    draw_x = (unsigned int*) new unsigned int[max_ops];
    draw_y = (unsigned int*) new unsigned int[max_ops];
    draw_red = (int *) new int[max_ops];
    draw_green = (int *) new int[max_ops];
    draw_blue = (int *) new int[max_ops];
    draw_sizes = (int *) new int[max_ops];
    draw_string_index = new int[max_ops];
    draw_text_rotations = new int[max_ops];
    for(unsigned int i = 0; i < max_ops; i++) {
        draw_operation[i] = 0;
        draw_x[i] = 0;
        draw_y[i] = 0;
        draw_red[i] = 0;
        draw_green[i] = 0;
        draw_blue[i] = 0;
        draw_sizes[i] = 0;
        draw_string_index[i] = 0;
        draw_text_rotations[i] = 0;
    }

    // allocate an array of 32 QString objects
    draw_text_strings = new QString[max_strings];
}
