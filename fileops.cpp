// fileops.cpp
//
// implements everything described in fileops.hpp

// includes
#include <cstdio>
#include <iostream>
#include <QStringList>
#include <QRegularExpression>
#include "constants.hpp"
#include "fileops.hpp"

// functions

// function that will determine what max images value was used for this total images
unsigned int determineMaxImages(unsigned int total_images) {
    // the calculated max images set to its base value
    unsigned max_images = 16;

    // figure out the total number of images we need and return it
    while(max_images < total_images)
        max_images <<= 1;
    return max_images;
}

// function that will determine what max ops we need for the images
unsigned int determineMaxOps(unsigned int total_ops) {
    // the calculated max ops set to its base value
    unsigned int max_ops = 1024;

    // figure out the total number of ops we need and return it
    while(max_ops < total_ops)
        max_ops <<= 1;
    return max_ops;
}

// function that will determine and return a relative path given the location of the whiteboard, and the location of the image
QString determineRelativePath(QString whiteboard_path, QString image_path) {
    // split both strings into their respective tokens
    QStringList whiteboard_tokens = whiteboard_path.split(QRegularExpression("/"));
    QStringList image_tokens = image_path.split(QRegularExpression("/"));

    // while the head of both lists match remove the head
    while(QString::compare(whiteboard_tokens.front(), image_tokens.front()) == 0) {
        whiteboard_tokens.removeFirst();
        image_tokens.removeFirst();
    }

    for(int i = 0; i < whiteboard_tokens.size(); i++)
        std::cout << whiteboard_tokens.at(i).toStdString() << std::endl;

    for(int i = 0; i < image_tokens.size(); i++)
        std::cout << image_tokens.at(i).toStdString() << std::endl;

    // one of three things will happen depending on the size of the lists
    if(whiteboard_tokens.size() == 1 && image_tokens.size() == 1) {
        // the image is in the same directory as the whiteboard therefore we just need the filename
        return image_tokens.first();
    }

    // keep the compiler happy for now
    return QString("");
}

// function that will load a line end from disk
void loadLineEnd(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the line end and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawFreehandEnd(x, y, colour, size);
}

// function that will load a line point from disk
void loadLinePoint(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the line end and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawFreehandMid(x, y, colour, size);
}

// function that will load a line end from disk
void loadLineStart(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the line start and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawFreehandStart(x, y, colour, size);
}

// function that will load a point circle from disk
void loadPointCircle(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the point circle and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawPointCircle(x, y, colour, size);
}

// function that will load a point square from disk
void loadPointSquare(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the point square and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawPointSquare(x, y, colour, size);
}

// function that will load an x point from disk
void loadPointX(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the point x and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawPointX(x, y, colour, size);
}

// function that will load a QString from disk and will return it
QString loadQString(FILE *to_read) {
    // read in the length of the string from disk
    unsigned int length = 0;
    fread(&length, sizeof(unsigned int), 1, to_read);

    // allocate the necessary space for the character data and read it in
    char *data = new char[length];
    fread(data, sizeof(char), length, to_read);

    // get and return the qstring. make sure to deallocate the array before returning
    QString temp = QString::fromUtf8(data, length);
    //delete data;
    return temp;
}

// function that will load a raster image from disk
void loadRasterImage(DrawOperations *image, unsigned int draw_operation, FILE *to_read, QString &filename) {
    // read in the four integers
    int x = 0, y = 0, width = 0, height = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&width, sizeof(int), 1, to_read);
    fread(&height, sizeof(int), 1, to_read);

    // read in the filename and add in the draw image
    QString temp = loadQString(to_read);
    QString full_path = recreateAbsolutePath(filename, temp);
    image->addDrawRasterImage(full_path, x, y, width, height);
}

// function that will load a point square from disk
void loadStraightLineStart(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the straight line start and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawStraightLineStart(x, y, colour, size);
}

// function that will load a straight line end from disk
void loadStraightLineEnd(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in all four values for the straight line end and add it into the image
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    image->addDrawStraightLineEnd(x, y, colour, size);
}

// function that will load an SVG image from disk
void loadSVGImage(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in the four integers
    int x = 0, y = 0, width = 0, height = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&width, sizeof(int), 1, to_read);
    fread(&height, sizeof(int), 1, to_read);

    // read in the filename and add in the draw image
    QString temp = loadQString(to_read);
    image->addDrawRasterImage(temp, x, y, width, height);
}

