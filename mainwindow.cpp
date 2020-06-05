// mainwindow.cpp
//
// implements the class defined in mainwindow.hpp

// includes
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
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

    // create a widget with a hbox layout and add it to the main view. this will serve as our
    // bar for naming and saving.
    QWidget *main_toolbar = generateToolbar();
    QHBoxLayout *main_toolbar_layout = generateHBoxNoMargins();
    main_toolbar->setLayout(main_toolbar_layout);
    whiteboard_container_layout->addWidget(main_toolbar);

    // add in a button for a new set of images to draw and connect it to the appropriate slot
    QPushButton *new_button = new QPushButton("New");
    main_toolbar_layout->addWidget(new_button);
    QObject::connect(new_button, SIGNAL(clicked()), this, SLOT(startNewWhiteboard()));

    // add in a button for opening a set of draw operations
    QPushButton *open_button = new QPushButton("Open");
    main_toolbar_layout->addWidget(open_button);

    // add in a save button for the set of images
    QPushButton *save_button = new QPushButton("Save");
    main_toolbar_layout->addWidget(save_button);

    // add in an export to PNG button
    QPushButton *export_png_button = new QPushButton("Export PNG");
    main_toolbar_layout->addWidget(export_png_button);

    // add in a button for adding an image
    QPushButton *add_button = new QPushButton("Add image");
    main_toolbar_layout->addWidget(add_button);

    // add in a button for deleting an image
    QPushButton *delete_button = new QPushButton("Delete image");
    main_toolbar_layout->addWidget(delete_button);

    // add in a spinbox for choosing the image number along with labels for this
    QLabel *image_number_label = new QLabel("Viewing image:");
    main_toolbar_layout->addWidget(image_number_label);
    image_selector_spinbox = new QSpinBox();
    image_selector_spinbox->setValue(1);
    main_toolbar_layout->addWidget(image_selector_spinbox);
    total_images_label = new QLabel("/ 1");
    main_toolbar_layout->addWidget(total_images_label);

    // add in a label for the image title and a line edit for changing that image title
    QLabel *image_title_label = new QLabel("image Title:");
    main_toolbar_layout->addWidget(image_title_label);
    QLineEdit *image_title_edit = new QLineEdit();
    main_toolbar_layout->addWidget(image_title_edit);

    // add a whiteboard to the layout and drop all of the margins
    whiteboard = new Whiteboard();
    whiteboard_container_layout->addWidget(whiteboard);
    whiteboard_container_layout->setContentsMargins(0, 0, 0, 0);

    // create a widget with a hbox layout and add it to the main view
    QWidget *toolbar = generateToolbar();
    QHBoxLayout *toolbar_layout = generateHBoxNoMargins();
    toolbar->setLayout(toolbar_layout);
    whiteboard_container_layout->addWidget(toolbar);

    // add in colour selectors to the bottom bar
    generateColourSelector(toolbar_layout, 0, 0, 0);
    generateColourSelector(toolbar_layout, 128, 0, 0);
    generateColourSelector(toolbar_layout, 255, 0, 0);
    generateColourSelector(toolbar_layout, 0, 128, 0);
    generateColourSelector(toolbar_layout, 0, 255, 0);
    generateColourSelector(toolbar_layout, 0, 0, 128);
    generateColourSelector(toolbar_layout, 0, 0, 255);
    generateColourSelector(toolbar_layout, 128, 128, 0);
    generateColourSelector(toolbar_layout, 255, 255, 0);
    generateColourSelector(toolbar_layout, 128, 0, 128);
    generateColourSelector(toolbar_layout, 255, 0, 255);
    generateColourSelector(toolbar_layout, 0, 128, 128);
    generateColourSelector(toolbar_layout, 0, 255, 255);
    generateColourSelector(toolbar_layout, 255, 255, 255);

    // add in a label and spinbox for the point size. connect tthe spinbox to the
    // appropriate method in the whiteboard
    QLabel *point_size_label = new QLabel("Point Size:");
    toolbar_layout->addWidget(point_size_label);
    point_size_spinbox = new QSpinBox();
    point_size_spinbox->setRange(1, 100);
    point_size_spinbox->setValue(3);
    toolbar_layout->addWidget(point_size_spinbox);
    QObject::connect(point_size_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changePointSize(int)));

    // add in a label and spin box for the line thickness. disable this to begin
    // with as we will default to a point operation. connect the spinbox to the
    // appropriate method in the whiteboard
    QLabel *line_thickness_label = new QLabel("Line Thickness:");
    toolbar_layout->addWidget(line_thickness_label);
    line_thickness_spinbox = new QSpinBox();
    line_thickness_spinbox->setRange(1, 100);
    line_thickness_spinbox->setValue(1);
    line_thickness_spinbox->setEnabled(false);
    toolbar_layout->addWidget(line_thickness_spinbox);
    QObject::connect(line_thickness_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changeLineThickness(int)));

    // add in a label for tools
    QLabel *tools_label = new QLabel("Tools:");
    toolbar_layout->addWidget(tools_label);

    // add in all of our tools
    //generateToolSelector(toolbar_layout, OP_POINT_FIXED_SIZE);
    generateToolSelector(toolbar_layout, OP_POINT_VARIABLE_SIZE);
    //generateToolSelector(toolbar_layout, OP_LINE_FIXED_THICKNESS);
    generateToolSelector(toolbar_layout, OP_LINE_VARIABLE_THICKNESS);

}

