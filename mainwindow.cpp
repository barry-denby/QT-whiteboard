// mainwindow.cpp
//
// implements the class defined in mainwindow.hpp

// includes
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <QByteArray>
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
#include "fileops.hpp"
#include "mainwindow.hpp"
#include "toolselector.hpp"

// constructor for the class
MainWindow::MainWindow(QWidget *parent)
: QWidget(parent), filename(""), image_filename(""), load_raster(true)
{
    // allocate space for our tools and selectors
    tools = new ToolSelector *[8];
    colours = new ColourSelector *[14];

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
    save_button = new QPushButton("Save");
    main_toolbar_layout->addWidget(save_button);
    save_button->setEnabled(false);
    QObject::connect(save_button, SIGNAL(clicked()), this, SLOT(saveImages()));

    // add in an export to PNG button
    QPushButton *export_png_button = new QPushButton("Export PNG");
    main_toolbar_layout->addWidget(export_png_button);
    QObject::connect(export_png_button, SIGNAL(clicked()), this, SLOT(exportPNG()));

    // add in a button to lock and unlock the undo for the current image
    lock_button = new QPushButton("Lock");
    main_toolbar_layout->addWidget(lock_button);
    QObject::connect(lock_button, SIGNAL(clicked()), this, SLOT(lockUnlockImage()));

    // add in a button for adding an image
    QPushButton *add_button = new QPushButton("Add image");
    main_toolbar_layout->addWidget(add_button);
    QObject::connect(add_button, SIGNAL(clicked()), this, SLOT(addNewImage()));

    // add in a button for deleting an image
    QPushButton *delete_button = new QPushButton("Delete image");
    main_toolbar_layout->addWidget(delete_button);
    QObject::connect(delete_button, SIGNAL(clicked()), this, SLOT(deleteImage()));

    // add in a pushbutton for loading an image
    load_image_pushbutton = new QPushButton("Load PNG/JPG/SVG");
    load_image_pushbutton->setEnabled(false);
    main_toolbar_layout->addWidget(load_image_pushbutton);
    QObject::connect(load_image_pushbutton, SIGNAL(clicked()), this, SLOT(loadJpgPngSvgImage()));

    // add in a spinbox for choosing the image number along with labels for this
    QLabel *image_number_label = new QLabel("Viewing image:");
    main_toolbar_layout->addWidget(image_number_label);
    image_selector_spinbox = new QSpinBox();
    image_selector_spinbox->setRange(1, 1);
    main_toolbar_layout->addWidget(image_selector_spinbox);
    total_images_label = new QLabel("/ 1");
    main_toolbar_layout->addWidget(total_images_label);
    QObject::connect(image_selector_spinbox, SIGNAL(valueChanged(int)), this, SLOT(changeImage(int)));

    // add in a label for the image title and a line edit for changing that image title
    QLabel *image_title_label = new QLabel("image Title:");
    main_toolbar_layout->addWidget(image_title_label);
    image_title_edit = new QLineEdit("Placeholder title");
    image_title_edit->setMaxLength(1023);
    main_toolbar_layout->addWidget(image_title_edit);
    QObject::connect(image_title_edit, SIGNAL(textEdited(const QString &)), this, SLOT(titleChanged(const QString &)));

    // add a whiteboard to the layout and drop all of the margins
    whiteboard = new Whiteboard();
    whiteboard_container_layout->addWidget(whiteboard);
    whiteboard_container_layout->setContentsMargins(0, 0, 0, 0);
    QObject::connect(whiteboard, SIGNAL(advanceColour()), this, SLOT(advanceColour()));
    QObject::connect(whiteboard, SIGNAL(advanceImage()), this, SLOT(advanceImage()));
    QObject::connect(whiteboard, SIGNAL(advanceTool()), this, SLOT(advanceTool()));
    QObject::connect(whiteboard, SIGNAL(goBackColour()), this, SLOT(goBackColour()));
    QObject::connect(whiteboard, SIGNAL(goBackImage()), this, SLOT(goBackImage()));
    QObject::connect(whiteboard, SIGNAL(goBackTool()), this, SLOT(goBackTool()));
    QObject::connect(whiteboard, SIGNAL(increaseSize()), this, SLOT(increaseDrawSize()));
    QObject::connect(whiteboard, SIGNAL(decreaseSize()), this, SLOT(decreaseDrawSize()));
    QObject::connect(whiteboard, SIGNAL(requestSave()), this, SLOT(whiteboardSave()));
    QObject::connect(whiteboard, SIGNAL(modified()), this, SLOT(boardModified()));
    QObject::connect(whiteboard, SIGNAL(requestTitleFocus()), this, SLOT(titleKeyboardFocus()));
    QObject::connect(whiteboard, SIGNAL(requestTextFocus()), this, SLOT(textKeyboardFocus()));
    QObject::connect(whiteboard, SIGNAL(requestRotateLeft()), this, SLOT(rotateLeft()));
    QObject::connect(whiteboard, SIGNAL(requestRotateRight()), this, SLOT(rotateRight()));

    // as the whiteboard is now defined set the title on the first image and connect a signal from the line
    // edit to change the text on the current image
    whiteboard->changeImageTitle(QString("Placeholder title"));
    QObject::connect(image_title_edit, SIGNAL(textEdited(const QString&)), whiteboard, SLOT(changeImageTitle(const QString&)));

    // create a widget with a hbox layout and add it to the main view
    QWidget *toolbar = generateToolbar();
    QHBoxLayout *toolbar_layout = generateHBoxNoMargins();
    toolbar->setLayout(toolbar_layout);
    whiteboard_container_layout->addWidget(toolbar);

    // add in colour selectors to the bottom bar
    colours[0] = generateColourSelector(toolbar_layout, 0, 0, 0);
    colours[1] = generateColourSelector(toolbar_layout, 128, 0, 0);
    colours[2] = generateColourSelector(toolbar_layout, 255, 0, 0);
    colours[3] = generateColourSelector(toolbar_layout, 0, 128, 0);
    colours[4] = generateColourSelector(toolbar_layout, 0, 255, 0);
    colours[5] = generateColourSelector(toolbar_layout, 0, 0, 128);
    colours[6] = generateColourSelector(toolbar_layout, 0, 0, 255);
    colours[7] = generateColourSelector(toolbar_layout, 128, 128, 0);
    colours[8] = generateColourSelector(toolbar_layout, 255, 255, 0);
    colours[9] = generateColourSelector(toolbar_layout, 128, 0, 128);
    colours[10] = generateColourSelector(toolbar_layout, 255, 0, 255);
    colours[11] = generateColourSelector(toolbar_layout, 0, 128, 128);
    colours[12] = generateColourSelector(toolbar_layout, 0, 255, 255);
    colours[13] = generateColourSelector(toolbar_layout, 255, 255, 255);
    colours[0]->setSelected(true);

    // add in a label for tools
    QLabel *tools_label = new QLabel("Tools:");
    toolbar_layout->addWidget(tools_label);

    // add in all of our tools and set selected on the point square
    tools[0] = generateToolSelector(toolbar_layout, OP_POINT_CIRCLE);
    tools[1] = generateToolSelector(toolbar_layout, OP_POINT_SQUARE);
    tools[2] = generateToolSelector(toolbar_layout, OP_POINT_X);
    tools[3] = generateToolSelector(toolbar_layout, OP_LINE_FREEFORM);
    tools[4] = generateToolSelector(toolbar_layout, OP_LINE_STRAIGHT);
    tools[5] = generateToolSelector(toolbar_layout, OP_DRAW_TEXT);
    tools[6] = generateToolSelector(toolbar_layout, OP_DRAW_RASTER);
    tools[7] = generateToolSelector(toolbar_layout, OP_DRAW_SVG);
    tools[1]->setSelected(true);

    // add in a label and spinbox for the point size. connect tthe spinbox to the
    // appropriate method in the whiteboard
    QLabel *point_size_label = new QLabel("Point Size:");
    toolbar_layout->addWidget(point_size_label);
    point_size_spinbox = new QSpinBox();
    point_size_spinbox->setRange(1, 100);
    point_size_spinbox->setValue(6);
    toolbar_layout->addWidget(point_size_spinbox);
    QObject::connect(point_size_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changePointSize(int)));

    // add in a label and spin box for the line thickness. disable this to begin
    // with as we will default to a point operation. connect the spinbox to the
    // appropriate method in the whiteboard
    QLabel *line_thickness_label = new QLabel("Line Thickness:");
    toolbar_layout->addWidget(line_thickness_label);
    line_thickness_spinbox = new QSpinBox();
    line_thickness_spinbox->setRange(1, 100);
    line_thickness_spinbox->setValue(2);
    line_thickness_spinbox->setEnabled(false);
    toolbar_layout->addWidget(line_thickness_spinbox);
    QObject::connect(line_thickness_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changeLineThickness(int)));

    // add in a label and a spinbox for the text size
    QLabel *text_size_label = new QLabel("Text Size:");
    toolbar_layout->addWidget(text_size_label);
    text_size_spinbox = new QSpinBox();
    text_size_spinbox->setRange(8, 56);
    text_size_spinbox->setValue(20);
    text_size_spinbox->setEnabled(false);
    toolbar_layout->addWidget(text_size_spinbox);
    QObject::connect(text_size_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changeTextSize(int)));

    // add in a label and a spinbox for the text rotation
    QLabel *text_rotation_label = new QLabel("Text Rotation:");
    toolbar_layout->addWidget(text_rotation_label);
    text_rotation_spinbox = new QSpinBox();
    text_rotation_spinbox->setRange(0, 359);
    text_rotation_spinbox->setValue(0);
    text_rotation_spinbox->setEnabled(false);
    toolbar_layout->addWidget(text_rotation_spinbox);
    QObject::connect(text_rotation_spinbox, SIGNAL(valueChanged(int)), whiteboard, SLOT(changeTextRotation(int)));

    // add in a label and a line edit for the text itself
    QLabel *text_label = new QLabel("Text:");
    toolbar_layout->addWidget(text_label);
    text_lineedit = new QLineEdit("Placeholder text to draw");
    text_lineedit->setMaxLength(1023);
    text_lineedit->setEnabled(false);
    text_lineedit->setPlaceholderText(QString("Enter text to draw here."));
    toolbar_layout->addWidget(text_lineedit);
    QObject::connect(text_lineedit, SIGNAL(textEdited(const QString &)), whiteboard, SLOT(changeText(const QString &)));

}