// function that will load a text op from disk
void loadText(DrawOperations *image, unsigned int draw_operation, FILE *to_read) {
    // read in the five integer values from disk
    unsigned int colour = 0;
    int x = 0, y = 0, size = 0, rotation = 0;
    fread(&x, sizeof(int), 1, to_read);
    fread(&y, sizeof(int), 1, to_read);
    fread(&colour, sizeof(unsigned int), 1, to_read);
    fread(&size, sizeof(int), 1, to_read);
    fread(&rotation, sizeof(int), 1, to_read);

    // read in the text and add in the text draw operation
    QString temp = loadQString(to_read);
    image->addDrawText(temp, x, y, colour, size, rotation);

}

// function that will load a whiteboard from disk
DrawOperations **loadWhiteboard(QString &filename, unsigned int *image_total, unsigned int *image_max) {
    // the draw operations that we will return to the caller
    DrawOperations **ops = NULL;

    // open the file for reading
    FILE *to_read = fopen(filename.toStdString().c_str(), "rb");

    // read in the total number of images for this file
    unsigned int total_images = 0;
    fread(&total_images, sizeof(unsigned int), 1, to_read);

    // figure out what our max images is and then allocate our draw images
    unsigned int max_images = determineMaxImages(total_images);
    ops = (DrawOperations **) new DrawOperations *[max_images];

    // go through each of the images in turn and read it in
    for(unsigned int i = 0; i < total_images; i++) {
        // read in the total number of ops for this image and then the required max ops, then allocate the necessary image
        unsigned int total_ops = 0;
        fread(&total_ops, sizeof(unsigned int), 1, to_read);
        unsigned int max_ops = determineMaxOps(total_ops);
        ops[i] = (DrawOperations *) new DrawOperations(max_ops);

        // read in the locked op and the locked state for this image
        unsigned int locked_ops = 0;
        bool locked = false;
        fread(&locked_ops, sizeof(unsigned int), 1, to_read);
        fread(&locked, sizeof(bool), 1, to_read);
        ops[i]->locked = locked;
        ops[i]->locked_op = locked_ops;

        // go through each of the ops in turn and read them in
        for(unsigned int j = 0; j < total_ops; j++) {
            // read in the draw op from disk
            unsigned int draw_operation = 0;
            fread(&draw_operation, sizeof(unsigned int), 1, to_read);

            // see what draw op we have and read in as appropriate
            if(draw_operation == POINT_CIRCLE)
                loadPointCircle(ops[i], draw_operation, to_read);
            else if(draw_operation == POINT_SQUARE)
                loadPointSquare(ops[i], draw_operation, to_read);
            else if(draw_operation == POINT_X)
                loadPointX(ops[i], draw_operation, to_read);
            else if(draw_operation == STRAIGHT_LINE_START)
                loadStraightLineStart(ops[i], draw_operation, to_read);
            else if(draw_operation == STRAIGHT_LINE_END)
                loadStraightLineEnd(ops[i], draw_operation, to_read);
            else if(draw_operation == LINE_END)
                loadLineEnd(ops[i], draw_operation, to_read);
            else if(draw_operation == LINE_POINT)
                loadLinePoint(ops[i], draw_operation, to_read);
            else if(draw_operation == LINE_START)
                loadLineStart(ops[i], draw_operation, to_read);
            else if(draw_operation == DRAW_TEXT)
                loadText(ops[i], draw_operation, to_read);
            else if(draw_operation == DRAW_RASTER)
                loadRasterImage(ops[i], draw_operation, to_read, filename);
            else if(draw_operation == DRAW_SVG)
                loadSVGImage(ops[i], draw_operation, to_read);
        }
    }

    // read in the titles for each of the images
    for(unsigned int i = 0; i < total_images; i++) {
        ops[i]->title = loadQString(to_read);
    }

    // close the file when we are finished
    fclose(to_read);

    // any images that have not been loaded fill them with empty images
    for(unsigned int i = total_images; i < max_images; i++)
        ops[i] = (DrawOperations *) new DrawOperations();

    // return the images, max images, and total images
    *image_max = max_images;
    *image_total = total_images;
    return ops;
}

