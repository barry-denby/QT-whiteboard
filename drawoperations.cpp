// drawoperations.cpp
//
// implenents everything described in drawoperations.hpp

// includes
#include "constants.hpp"
#include "drawoperations.hpp"

// default constructor for the class that will initialise a 4K sized draw operations object
DrawOperations::DrawOperations()
: total_ops(0), max_ops(4096)
{
    // initialise support for 4K drawing ops
    draw_operation = (unsigned int*) new unsigned int[max_ops];
    draw_x = (unsigned int*) new unsigned int[max_ops];
    draw_y = (unsigned int*) new unsigned int[max_ops];
    draw_red = (int *) new int[max_ops];
    draw_green = (int *) new int[max_ops];
    draw_blue = (int *) new int[max_ops];
    draw_sizes = (int *) new int[max_ops];
    for(unsigned int i = 0; i < max_ops; i++) {
        draw_operation[i] = 0;
        draw_x[i] = 0;
        draw_y[i] = 0;
        draw_red[i] = 0;
        draw_green[i] = 0;
        draw_blue[i] = 0;
        draw_sizes[i] = 0;
    }
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
    total_ops++;

    // if the total ops is equal the the max ops then we don't have any more room so double the memory
    if(total_ops == max_ops)
        doubleArrays();
}

// refactored function that will remove the last set of draw data from the arrays
void DrawOperations::removeLastDrawData() {
    // if the next op is already zero then we cant remove anything
    if(total_ops == 0)
        return;

    // reduce total_ops by one and remove the data that is there
    total_ops--;
    draw_operation[total_ops] = NO_DRAW;
    draw_x[total_ops] = 0;
    draw_y[total_ops] = 0;
    draw_red[total_ops] = 0;
    draw_green[total_ops] = 0;
    draw_blue[total_ops] = 0;
    draw_sizes[total_ops] = 0;
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

    // copy accross the old arrays to the new arrays
    for(unsigned int i = 0; i < max_ops; i++) {
        new_draw_operation[i] = draw_operation[i];
        new_draw_x[i] = draw_x[i];
        new_draw_y[i] = draw_y[i];
        new_draw_red[i] = draw_red[i];
        new_draw_green[i] = draw_green[i];
        new_draw_blue[i] = draw_blue[i];
        new_draw_sizes[i] = draw_sizes[i];
    }

    // delete the old arrays
    delete draw_operation;
    delete draw_x;
    delete draw_y;
    delete draw_red;
    delete draw_green;
    delete draw_blue;
    delete draw_sizes;

    // assign the new arrays to the new names
    draw_operation = new_draw_operation;
    draw_x = new_draw_x;
    draw_y = new_draw_y;
    draw_red = new_draw_red;
    draw_green = new_draw_green;
    draw_blue = new_draw_blue;
    draw_sizes = new_draw_sizes;
}