// destructor for the class
MainWindow::~MainWindow() {
    delete tools;
    delete colours;
}

// slot that readjust the spinners depending on the operation we have
void MainWindow::enableSpinBoxes(const unsigned int op) {
    // adjust the spinboxed based on the operation
    if(op == OP_POINT_SQUARE || op == OP_POINT_CIRCLE || op == OP_POINT_X) {
        // enable the point size and disable everything else
        point_size_spinbox->setEnabled(true);
        line_thickness_spinbox->setEnabled(false);
        text_size_spinbox->setEnabled(false);
        text_rotation_spinbox->setEnabled(false);
        text_lineedit->setEnabled(false);
    } else if(op == OP_LINE_FREEFORM || op == OP_LINE_STRAIGHT) {
        // enable the line thickness and disable everything else
        point_size_spinbox->setEnabled(false);
        line_thickness_spinbox->setEnabled(true);
        text_size_spinbox->setEnabled(false);
        text_rotation_spinbox->setEnabled(false);
        text_lineedit->setEnabled(false);
    } else if(op == OP_DRAW_TEXT) {
        // enable the text size, rotation, and line edit, disable everything else
        point_size_spinbox->setEnabled(false);
        line_thickness_spinbox->setEnabled(false);
        text_size_spinbox->setEnabled(true);
        text_rotation_spinbox->setEnabled(true);
        text_lineedit->setEnabled(true);
    } else if(op == OP_DRAW_RASTER || op == OP_DRAW_SVG) {
        // enable the text size, rotation, and line edit, disable everything else
        point_size_spinbox->setEnabled(false);
        line_thickness_spinbox->setEnabled(false);
        text_size_spinbox->setEnabled(false);
        text_rotation_spinbox->setEnabled(false);
        text_lineedit->setEnabled(false);
    }
}

