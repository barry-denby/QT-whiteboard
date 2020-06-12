// toolselector.cpp
//
// implements everything described in toolselector.hpp

// includes
#include <iostream>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include "constants.hpp"
#include "toolselector.hpp"

// constructor for the class that takes in the tool type
ToolSelector::ToolSelector(const unsigned int tool, QWidget *parent)
: QWidget(parent), _tool(tool), black(QColor(0, 0, 0)), white(QColor(255, 255, 255)), pen(black), brush(black), _selected(false)
{
    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
    this->setMinimumWidth(32);
    this->setMinimumHeight(32);

}

// destructor for the class
ToolSelector::~ToolSelector() {

}

// returns the selected state for this tool
const bool ToolSelector::selected() {
    return _selected;
}

// sets the selection state for this tool
void ToolSelector::setSelected(const bool selected) {
    _selected = selected;
    repaint();
}

// states what this tool is
const unsigned int ToolSelector::tool() {
    return _tool;
}

// function that will emit the clicked signal when the mouse button is released
void ToolSelector::mouseReleaseEvent(QMouseEvent *event) {
    // emit a clicked event, set this selected and repaint
    emit clicked(_tool);
    _selected = true;
    repaint();
}

// function that will paint the widget
void ToolSelector::paintEvent(QPaintEvent *event) {
    // get a painter object and begin painting
    QPainter painter;
    painter.begin(this);

    // draw a white background for the tool
    painter.setPen(white);
    painter.setBrush(white);
    painter.drawRect(0, 0, 32, 32);

    // draw a tool depending on what tool constant we have
    if(_tool == OP_POINT_CIRCLE)
        drawPointCircleTool(painter);
    else if(_tool == OP_POINT_X)
        drawPointXTool(painter);
    else if(_tool == OP_POINT_SQUARE)
        drawPointSquareTool(painter);
    else if(_tool == OP_LINE_FREEFORM)
        drawLineFreeformTool(painter);
    else if(_tool == OP_LINE_STRAIGHT)
        drawLineStraightTool(painter);
    else if(_tool == OP_DRAW_TEXT)
        drawTextTool(painter);

    // if this tool is selected then draw a border around the edge of the image to indicate selection
    if(_selected) {
        QPen border_pen(QColor(0, 160, 160));
        border_pen.setWidth(6);
        painter.setPen(border_pen);

        painter.drawLine(0, 0, 0, 32);
        painter.drawLine(0, 0, 32, 0);
        painter.drawLine(0, 32, 32, 32);
        painter.drawLine(32, 0, 32, 32);
    }

    // finish the painting when done
    painter.end();
}

void ToolSelector::drawPointXTool(QPainter &painter) {
    // set the point size to three and draw a line in the middle
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(19, 13, 13, 19);
    painter.drawLine(13, 13, 19, 19);
}

void ToolSelector::drawLineFreeformTool(QPainter &painter) {
    // set the point size to three and draw a line in the middle
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(0, 32, 8, 16);
    painter.drawLine(8, 16, 24, 16);
    painter.drawLine(24, 16, 32, 0);
}

// function that will draw the straight line tool
void ToolSelector::drawLineStraightTool(QPainter &painter) {
    // set the point size to 2 and draw the line
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(0, 32, 32, 0);
}

void ToolSelector::drawPointCircleTool(QPainter &painter) {
    // set the point size to three and draw a point in the middle
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(13, 13, 6, 6);
}

void ToolSelector::drawPointSquareTool(QPainter &painter) {
    // set the pen width to one and draw a larger circle for this point
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(13, 13, 6, 6);
}

// function that will draw the text tool
void ToolSelector::drawTextTool(QPainter &painter) {
    // get a font and figure out the width and height of a T
    QFont font(QString("Arial"), 20, QFont::Bold);
    QFontMetrics metrics = painter.fontMetrics();
    int width = metrics.horizontalAdvance(QString("T"));
    int height = metrics.height();

    // draw the text in the middle
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setFont(font);
    painter.drawText(16 - (width), 16 + (height / 2), QString("T"));
}
