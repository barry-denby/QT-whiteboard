// main.cpp
//
// main file for the qt based whiteboard. this program will generate a whiteboard
// that can be drawn on and if possible should enable undo operations and also
// saving and dumping of images to file using PNG format.

// includes
#include <iostream>
#include <QApplication>
#include "mainwindow.hpp"

// entry point to our program
int main(int argc, char** argv) {
    // create a QApplication that will manage everything for the QT toolkit
    QApplication app(argc, argv);

    // create a whiteboard and show it
    //Whiteboard board;
    MainWindow board;
    //board.showFullScreen();
    board.showMaximized();

    // return the status of the application
    return app.exec();
}