// slot that will advance a colour
void MainWindow::advanceColour() {
    // determine the current tool that is selected
    unsigned int index = 0;
    for(index = 0; index < 14; index++) {
        if(colours[index]->selected())
            break;
    }

    // if the index is less than 13 then deselect the current tool and advance the current one
    if(index < 13) {
        colours[index]->setSelected(false);
        colours[index + 1]->setSelected(true);
        QColor colour = colours[index + 1]->colour();
        whiteboard->changeColour(colour.red(), colour.green(), colour.blue());
    }
}

// slot that will advance an image
void MainWindow::advanceImage() {
    // if there is no image to advance then just return immediately
    if((unsigned int) image_selector_spinbox->value() == whiteboard->totalImages())
        return;

    // take the current spinner value and advance it on by one.
    image_selector_spinbox->setValue(image_selector_spinbox->value() + 1);
}

// slot that will advance a tool
void MainWindow::advanceTool() {
    // determine the current tool that is selected
    unsigned int index = 0;
    for(index = 0; index < 8; index++) {
        if(tools[index]->selected())
            break;
    }

    // if the index is less than 7 then deselect the current tool and advance the current one
    if(index < 7) {
        tools[index]->setSelected(false);
        tools[index + 1]->setSelected(true);
        whiteboard->changeTool(tools[index + 1]->tool());

        // update the spinboxes to reflect the new tool
        enableSpinBoxes(tools[index + 1]->tool());
    }

    // if the index is one of the image tools then enable the load image button and vice versa
    if(index + 1 == 6 || index + 1 == 7) {
        load_image_pushbutton->setEnabled(true);
        whiteboard->setImportImageFilename(QString(""));
    } else {
        load_image_pushbutton->setEnabled(false);
        whiteboard->setImportImageFilename(QString(""));
    }

    // if the index is 6 or 7 change to either a raster or svg image
    if(index + 1 == 6)
        load_raster = true;
    else if(index + 1 == 7)
        load_raster = false;

}

