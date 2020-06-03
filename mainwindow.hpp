#ifndef _MAINWINDOW_HPP
#define _MAINWINDOW_HPP

// mainwindow.hpp
//
// class that defines a fullscreen window that will have options for assisting with the whiteboard

// includes
#include <QHBoxLayout>
#include <QWidget>
#include "colourselector.hpp"
#include "whiteboard.hpp"

// class definition
class MainWindow : public QWidget {
    // gives us access to the signals and slots mechanism
    Q_OBJECT
// public section of the class
public:
    // default constructor for the class
    MainWindow(QWidget *parent = 0);
    // destructor for the class
    ~MainWindow();
// private section of the class
private:
    // function that will generate and attach a colour picker to the given layout
    void generateColourSelector(QHBoxLayout *layout, int red, int green, int blue);
    // function that will generate a hboxlayout with no contents margins in it
    QHBoxLayout *generateHBoxNoMargins();
    // function that will setup a basic toolbar. this is refactored out as we have two toolbars
    QWidget *generateToolbar();
    // whiteboard that everything will be drawn on
    Whiteboard *whiteboard;
};

#endif // _MAINWINDOW_HPP