// function that will recreate an absolute path from the current file location and a relative path
QString recreateAbsolutePath(QString whiteboard_path, QString relative_path) {
    // the full path to be returned
    QString full_path;

    // convert the whiteboard path into a string list and remove the filename from the end
    QStringList whiteboard_tokens = whiteboard_path.split(QRegularExpression("/"));
    whiteboard_tokens.removeLast();

    // convert the relative_path into a string list as well
    QStringList relative_tokens = relative_path.split(QRegularExpression("/"));

    // if there is only one relative token this means the file in in the current directory
    if(relative_tokens.size() == 1) {
        // add the token to the end of the whiteboard tokens and convert it into a full path
        whiteboard_tokens.append(relative_tokens.first());
        full_path = whiteboard_tokens.join("/");
        std::cout << full_path.toStdString() << std::endl;
    }

    // keep the compiler happy for now
    return full_path;
}

// function that will write a line end to disk
void saveLineEnd(DrawOp *point, FILE *to_write) {
    // get a reference to a point circle and write it to disk in one go
    LineEnd *temp = (LineEnd *) point;
    fwrite(temp, sizeof(LineEnd), 1, to_write);
}

// function that will write a line start to disk
void saveLineStart(DrawOp *point, FILE *to_write) {
    // get a reference to a point circle and write it to disk in one go
    LineStart *temp = (LineStart *) point;
    fwrite(temp, sizeof(LineStart), 1, to_write);
}

// function that will write a line point to disk
void saveLinePoint(DrawOp *point, FILE *to_write) {
    // get a reference to a point circle and write it to disk in one go
    LinePoint *temp = (LinePoint *) point;
    fwrite(temp, sizeof(LinePoint), 1, to_write);
}

// function that will write the given point circle to disk
void savePointCircle(DrawOp *point, FILE *to_write) {
    // get a reference to a point circle and write it to disk in one go
    PointCircle *temp = (PointCircle *) point;
    fwrite(temp, sizeof(PointCircle), 1, to_write);
}

// function that will write the given point circle to disk
void savePointSquare(DrawOp *point, FILE *to_write) {
    // get a reference to a point square and write it to disk in one go
    PointSquare *temp = (PointSquare *) point;
    fwrite(temp, sizeof(PointSquare), 1, to_write);
}

// function that will write the given point x to disk
void savePointX(DrawOp *point, FILE *to_write) {
    // get a reference to a point X and write it to disk in one go
    PointX *temp = (PointX *) point;
    fwrite(temp, sizeof(PointX), 1, to_write);
}

// function that will take a qstring and will write it to disk
// it will do so by figuring out its length, write that first and then write the byte array
void saveQString(QString *string, FILE *to_write) {
    // get the byte array from the string in UTF-8 format and get the length of that byte array
    QByteArray ba_string = string->toUtf8();
    unsigned int length = (unsigned int) ba_string.size();

    // write the length and the bytes to disk
    const char *data = ba_string.data();
    fwrite(&length, sizeof(unsigned int), 1, to_write);
    fwrite(data, sizeof(char), length, to_write);
}

// function that will write a raster image operation to disk
void saveRasterImage(DrawOp *raster, FILE *to_write, QString &filename) {
    // get a reference to a raster image
    RasterImage *temp = (RasterImage *) raster;

    // write all of the integer fields one by one to disk
    fwrite(&temp->draw_operation, sizeof(unsigned int), 1, to_write);
    fwrite(&temp->x, sizeof(int), 1, to_write);
    fwrite(&temp->y, sizeof(int), 1, to_write);
    fwrite(&temp->width, sizeof(int), 1, to_write);
    fwrite(&temp->height, sizeof(int), 1, to_write);

    // write the string filename to disk
    //saveQString(temp->filename, to_write);

    // write the string filename to disk using a relative path
    QString relative = determineRelativePath(filename, *temp->filename);
    std::cout << relative.toStdString() << std::endl;
    saveQString(&relative, to_write);
}

// function that will write the straight line start to disk
void saveStraightLineStart(DrawOp *point, FILE *to_write) {
    // get a reference to a straight line start and write it to disk in one go
    StraightLineStart *temp = (StraightLineStart *) point;
    fwrite(temp, sizeof(StraightLineStart), 1, to_write);
}