// slot that will add a new image in the current place
void MainWindow::addNewImage() {
    // if the current image has no title then do nothing
    if(QString::compare(image_title_edit->text(), QString(""))== 0) {
        warnNoTitle();
        return;
    }

    // take a copy of the current title
    QString title = image_title_edit->text();

    // get the whiteboard to add an image
    whiteboard->addNewImage();

    // get the total number of images in the whiteboard and increase the range on the spinner. increment the spinner by 1
    const unsigned int total_images = whiteboard->totalImages();
    image_selector_spinbox->setRange(1, total_images);
    image_selector_spinbox->setValue(image_selector_spinbox->value() + 1);
    // update the label with the total number of images too
    total_images_label->setText(QString("/ %1").arg(total_images));

    // set the title on the new image
    whiteboard->changeImageTitle(title);
    image_title_edit->setText(title);

    // enable the save button
    save_button->setEnabled(true);
}

// slot that will react by changing the UI when the board has been modified
void MainWindow::boardModified() {
    // enable the save button
    save_button->setEnabled(true);
}

// slot that will update the colour selectors in response to a tool being changed
void MainWindow::changeColour(int red, int green, int blue) {
    // go through all the colours and disable their selected state
    for(unsigned int i = 0; i < 14; i++) {
        colours[i]->setSelected(false);
    }
}

// slot that will change the current image
void MainWindow::changeImage(int number) {
    // if the current image has no title then do nothing
    if(QString::compare(image_title_edit->text(), QString(""))== 0) {
        warnNoTitle();
        return;
    }

    // call the change image on the whiteboard itself and update the title to reflect the change and update the lock button
    whiteboard->changeImage(number);
    image_title_edit->setText(whiteboard->imageTitleCurrent());
    updateLockButton();
}

