// mainwindow.cpp
//
// implements the class defined in mainwindow.hpp

// includes
#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QSizePolicy>
#include <QSpinBox>
#include <QVBoxLayout>
#include "constants.hpp"
#include "colourselector.hpp"
#include "mainwindow.hpp"
#include "toolselector.hpp"

// constructor for the class
MainWindow::MainWindow(QWidget *parent)
: QWidget(parent)
{
    // create a vbox layout for this widget
    QVBoxLayout *whiteboard_container_layout = new QVBoxLayout();
    this->setLayout(whiteboard_container_layout);

    // set a palette that we will use to paint the background to grey
    //QPalette pal = palette();
    //pal.setColor(QPalette::Background, QColor(128, 128, 128));

    // add a whiteboard to the layout and drop all of the margins
    whiteboard = new Whiteboard();
    whiteboard_container_layout->addWidget(whiteboard);
    whiteboard_container_layout->setContentsMargins(0, 0, 0, 0);

    // create a widget with a hbox layout and add it to the main view
    //QWidget *bottom_bar = new QWidget();
    //bottom_bar->setAutoFillBackground(true);
    //bottom_bar->setPalette(pal);
    //bottom_bar->setMaximumHeight(32);
    //QHBoxLayout *bottom_bar_layout = new QHBoxLayout();
    //bottom_bar_layout->setContentsMargins(0, 0, 0, 0);
    //bottom_bar->setLayout(bottom_bar_layout);
    QWidget *toolbar = generateToolbar();
    QHBoxLayout *toolbar_layout = generateHBoxNoMargins();
    toolbar->setLayout(toolbar_layout);
    whiteboard_container_layout->addWidget(toolbar);

    // add in colour selectors to the bottom bar
    ColourSelector *temp = new ColourSelector(0, 0, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(128, 0, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(255, 0, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 128, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 255, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 0, 128);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 0, 255);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(128, 128, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(255, 255, 0);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(128, 0, 128);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(255, 0, 255);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 128, 128);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(0, 255, 255);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    temp = new ColourSelector(255, 255, 255);
    toolbar_layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));

    // add in a label and spinbox for the point size
    QLabel *point_size_label = new QLabel("Point Size:");
    toolbar_layout->addWidget(point_size_label);
    QSpinBox *point_size_spinbox = new QSpinBox();
    point_size_spinbox->setRange(1, 100);
    point_size_spinbox->setValue(2);
    toolbar_layout->addWidget(point_size_spinbox);

    // add in a label and spin box for the line thickness
    QLabel *line_thickness_label = new QLabel("Line Thickness:");
    toolbar_layout->addWidget(line_thickness_label);
    QSpinBox *line_thickness_spinbox = new QSpinBox();
    line_thickness_spinbox->setRange(1, 100);
    line_thickness_spinbox->setValue(1);
    toolbar_layout->addWidget(line_thickness_spinbox);

    // add in a label for tools
    QLabel *tools_label = new QLabel("Tools:");
    toolbar_layout->addWidget(tools_label);

    // add in our normal point tool
    ToolSelector *tool_selector = new ToolSelector(OP_POINT_FIXED_SIZE);
    toolbar_layout->addWidget(tool_selector);

    // add in our line tool
    tool_selector = new ToolSelector(OP_LINE_FIXED_THICKNESS);
    toolbar_layout->addWidget(tool_selector);

    // add in our variable point tool
    tool_selector = new ToolSelector(OP_POINT_VARIABLE_SIZE);
    toolbar_layout->addWidget(tool_selector);

    // add in our variable line tool
    tool_selector = new ToolSelector(OP_LINE_VARIABLE_THICKNESS);
    toolbar_layout->addWidget(tool_selector);

}

// destructor for the class
MainWindow::~MainWindow() {

}

// refactored function that will generate and return a hbox with no contents margins
QHBoxLayout *MainWindow::generateHBoxNoMargins() {
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    return layout;
}

// refactored out function that will generate and return a tool bar as we will use more than one
QWidget *MainWindow::generateToolbar() {
    // the toolbar we will generate
    QWidget *toolbar = new QWidget();

    // set a palette that we will use to paint the background to grey
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(128, 128, 128));

    // create a widget with a hbox layout and add it to the main view
    toolbar->setAutoFillBackground(true);
    toolbar->setPalette(pal);
    toolbar->setMaximumHeight(32);

    // return the toolbar when finished
    return toolbar;
}
