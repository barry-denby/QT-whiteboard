#ifndef _WHITEBOARD_HPP
#define _WHITEBOARD_HPP

// whiteboard.hpp
//
// header file for the whiteboard

// includes needed to make this work
#include <QImage>
#include <QWidget>

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
// protected methods of the class
protected:
    // overridden paintevent method so we can draw on the screen
    void paintEvent(QPaintEvent* event);
// private slots section of the class
private slots:
    // slot for quitting the application
    void quitApplication();
// private fields of the class
private:
    // the image that will contain all of our data and will be rendered to screen
    QImage current_screen;
};

#endif // _WHITEBOARD_HPP