// slot that will update the tools in response to a tool being changed
void MainWindow::changeTools(unsigned int tool) {
    // go through all of the tools and disable their selection
    for(unsigned int i = 0; i < 8; i++) {
        tools[i]->setSelected(false);
    }

    // if the tool is one of the IMG or SVG tools then enable the load image button
    if(tool == OP_DRAW_RASTER || tool == OP_DRAW_SVG) {
        load_image_pushbutton->setEnabled(true);
        whiteboard->setImportImageFilename(QString(""));
    } else {
        load_image_pushbutton->setEnabled(false);
        whiteboard->setImportImageFilename(QString(""));
    }

    // if the tool is raster or the svg set that type to load
    if(tool == OP_DRAW_RASTER)
        load_raster = true;
    else if(tool == OP_DRAW_SVG)
        load_raster = false;
}

// slot for decreasing the draw size
void MainWindow::decreaseDrawSize() {
    // check which draw operation we have and change the appropriate spinbox
    if(tools[0]->selected() || tools[1]->selected() || tools[2]->selected())
        // this is one of the point tools so change the point size
        point_size_spinbox->setValue(point_size_spinbox->value() - 1);
    else if(tools[3]->selected() || tools[4]->selected())
        // this is one of the line tools so change the line thickness
        line_thickness_spinbox->setValue(line_thickness_spinbox->value() - 1);
    else if(tools[5]->selected())
        // this is the text tool so decrease the text size
        text_size_spinbox->setValue(text_size_spinbox->value() - 1);
}

// slot that will delete the current image
void MainWindow::deleteImage() {
    // if we only have the one image then do nothing
    if(whiteboard->totalImages() == 1 || !warnDelete())
        return;

    // get the whiteboard to delete the current image and update the UI to show we are an image down
    whiteboard->deleteImage();
    image_selector_spinbox->setValue(image_selector_spinbox->value() - 1);
    image_selector_spinbox->setRange(1, whiteboard->totalImages());
    total_images_label->setText(QString("/ %1").arg(whiteboard->totalImages()));

    // set the title field to the title of the current image
    image_title_edit->setText(whiteboard->imageTitleCurrent());

    // enable the save button as the board as been modified
    save_button->setEnabled(true);
}

// slot that will go back a colour
void MainWindow::goBackColour() {
    // determine the current colour that is selected
    unsigned int index = 0;
    for(index = 0; index < 14; index++) {
        if(colours[index]->selected())
            break;
    }

    // if the index is less than 13 then deselect the current tool and advance the current one and set the colour on the whiteboard
    if(index > 0) {
        colours[index]->setSelected(false);
        colours[index - 1]->setSelected(true);
        QColor colour = colours[index - 1]->colour();
        whiteboard->changeColour(colour.red(), colour.green(), colour.blue());
    }
}

// slot that will go back an image
void MainWindow::goBackImage() {
    // if there is no image to go back then just return immediately
    if(image_selector_spinbox->value() == 1)
        return;

    // take the current spinner value and advance it on by one.
    image_selector_spinbox->setValue(image_selector_spinbox->value() - 1);
}

// slot that will go back a tool
void MainWindow::goBackTool() {
    // determine the current tool that is selected
    unsigned int index = 0;
    for(index = 0; index < 8; index++) {
        if(tools[index]->selected())
            break;
    }

    // if the index is less than 13 then deselect the current tool and advance the current one
    if(index > 0) {
        tools[index]->setSelected(false);
        tools[index - 1]->setSelected(true);
        whiteboard->changeTool(tools[index - 1]->tool());

        // update the spinboxes to reflect the new tool
        enableSpinBoxes(tools[index - 1]->tool());
    }

    // if the index is one of the image tools then enable the load image button and vice versa
    if(index - 1 == 6 || index - 1 == 7) {
        load_image_pushbutton->setEnabled(true);
        whiteboard->setImportImageFilename(QString(""));
    } else {
        load_image_pushbutton->setEnabled(false);
        whiteboard->setImportImageFilename(QString(""));
    }

    // if the index is 6 or 7 change to either a raster or svg image
    if(index + 1 == 6)
        load_raster = true;
    else if(index + 1 == 7)
        load_raster = false;
}

