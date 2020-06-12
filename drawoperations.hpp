#ifndef _DRAWOPERATIONS_HPP
#define _DRAWOPERATIONS_HPP

// drawoperations.hpp
//
// defines a container class that contains all the draw operations to recreate an image.
// these will be used to simplify drawing on screen and also for export purposes.
// initally these will start off at 4K draw ops but it will double in size if necessary
//
// note that most of the fields have been made public here as from a performance point of
// view with the amount of times the arrays are potentially accessed for data it will save
// a lot of time to drop getter function calls on a redraw.

// includes
#include <QColor>

// class definition
class DrawOperations {
// public section of the class
public:
    // default constructor of the class that will make a 4K size draw ops avaialable
    DrawOperations();
    // constructor that will make a draw operations object with the given number of objects and
    // a maximum number of strings
    DrawOperations(const unsigned int max_ops, const unsigned int max_strings);
    // destructor for the class
    ~DrawOperations();
    // function that will allocate and all arrays to size max_ops except the strings array
    void allocateArrays();
    // adds draw data to the draw operations
    void addDrawData(unsigned int operation, int x, int y, QColor &colour, int draw_size);
    // adds in drawn text to the draw operations as this needs to be handled differently to the other operations
    void addDrawText(const QString &text, int x, int y, QColor &colour, int draw_size, int draw_rotation);
    // adds in a drawn raster image to the draw operations as this needs to be handled differently to other operations
    void addDrawRasterImage(const QString &file, int x, int y, int width, int height);
    // adds in a drawn vector image to the draw operations as this needs to be handled differently to other operations
    void addDrawSVGImage(const QString &file, int x, int y, int width, int height);
    // function that will deallocate all of the arrays of this draw operations
    void deallocateArrays();
    // removes the last set of draw data from this draw operations
    void removeLastDrawData();
    // private function that will increase the size of the draw ops arrays by doubling them
    void doubleArrays();
    // private function that will increase the size of the string array
    void doubleStringArray();
    // function that will reset the entire drawoperations back to the starting state
    void reset();
    // function that will set the title of this image
    void setTitle(const QString& new_title);
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
    // the rotation of each text operation
    int *draw_text_rotations;
    // index to the string that is to be rendered
    int *draw_string_index;
    // an array of QStrings that will hold all the text we are displaying
    QString *draw_text_strings;
    // how many operations in total in this draw operations
    unsigned int total_ops;
    // how many operations we can hold at the moment
    unsigned int max_ops;
    // how many strings and the max number of strings we have
    unsigned int total_strings;
    unsigned int max_strings;
    // the title of the current image
    QString title;
};

#endif // _DRAWOPERATIONS_HPP
