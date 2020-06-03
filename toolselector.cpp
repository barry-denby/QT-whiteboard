// toolselector.cpp
//
// implements everything described in toolselector.hpp

// includes
#include <QPainter>
#include "constants.hpp"
#include "toolselector.hpp"

// constructor for the class that takes in the tool type
ToolSelector::ToolSelector(const unsigned int tool, QWidget *parent)
: QWidget(parent), tool(tool), black(QColor(0, 0, 0)), white(QColor(255, 255, 255)), pen(black), brush(black)
{
    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
}

// destructor for the class
ToolSelector::~ToolSelector() {

}

void ToolSelector::paintEvent(QPaintEvent *event) {
    // get a painter object and begin painting
    QPainter painter;
    painter.begin(this);

    // draw a white background for the tool
    painter.setPen(white);
    painter.setBrush(white);
    painter.drawRect(0, 0, 32, 32);

    // draw a tool depending on what tool constant we have
    if(tool == OP_POINT_FIXED_SIZE)
        drawPointTool(painter);
    else if(tool == OP_LINE_FIXED_THICKNESS)
        drawLineTool(painter);
    else if(tool == OP_POINT_VARIABLE_SIZE)
        drawPointVariableTool(painter);

    // finish the painting when done
    painter.end();
}

void ToolSelector::drawLineTool(QPainter &painter) {
    // set the point size to three and draw a line in the middle
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(0, 32, 32, 0);
}

void ToolSelector::drawPointTool(QPainter &painter) {
    // set the point size to three and draw a point in the middle
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawPoint(16, 16);
}

void ToolSelector::drawPointVariableTool(QPainter &painter) {
    // set the pen width to one and draw a larger circle for this point
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(13, 13, 6, 6);
}