// slot for increasing the draw size
void MainWindow::increaseDrawSize() {
    // check which draw operation we have and change the appropriate spinbox
    if(tools[0]->selected() || tools[1]->selected() || tools[2]->selected())
        // this is one of the point tools so change the point size
        point_size_spinbox->setValue(point_size_spinbox->value() + 1);
    else if(tools[3]->selected() || tools[4]->selected())
        // this is one of the line tools so change the line thickness
        line_thickness_spinbox->setValue(line_thickness_spinbox->value() + 1);
    else if(tools[5]->selected())
        // this is the text tool so increase the text size
        text_size_spinbox->setValue(text_size_spinbox->value() + 1);
}

// slot that will export the current whiteboard to a set of PNG images
void MainWindow::exportPNG() {
    // if the current image has no title then do nothing
    if(QString::compare(image_title_edit->text(), QString(""))== 0) {
        warnNoTitle();
        return;
    }

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

    // read in the whiteboard
    unsigned int total_images = 0;
    unsigned int max_images = 0;
    DrawOperations **board = loadWhiteboard(filename, &total_images, &max_images);

    // set the images on the whiteboard and reset its current image to one
    whiteboard->setDrawOperations(board, total_images, max_images);
    image_selector_spinbox->setRange(1, total_images);
    image_selector_spinbox->setValue(1);
    total_images_label->setText(QString("/ %1").arg(total_images));
    image_title_edit->setText(board[0]->title);

    // as there is no modification at this point disable the save button
    save_button->setEnabled(false);

    // update the lock button to reflect the status of the first image
    updateLockButton();

}

// slot that will load an image for drawing onto the board
void MainWindow::loadJpgPngSvgImage() {
    // throw up a different dialog box depending on whether we are loading a raster image or an SVG
    QString filename;
    if(load_raster) {
        // get the filename that we want to load. if no file is selected then don't do anything
        filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Raster image Files (*.jpg, *.png)");
    } else {
        // get the filename that we want to load. if no file is selected then don't do anything
        filename = QFileDialog::getOpenFileName(this, "Open Image", "", "Vector image Files (*.svg)");
    }

    // if we have no filename then return immediately
    if(filename.compare(QString("")) == 0)
        return;

    // set the filename on the whiteboard
    whiteboard->setImportImageFilename(filename);
}

// slot that will lock and unlock the current image
void MainWindow::lockUnlockImage() {
    // see what is currently written as the button title and react to that
    if(QString::compare(QString("Lock"), lock_button->text()) == 0) {
        // the user is asking us to lock the image at the current ops
        whiteboard->lockImage();

        // change the title of the button when finished
        lock_button->setText("Unlock");
    } else {
        // the user is asking us to unlock the image
        whiteboard->unlockImage();

        // change the title of the button when finished
        lock_button->setText("Lock");
    }
}

// slot that will decrease rotation to the left by 45 degrees
void MainWindow::rotateLeft() {
    // check if the rotation is enabled
    if(text_rotation_spinbox->isEnabled()) {
        // pull the current value and subtract 45 from it. if this value is less than or equal 0 then add 360 to it
        int new_rotation = text_rotation_spinbox->value() - 45;
        if(new_rotation < 0)
            new_rotation += 360;

        // set the new value
        text_rotation_spinbox->setValue(new_rotation);
    }
}

// slot that will increase the rotation to the right by 45 degrees
void MainWindow::rotateRight() {
    // check if the rotation is enabled
    if(text_rotation_spinbox->isEnabled()) {
        // pull the current value and add 45 from it. if this value is greater than or equal 360 then subract 360 from it
        int new_rotation = text_rotation_spinbox->value() + 45;
        if(new_rotation >= 360)
            new_rotation -= 360;

        // set the new value
        text_rotation_spinbox->setValue(new_rotation);
    }
}

