#ifndef _TOOLSELECTOR_HPP
#define _TOOLSELECTOR_HPP

// toolselector.hpp
//
// defines a template that will contain all of the tools that can be selected and their visual represntations.
// if a user clicks on a tool the emitted signal should be sent to the whiteboard itself to reflect this.

// includes
#include <QBrush>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWidget>

// class definition
class ToolSelector : public QWidget {
    // needed to get access to the signals and slots mechanism
    Q_OBJECT
// public section of the class
public:
    // construtor for the class that will take in a tool constant and will generate the appropriate view
    // for that tool
    ToolSelector(const unsigned int tool, QWidget *parent = 0);
    // destructor for the class
    ~ToolSelector();
// signals that can be emitted by the class
signals:
    // when selected the tool will emit which operation it is
    void clicked(const unsigned int operation);
// protected section of the class
protected:
    // overridden mouse release event that will emit the clicked signal for the operation
    void mouseReleaseEvent(QMouseEvent *event);
    // overridden version of the paint event that will draw something different depending
    // on the tool constant
    void paintEvent(QPaintEvent *event);
// private section of the class
private:
    // function that will draw the simple line tool
    void drawLineTool(QPainter &painter);
    // function that will draw the variable line tool
    void drawLineVariableTool(QPainter &painter);
    // function that will draw the simple point tool
    void drawPointTool(QPainter &painter);
    // function that will draw the variable point tool
    void drawPointVariableTool(QPainter &painter);
    // the tool that this selector represents
    const unsigned int tool;
    // colours that we will use for painting
    const QColor black, white;
    // pen and brush that we will use for drawing
    QPen pen;
    QBrush brush;
};

#endif // _TOOLSELECTOR_HPP
