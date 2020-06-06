#ifndef _DRAWOPERATIONS_HPP
#define _DRAWOPERATIONS_HPP

// drawoperations.hpp
//
// defines a container class that contains all the draw operations to recreate an image.
// these will be used to simplify drawing on screen and also for export purposes.
// initally these will start off at 4K draw ops but it will double in size if necessary

// includes
#include <QColor>

// class definition
class DrawOperations {
// public section of the class
public:
    // default constructor of the class that will make a 4K size draw ops avaialable
    DrawOperations();
    // constructor that will make a draw operations object with the given number of objects
    DrawOperations(const unsigned int max_ops);
    // destructor for the class
    ~DrawOperations();
    // adds draw data to the draw operations
    void addDrawData(unsigned int operation, int x, int y, QColor &colour, int draw_size);
    // removes the last set of draw data from this draw operations
    void removeLastDrawData();
    // private function that will increase the size of the draw ops arrays by doubling them
    void doubleArrays();
    // array containing the list of draw operations
    unsigned int *draw_operation;
    // arrays containing the list of x and y coordinates
    unsigned int *draw_x;
    unsigned int *draw_y;
    // colours of all the draw operations
    int *draw_red;
    int *draw_green;
    int *draw_blue;
    // the size of each draw operation
    int *draw_sizes;
    // how many operations in total in this draw operations
    unsigned int total_ops;
    // how many operations we can hold at the moment
    unsigned int max_ops;
};

#endif // _DRAWOPERATIONS_HPP
