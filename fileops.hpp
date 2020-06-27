#ifndef _FILEOPS_H
#define _FILEOPS_H

// fileops.hpp
//
// describes operations that are necessary for reading and writing files to and from disk

// includes
#include "drawoperations.hpp"

// function prototypes

// function that will determine what max images value was used for this total images
unsigned int determineMaxImages(unsigned int total_images);

// function that will determine what max ops we need for the images
unsigned int determineMaxOps(unsigned int total_ops);

// function that will determine and return a relative path given the location of the whiteboard, and the location of the image
QString determineRelativePath(QString whiteboard_path, QString image_path);

// function that will load a line end from disk
void loadLineEnd(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a line point from disk
void loadLinePoint(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a line end from disk
void loadLineStart(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a point circle from disk
void loadPointCircle(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a point square from disk
void loadPointSquare(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load an x point from disk
void loadPointX(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a QString from disk and will return it
QString loadQString(FILE *to_read);

// function that will load a raster image from disk
void loadRasterImage(DrawOperations *image, unsigned int draw_operation, FILE *to_read, QString &filename);

// function that will load a straight line start from disk
void loadStraightLineStart(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a straight line end from disk
void loadStraightLineEnd(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load an SVG image from disk
void loadSVGImage(DrawOperations *image, unsigned int draw_operation, FILE *to_read, QString &filename);

// function that will load a text op from disk
void loadText(DrawOperations *image, unsigned int draw_operation, FILE *to_read);

// function that will load a whiteboard from disk, this takes in points to variables where
// the max images and total images are stored
DrawOperations **loadWhiteboard(QString &filename, unsigned int *image_total, unsigned int *image_max);

// function that will recreate an absolute path from the current file location and a relative path
QString recreateAbsolutePath(QString whiteboard_path, QString relative_path);

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
void saveRasterImage(DrawOp *raster, FILE *to_write, QString &filename);

// function that will write the straight line start to disk
void saveStraightLineStart(DrawOp *point, FILE *to_write);

// function that will write the straight line end to disk
void saveStraightLineEnd(DrawOp *point, FILE *to_write);

// function that will write an SVG image to disk
void saveSVGImage(DrawOp *svg, FILE *to_write, QString &filename);

// function that will write a text operation to disk
void saveText(DrawOp *text, FILE *to_write);

// function that will take a whiteboard and write it to disk
void saveWhiteboard(QString &filename, DrawOperations **whiteboard, unsigned int total_images);

#endif // _FILEOPS_H
