// mainwindow.cpp
//
// implements the class defined in mainwindow.hpp

// includes
#include <QHBoxLayout>
#include <QPalette>
#include <QSizePolicy>
#include <QVBoxLayout>
#include "mainwindow.hpp"

// constructor for the class
MainWindow::MainWindow(QWidget *parent)
: QWidget(parent)
{
    // create a vbox layout for this widget
    QVBoxLayout *whiteboard_container_layout = new QVBoxLayout();
    this->setLayout(whiteboard_container_layout);

    // set a palette that we will use to paint the background to grey
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(128, 128, 128));

    // add a whiteboard to the layout and drop all of the margins
    Whiteboard *whiteboard = new Whiteboard();
    whiteboard_container_layout->addWidget(whiteboard);
    whiteboard_container_layout->setContentsMargins(0, 0, 0, 0);

    // create a widget with a hbox layout and add it to the main view
    QWidget *bottom_bar = new QWidget();
    bottom_bar->setAutoFillBackground(true);
    bottom_bar->setPalette(pal);
    bottom_bar->setMaximumHeight(32);
    QHBoxLayout *bottom_bar_layout = new QHBoxLayout();
    bottom_bar->setLayout(bottom_bar_layout);
    whiteboard_container_layout->addWidget(bottom_bar);

}

// destructor for the class
MainWindow::~MainWindow() {

}