// function that will write the straight line end to disk
void saveStraightLineEnd(DrawOp *point, FILE *to_write) {
    // get a reference to a straight line start and write it to disk in one go
    StraightLineEnd *temp = (StraightLineEnd *) point;
    fwrite(temp, sizeof(StraightLineStart), 1, to_write);
}

// function that will write an SVG image to disk
void saveSVGImage(DrawOp *svg, FILE *to_write) {
    // get a reference to the SVG image
    SVGImage *temp = (SVGImage *) svg;

    // write all of the integer fields one by one to disk
    fwrite(&temp->draw_operation, sizeof(unsigned int), 1, to_write);
    fwrite(&temp->x, sizeof(int), 1, to_write);
    fwrite(&temp->y, sizeof(int), 1, to_write);
    fwrite(&temp->width, sizeof(int), 1, to_write);
    fwrite(&temp->height, sizeof(int), 1, to_write);

    // write the string filename to disk
    saveQString(temp->filename, to_write);
}

// function that will write a text operation to disk
void saveText(DrawOp *text, FILE *to_write) {
    // get a reference to a text
    Text *temp = (Text *) text;

    // write all of the integer fields one by one to disk
    fwrite(&temp->draw_operation, sizeof(unsigned int), 1, to_write);
    fwrite(&temp->x, sizeof(int), 1, to_write);
    fwrite(&temp->y, sizeof(int), 1, to_write);
    fwrite(&temp->colour, sizeof(unsigned int), 1, to_write);
    fwrite(&temp->size, sizeof(int), 1, to_write);
    fwrite(&temp->rotation, sizeof(int), 1, to_write);

    // write the string filename to disk
    saveQString(temp->string, to_write);

}

// function that will take a whiteboard and write it to disk
void saveWhiteboard(QString &filename, DrawOperations **whiteboard, unsigned int total_images) {
    // open up the file for writing in binary mode
    FILE *to_write = fopen(filename.toStdString().c_str(), "wb");

    // write the numbers for images to the file
    fwrite(&total_images, sizeof(unsigned int), 1, to_write);

    // go through each of the images in turn
    for(unsigned int i = 0; i < total_images; i++) {
        // write the total ops, the lock ops and lock state to the file first
        fwrite(&whiteboard[i]->total_ops, sizeof(unsigned int), 1, to_write);
        fwrite(&whiteboard[i]->locked_op, sizeof(unsigned int), 1, to_write);
        fwrite(&whiteboard[i]->locked, sizeof(bool), 1, to_write);

        // go through each of the ops in the image and write them to disk depnding on what ops we have
        for(unsigned int j = 0; j < whiteboard[i]->total_ops; j++) {
            // pull out the current operation as we will be referencing it a lot
            DrawOp *temp = &whiteboard[i]->operations[j];

            // see what operation we have and write it to disk with the appropriate method
            if(temp->draw_operation == POINT_CIRCLE)
                savePointCircle(temp, to_write);
            else if(temp->draw_operation == POINT_SQUARE)
                savePointSquare(temp, to_write);
            else if(temp->draw_operation == POINT_X)
                savePointX(temp, to_write);
            else if(temp->draw_operation == STRAIGHT_LINE_START)
                saveStraightLineStart(temp, to_write);
            else if(temp->draw_operation == STRAIGHT_LINE_END)
                saveStraightLineEnd(temp, to_write);
            else if(temp->draw_operation == LINE_START)
                saveLineStart(temp, to_write);
            else if(temp->draw_operation == LINE_POINT)
                saveLinePoint(temp, to_write);
            else if(temp->draw_operation == LINE_END)
                saveLineEnd(temp, to_write);
            else if(temp->draw_operation == DRAW_TEXT)
                saveText(temp, to_write);
            else if(temp->draw_operation == DRAW_RASTER)
                saveRasterImage(temp, to_write, filename);
            else if(temp->draw_operation == DRAW_SVG)
                saveSVGImage(temp, to_write);
        }
    }

    // write all the image titles to disk
    for(unsigned int i = 0; i < total_images; i++) {
        saveQString(&whiteboard[i]->title, to_write);
    }

    // close the file when we are finished
    fclose(to_write);
}