// destructor for the class
MainWindow::~MainWindow() {

}

// slot that readjust the spinners depending on the operation we have
void MainWindow::enableSpinBoxes(const unsigned int op) {
    // adjust the spinboxed based on the operation
    if(op == OP_POINT_VARIABLE_SIZE) {
        // enable the point and disable the line
        point_size_spinbox->setEnabled(true);
        line_thickness_spinbox->setEnabled(false);
    } else if(op == OP_LINE_VARIABLE_THICKNESS) {
        // enable the point and disable the line
        point_size_spinbox->setEnabled(false);
        line_thickness_spinbox->setEnabled(true);
    }
}

// slot that will start a new whiteboard and reset everything
void MainWindow::startNewWhiteboard() {
    // first we will need to put up a dialog asking if this is what the user wants to do
    // we are not going to check in advance if this is empty or new
    // first prompt the user to see if this is what they really want to do
    QMessageBox start_messagebox;
    start_messagebox.setText("Are you sure you want to start a new whiteboard? Previous whiteboard will be lost");
    start_messagebox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    start_messagebox.setDefaultButton(QMessageBox::No);
    int result = start_messagebox.exec();

    // if the user declined then exit immediately.
    if(result == QMessageBox::No)
        return;

    // ask the whiteboard to reset itself. then reset the UI to reflect this
    whiteboard->resetWhiteBoard();
    image_selector_spinbox->setValue(1);
    total_images_label->setText("/ 1");
}

// refactored function that will generate and return a colour sleector with the given colour
// this will also set up the appropriate signals
void MainWindow::generateColourSelector(QHBoxLayout *layout, int red, int green, int blue) {
    ColourSelector *temp = new ColourSelector(red, green, blue);
    layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
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

// refactored method that will generate the given tool selector, add it to the layout, and setup the
// appropriate signals
void MainWindow::generateToolSelector(QHBoxLayout *layout, const unsigned int operation) {
    // generate the tool and add it to the layout and connet the clicked signal to the whiteboard
    // and to the spinbox adjuster here
    ToolSelector *tool_selector = new ToolSelector(operation);
    layout->addWidget(tool_selector);
    QObject::connect(tool_selector, SIGNAL(clicked(unsigned int)), whiteboard, SLOT(changeTool(unsigned int)));
    QObject::connect(tool_selector, SIGNAL(clicked(unsigned int)), this, SLOT(enableSpinBoxes(unsigned int)));
}
