#ifndef _FILEOPS_H
#define _FILEOPS_H

// fileops.hpp
//
// describes operations that are necessary for reading and writing files to and from disk

// includes
#include "drawoperations.hpp"

// function prototypes

// function that will load a whiteboard from disk
DrawOperations **loadWhiteboard(QString &filename);

// function that will write a line end to disk
void saveLineEnd(DrawOp *point, FILE *to_write);

// function that will write a line start to disk
void saveLineStart(DrawOp *point, FILE *to_write);

// function that will write a line point to disk
void saveLinePoint(DrawOp *point, FILE *to_write);

// function that will write the given point circle to disk
void savePointCircle(DrawOp *point, FILE *to_write);

// function that will write the given point circle to disk
void savePointSquare(DrawOp *point, FILE *to_write);

// function that will write the given point x to disk
void savePointX(DrawOp *point, FILE *to_write);

// function that will take a qstring and will write it to disk
// it will do so by figuring out its length, write that first and then write the byte array
void saveQString(QString *string, FILE *to_write);

// function that will write a raster image operation to disk
void saveRasterImage(DrawOp *raster, FILE *to_write);

// function that will write the straight line start to disk
void saveStraightLineStart(DrawOp *point, FILE *to_write);

// function that will write the straight line end to disk
void saveStraightLineEnd(DrawOp *point, FILE *to_write);

// function that will write an SVG image to disk
void saveSVGImage(DrawOp *svg, FILE *to_write);

// function that will write a text operation to disk
void saveText(DrawOp *text, FILE *to_write);

// function that will take a whiteboard and write it to disk
void saveWhiteboard(QString &filename, DrawOperations **whiteboard, unsigned int total_images);

#endif // _FILEOPS_H