// slot that will go through the process of saving a whiteboard to disk
void MainWindow::saveImages() {
    // if the save button is disabled then there has been no modification so do nothing
    if(save_button->isEnabled() == false)
        return;

    // if the current image has no title then do nothing
    if(QString::compare(image_title_edit->text(), QString(""))== 0) {
        warnNoTitle();
        return;
    }

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

    // save the whiteboard to disk
    saveWhiteboard(filename, whiteboard->drawOperations(), whiteboard->totalImages());

    // disable the save button as we are now in a non modified state
    save_button->setEnabled(false);

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

    // set the filename to nothing
    filename = QString("");

    // ask the whiteboard to reset itself. then reset the UI to reflect this
    whiteboard->resetWhiteBoard();
    image_selector_spinbox->setRange(1, 1);
    total_images_label->setText("/ 1");
    image_title_edit->setText("Placeholder title");
    whiteboard->changeImageTitle(QString("Placeholder title"));
    updateLockButton();
    save_button->setEnabled(false);
}

// slot that will put keyboard focus on the text to be inserted
void MainWindow::textKeyboardFocus() {
    // application specific behaviour is the reason for the focus change
    text_lineedit->setFocus(Qt::OtherFocusReason);
}

// slot that will enable the save button when the title on an image has been changed
void MainWindow::titleChanged(const QString &text) {
    save_button->setEnabled(true);
}

// slot that will put keyboard focus on the image title when requested
void MainWindow::titleKeyboardFocus() {
    // we use other focus reason here as this is an application shortcut that is triggering the change in focus
    image_title_edit->setFocus(Qt::OtherFocusReason);
}

// slot that will save the current whiteboard
void MainWindow::whiteboardSave() {
    saveImages();
}

// refactored function that will generate and return a colour sleector with the given colour
// this will also set up the appropriate signals
ColourSelector *MainWindow::generateColourSelector(QHBoxLayout *layout, int red, int green, int blue) {
    ColourSelector *temp = new ColourSelector(red, green, blue);
    layout->addWidget(temp);
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), whiteboard, SLOT(changeColour(int, int, int)));
    QObject::connect(temp, SIGNAL(clicked(int, int, int)), this, SLOT(changeColour(int, int, int)));

    // return the colour selector
    return temp;
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
ToolSelector *MainWindow::generateToolSelector(QHBoxLayout *layout, const unsigned int operation) {
    // generate the tool and add it to the layout and connet the clicked signal to the whiteboard
    // and to the spinbox adjuster here
    ToolSelector *tool_selector = new ToolSelector(operation);
    layout->addWidget(tool_selector);
    QObject::connect(tool_selector, SIGNAL(clicked(unsigned int)), whiteboard, SLOT(changeTool(unsigned int)));
    QObject::connect(tool_selector, SIGNAL(clicked(unsigned int)), this, SLOT(enableSpinBoxes(unsigned int)));
    QObject::connect(tool_selector, SIGNAL(clicked(unsigned int)), this, SLOT(changeTools(unsigned int)));

    // return the tool selector
    return tool_selector;
}

// refactored function that will update the text on the lock button depending on the lock state of the current image
void MainWindow::updateLockButton() {
    // check to see if the current image is locked or not and update the lock button to reflect this
    if(whiteboard->imageLocked())
        lock_button->setText("Unlock");
    else
        lock_button->setText("Lock");
}

// function that will ask if the user is sure that they want to delete an image. true means the image
// is to be deleted
bool MainWindow::warnDelete() {
    // throw up a message box asking if they want to delete this image
    QMessageBox start_messagebox;
    start_messagebox.setText("Are you sure you want to delete this image? image will be lost");
    start_messagebox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    start_messagebox.setDefaultButton(QMessageBox::No);
    int result = start_messagebox.exec();
    if(result == QMessageBox::Yes)
        return true;
    else
        return false;
}

// function that will throw up a dialog warning that the image cannot be changed unless a title is entered
void MainWindow::warnNoTitle() {
    // emit a message stating that we cannot change unless a title is entered
    QMessageBox warning;
    warning.setText("Image has no title. Will not save/change/add/export until there is a title");
    warning.exec();
}
