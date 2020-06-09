#ifndef _MAINWINDOW_HPP
#define _MAINWINDOW_HPP

// mainwindow.hpp
//
// class that defines a fullscreen window that will have options for assisting with the whiteboard

// includes
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
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
// private slots of the class
private slots:
    // slot that readjust the spinners depending on the operation we have
    void enableSpinBoxes(const unsigned int op);
    // slot that will add a new image to the whiteboard immediately after the current image
    void addNewImage();
    // slot that will change the current image
    void changeImage(int number);
    // slot that will delete the current image
    void deleteImage();
    // slot that will export the current whiteboard to a set of PNG images
    void exportPNG();
    // slot that will run through the process of loading a set of images for a whiteboard
    void loadImages();
    // slot that will run through the process of saving an image
    void saveImages();
    // slot that will start a new whiteboard but will warn the user beforehand
    void startNewWhiteboard();
// private section of the class
private:
    // function that will generate and attach a colour picker to the given layout
    void generateColourSelector(QHBoxLayout *layout, int red, int green, int blue);
    // function that will generate a hboxlayout with no contents margins in it
    QHBoxLayout *generateHBoxNoMargins();
    // function that will setup a basic toolbar. this is refactored out as we have two toolbars
    QWidget *generateToolbar();
    // function that will generate and attach the given tool selector to the given layout
    void generateToolSelector(QHBoxLayout *layout, const unsigned int operation);
    // function that will ask if the user is sure that they want to delete an image. true means the image
    // is to be deleted
    bool warnDelete();
    // function that will throw up a dialog warning that the image cannot be changed unless a title is entered
    void warnNoTitle();
    // whiteboard that everything will be drawn on
    Whiteboard *whiteboard;
    // spinboxes for determining the point size and line thickness
    QSpinBox *point_size_spinbox, *line_thickness_spinbox;
    // spinbox for our image selector
    QSpinBox *image_selector_spinbox;
    // label stating how many images we have
    QLabel *total_images_label;
    // the name of the file that we are saving. if this is empty then a user has not chosen a name yet
    QString filename;
    // spinbox for selecting the size of our text
    QSpinBox *text_size_spinbox;
    // spinbox for selecting the rotation of our text
    QSpinBox *text_rotation_spinbox;
    // lineedit for chosing the text we want to write in
    QLineEdit *text_lineedit;
    // lineedit for setting the title of the image
    QLineEdit *image_title_edit;
};

#endif // _MAINWINDOW_HPP
