// mainwindow.cpp
//
// implements the class defined in mainwindow.hpp

// includes
#include <cstdlib>
#include <iostream>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QSizePolicy>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include "constants.hpp"
#include "colourselector.hpp"
#include "mainwindow.hpp"
#include "toolselector.hpp"

// constructor for the class
MainWindow::MainWindow(QWidget *parent)
: QWidget(parent), filename("")
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
    QObject::connect(open_button, SIGNAL(clicked()), this, SLOT(loadImages()));

    // add in a save button for the set of images
    QPushButton *save_button = new QPushButton("Save");
    main_toolbar_layout->addWidget(save_button);
    QObject::connect(save_button, SIGNAL(clicked()), this, SLOT(saveImages()));

    // add in an export to PNG button
    QPushButton *export_png_button = new QPushButton("Export PNG");
    main_toolbar_layout->addWidget(export_png_button);
    QObject::connect(export_png_button, SIGNAL(clicked()), this, SLOT(exportPNG()));

    // add in a button for adding an image
    QPushButton *add_button = new QPushButton("Add image");
    main_toolbar_layout->addWidget(add_button);
    QObject::connect(add_button, SIGNAL(clicked()), this, SLOT(addNewImage()));

    // add in a button for deleting an image
    QPushButton *delete_button = new QPushButton("Delete image");
    main_toolbar_layout->addWidget(delete_button);

    // add in a spinbox for choosing the image number along with labels for this
    QLabel *image_number_label = new QLabel("Viewing image:");
    main_toolbar_layout->addWidget(image_number_label);
    image_selector_spinbox = new QSpinBox();
    image_selector_spinbox->setRange(1, 1);
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
    QObject::connect(image_selector_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changeImage(int)));

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

// slot that will add a new image in the current place
void MainWindow::addNewImage() {
    // get the whiteboard to add an image
    whiteboard->addNewImage();

    // get the total number of images in the whiteboard and increase the range on the spinner. increment the spinner by 1
    const unsigned int total_images = whiteboard->totalImages();
    image_selector_spinbox->setRange(1, total_images);
    image_selector_spinbox->setValue(image_selector_spinbox->value() + 1);
    // update the label with the total number of images too
    total_images_label->setText(QString("/ %1").arg(total_images));
}

// slot that will export the current whiteboard to a set of PNG images
void MainWindow::exportPNG() {
    // first get a directory from the user
    QString directory = QFileDialog::getExistingDirectory(this, "Export directory", "",  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // get the total number of images in the whiteboard and then start cycling through the images for export
    const unsigned int total_images = whiteboard->totalImages();
    for(unsigned int i = 0; i < total_images; i++) {
        // get the image for the current board
        QImage *image = whiteboard->exportBoard(i);

        // generate a filename for this board. we pad this out to three digits to
        // ensure the filenames all appear in the coorect order.
        QString filename;
        if(i + 1 < 10)
            filename = QString("/00%1.png").arg(i + 1);
        else if(i + 1 < 100)
            filename = QString("/0%1.png").arg(i + 1);
        else
            filename = QString("/%1.png").arg(i + 1);

        // generate the full path and save the file and delete the image
        image->save(directory + filename, "PNG");

    }
}

// slot that will go through the process of loading a whiteboard from disk
void MainWindow::loadImages() {
    // get the filename that we want to load. if no file is selected then don't do anything
    QString filename = QFileDialog::getOpenFileName(this, "Open Whiteboard", "", "Whiteboard Files (*.wbd)");
    if(filename.compare(QString("")) == 0)
        return;

    // open up the file for reading in binary mode
    FILE *to_read = fopen(filename.toStdString().c_str(), "rb");

    // read the total number from the file and then allocate the necessary array of pointers for the images
    unsigned int total_images = 0, max_images = 0;
    fread(&total_images, sizeof(unsigned int), 1, to_read);
    fread(&max_images, sizeof(unsigned int), 1, to_read);
    DrawOperations **loaded_images = (DrawOperations **) new DrawOperations *[max_images];

    // go through each of the images in turn and read them in
    for(unsigned int i = 0; i < total_images; i++) {
        // first read in the total ops and the max ops for this image
        unsigned int total_ops = 0, max_ops = 0;
        fread(&total_ops, sizeof(unsigned int), 1, to_read);
        fread(&max_ops, sizeof(unsigned int), 1, to_read);

        // allocate a draw operations of the required size and then read in all of the data
        loaded_images[i] = (DrawOperations *) new DrawOperations(max_ops);
        loaded_images[i]->total_ops = total_ops;
        fread(loaded_images[i]->draw_operation, sizeof(unsigned int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_x, sizeof(unsigned int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_y, sizeof(unsigned int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_red, sizeof(int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_green, sizeof(int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_blue, sizeof(int), loaded_images[i]->max_ops, to_read);
        fread(loaded_images[i]->draw_sizes, sizeof(int), loaded_images[i]->max_ops, to_read);
    }

    // close the file for reading
    fclose(to_read);

    // fill out the spare images with empty draw operations as the whiteboard will expect them to be allocated already
    for(unsigned int i = total_images; i < max_images; i++)
        loaded_images[i] = (DrawOperations *) new DrawOperations();

    // set the images on the whiteboard and reset its current image to one
    whiteboard->setDrawOperations(loaded_images, total_images, max_images);
    image_selector_spinbox->setRange(1, total_images);
    image_selector_spinbox->setValue(1);
    total_images_label->setText(QString("/ %1").arg(total_images));
}

// slot that will go through the process of saving a whiteboard to disk
void MainWindow::saveImages() {
    // if we don't have a filename then throw up a save dialog looking for one
    if(filename.compare(QString("")) == 0) {
        // throw up a file dialog to get the filename to save. if the filename is nothing then
        // the user has cancelled and we can exit early
        QString temp = QFileDialog::getSaveFileName(this, "Save Whiteboard", "", "Whiteboard Files (*.wbd)");
        if(temp.compare(QString("")) == 0)
            return;

        // we have a filename so take a copy of it
        filename = temp;
    }

    // open up the file for writing in binary mode
    FILE *to_write = fopen(filename.toStdString().c_str(), "wb");

    // get the number of images and the pointers so we can write the images
    DrawOperations **images = whiteboard->drawOperations();
    const unsigned int total_images = whiteboard->totalImages();
    const unsigned int max_images = whiteboard->maxImages();

    // save the total number of images to file
    fwrite(&total_images, sizeof(unsigned int), 1, to_write);
    fwrite(&max_images, sizeof(unsigned int), 1, to_write);

    // go through each of the images in turn
    for(unsigned int i = 0; i < total_images; i++) {
        // write the total ops and the max ops to file first
        fwrite(&images[i]->total_ops, sizeof(unsigned int), 1, to_write);
        fwrite(&images[i]->max_ops, sizeof(unsigned int), 1, to_write);

        // write all of the arrays to disk
        fwrite(images[i]->draw_operation, sizeof(unsigned int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_x, sizeof(unsigned int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_y, sizeof(unsigned int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_red, sizeof(int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_green, sizeof(int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_blue, sizeof(int), images[i]->max_ops, to_write);
        fwrite(images[i]->draw_sizes, sizeof(int), images[i]->max_ops, to_write);
    }

    // close the file when we are finished
    fclose(to_write);

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
    image_selector_spinbox->setRange(1, 1);
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
