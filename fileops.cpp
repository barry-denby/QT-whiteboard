// fileops.cpp
//
// implements everything described in fileops.hpp

// includes
#include <cstdio>
#include "constants.hpp"
#include "fileops.hpp"

// functions

// function that will load a whiteboard from disk
DrawOperations **loadWhiteboard(QString &filename) {
    // the draw operations that we will return to the caller
    DrawOperations **ops = NULL;

    // read in the the total number of images

    // // open up the file for reading in binary mode
    // FILE *to_read = fopen(filename.toStdString().c_str(), "rb");
    //
    // // read the total number from the file and then allocate the necessary array of pointers for the images
    // unsigned int total_images = 0, max_images = 0;
    // fread(&total_images, sizeof(unsigned int), 1, to_read);
    // fread(&max_images, sizeof(unsigned int), 1, to_read);
    // DrawOperations **loaded_images = (DrawOperations **) new DrawOperations *[max_images];
    //
    // // go through each of the images in turn and read them in
    // for(unsigned int i = 0; i < total_images; i++) {
    //     // first read in the total ops and the max ops for this image
    //     unsigned int total_ops = 0, max_ops = 0;
    //     fread(&total_ops, sizeof(unsigned int), 1, to_read);
    //     fread(&max_ops, sizeof(unsigned int), 1, to_read);
    //
    //     // read the length of the title and the title of the image
    //     char *image_title = new char[1024];
    //     unsigned int title_length = 0;
    //     fread(&title_length, sizeof(unsigned int), 1, to_read);
    //     fread(image_title, sizeof(char), title_length, to_read);
    //     QString title = QString::fromUtf8(QByteArray(image_title));
    //     loaded_images[i]->title = title;
    //     delete image_title;
    // }

    // return the whiteboard when we are finished with it
    return ops;
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
void saveRasterImage(DrawOp *raster, FILE *to_write) {
    // get a reference to a raster image
    RasterImage *temp = (RasterImage *) raster;

    // write all of the integer fields one by one to disk
    fwrite(&temp->draw_operation, sizeof(unsigned int), 1, to_write);
    fwrite(&temp->x, sizeof(int), 1, to_write);
    fwrite(&temp->y, sizeof(int), 1, to_write);
    fwrite(&temp->width, sizeof(int), 1, to_write);
    fwrite(&temp->height, sizeof(int), 1, to_write);

    // write the string filename to disk
    saveQString(temp->filename, to_write);
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
        // write the total ops and max ops to the file first
        fwrite(&whiteboard[i]->total_ops, sizeof(unsigned int), 1, to_write);

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
                saveRasterImage(temp, to_write);
        }
    }

    // write all the image titles to disk
    for(unsigned int i = 0; i < total_images; i++) {
        saveQString(&whiteboard[i]->title, to_write);
    }

    // close the file when we are finished
    fclose(to_write);

    // // go through each of the images in turn
    // for(unsigned int i = 0; i < total_images; i++) {
    //     // write the length of the title and the title of the image
    //     QByteArray image_title = images[i]->title.toUtf8();
    //     unsigned int length = (unsigned int) image_title.size();
    //     const char *data = image_title.data();
    //     fwrite(&length, sizeof(unsigned int), 1, to_write);
    //     fwrite(data, sizeof(char), length, to_write);
    // }
    //
    // // close the file when we are finished
    // fclose(to_write);

}
