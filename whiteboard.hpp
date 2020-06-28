#ifndef _WHITEBOARD_HPP
#define _WHITEBOARD_HPP

// whiteboard.hpp
//
// header file for the whiteboard

// includes needed to make this work
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QImage>
#include <QMouseEvent>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include "drawoperations.hpp"

// class defintion
class Whiteboard : public QWidget {
    // needed to get access to the signals and slots mechanism
    Q_OBJECT

// public section of the class
public:
    // constructor for the class
    Whiteboard(QWidget* parent = 0);
    // destructor for the class
    virtual ~Whiteboard();
    // function that will add a new image in place of the current one
    void addNewImage();
    // function that will return the full list of draw images
    DrawOperations **drawOperations();
    // function that will delete the currently selected image
    void deleteImage();
    // function that will run the draw commands on a QImage and will return it
    // this is for exporting purposes
    QImage *exportBoard(const unsigned int board);
    // function that will tell us if the current image is locked
    const bool imageLocked();
    // function that will return the title of the current image
    const QString &imageTitleCurrent();
    // function that will set the lock on the current image
    void lockImage();
    // function that will return the maximum number of images in this whiteboard
    const unsigned int maxImages();
    // function that will replace the current set of images with this set of images
    void setDrawOperations(DrawOperations **operations, const unsigned int total, const unsigned int max);
    // function that will set the filename of the image to be imported on the next draw operation
    void setImportImageFilename(const QString &filename);
    // function that will reset the whiteboard to its starting conditions
    void resetWhiteBoard();
    // function that states how many images in total this whiteboard has thus far
    const unsigned int totalImages();
    // function that unlocks the current image
    void unlockImage();
// public slots of the class
public slots:
    // slot that will change the current draw colour to the indicated colour
    void changeColour(int red, int green, int blue);
    // slot that will change the current image to the given number
    void changeImage(int number);
    // slot that will change the title of the current image
    void changeImageTitle(const QString& title);
    // slot that will change the line thickness
    void changeLineThickness(int line_thickness);
    // slot that will change the point size
    void changePointSize(int point_size);
    // slot that will change the text to be displayed
    void changeText(const QString &text);
    // slot that will change the rotation of the text
    void changeTextRotation(int text_rotation);
    // slot that will change the size of the text
    void changeTextSize(int text_size);
    // slot that will change the tool to the indicated tool
    void changeTool(unsigned int tool);
// signals emitted by the class
signals:
    // signal to advance a colour
    void advanceColour();
    // signal to advance an image
    void advanceImage();
    // signal to advance a tool
    void advanceTool();
    // signal to decrease the size of the draw op
    void decreaseSize();
    // signal to increase the size of the draw op
    void increaseSize();
    // signal to go back a colour
    void goBackColour();
    // signal to go back an image
    void goBackImage();
    // signal to go back a tool
    void goBackTool();
    // signal that will be emitted to say this whiteboard was modified
    void modified();
    // signal requesting the whiteboard to be saved
    void requestSave();
    // signal requesting keyboard focus for the text of the image
    void requestTextFocus();
    // signal requesting keyboard focus for the title of the image
    void requestTitleFocus();
    // signal indicating that the whiteboard has been modified
    void whiteboardModified();
// protected methods of the class
protected:
    // overriddent mousepressevent method so we can take user drawing on screen
    void mousePressEvent(QMouseEvent* event);
    // overridden mouseMoveEvent function to do user drawing
    void mouseMoveEvent(QMouseEvent* event);
    // overridden mouseReleaseEvent
    void mouseReleaseEvent(QMouseEvent* event);
    // overridden paintevent method so we can draw on the screen
    void paintEvent(QPaintEvent* event);
// private slots section of the class
private slots:
    // slot that will emit a signal to advance the colour
    void advanceColourShortcut();
    // slot that will emit a signal to advance the image on the whiteboard
    void advanceImageShortcut();
    // slot that will emit a signal to advance the tool
    void advanceToolShortcut();
    // slot that will emit a signal to decreate the size of thickness of the current draw op
    void decreaseSizeShortcut();
    // slot that will emit a signal to go back a colour
    void goBackColourShortcut();
    // slot that will emit a signal to go back an image on the whiteboard
    void goBackImageShortcut();
    // slot that will emit a signal to go back a tool
    void goBackToolShortcut();
    // slot that will emit a signal to increase the size or thickness of the current draw op
    void increaseSizeShortcut();
    // slot for quitting the application
    void quitApplication();
    // slot that will request the application to save the whiteboard
    void requestSaveShortcut();
    // slot that will request the application to put keyboard focus on the text to be inserted
    void requestTextFocusShortcut();
    // slot that will request the application to put keyboard focus on the title of the current image
    void requestTitleFocusShortcut();
    // slot that will undo the last drawing operation
    void undoLastDrawOp();
// private fields of the class
private:
    // private function that will draw the board with the provided painter object. function
    // will assume that painter has been started before calling and will be ended after calling
    void drawBoard(QPainter &painter);
    // the current drawing colour
    QColor current_colour;
    // pen for drawing a point, and pen draw drawing lines
    QPen pen;
    // the current tool that is being used
    unsigned int tool;
    // the current line thickness and point sizes
    int current_line_thickness, current_point_size;
    // draw operations array that will hold all of the images for this whiteboard. note we set these
    // all up as pointers as we will need to shift them around
    DrawOperations **images;
    // the current image we are looking at and the maximum number of images we have
    unsigned int image_current;
    unsigned int image_max;
    // how many images does this whiteboard have in total
    unsigned int image_total;
    // coordinates for the preview before we commit to a drawing
    unsigned int preview_start_x, preview_start_y;
    unsigned int preview_end_x, preview_end_y;
    // are we in the middle of a preview draw (i.e. currently on pressed or move not released)
    bool on_preview;
    // the font that will be used for writing text to the board
    QFont font;
    // the text size, rotation and text to be displayed for the draw text operation
    int text_size;
    int text_rotation;
    QString text;
    // the name of the jpg, png, svg we are loading in
    QString image_import_filename;
    // preview image width and height
    int preview_image_width;
    int preview_image_height;
};

#endif // _WHITEBOARD_HPP
