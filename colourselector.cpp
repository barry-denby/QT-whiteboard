// colourselector.cpp
//
// implements everything described in colourselector.hpp

// includes
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include "colourselector.hpp"

// default constructor for the class that will default the background to black
ColourSelector::ColourSelector(QWidget *parent)
: QWidget(parent), red(0), green(0), blue(0), _selected(false)
{
    // set a palette that we will use to paint the background to the provided colour
    //QPalette pal = palette();
    //pal.setColor(QPalette::Background, QColor(red, green, blue));
    //this->setAutoFillBackground(true);
    //this->setPalette(pal);

    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
    this->setMinimumWidth(32);
    this->setMinimumHeight(32);
}

// constructor for the class that will take in an RGB value and will set it for this selector
ColourSelector::ColourSelector(int red, int green, int blue, QWidget *parent)
: QWidget(parent), red(red), green(green), blue(blue), _selected(false)
{
    // set a palette that we will use to paint the background to the provided colour
    //QPalette pal = palette();
    //pal.setColor(QPalette::Background, QColor(red, green, blue));
    //this->setAutoFillBackground(true);
    //this->setPalette(pal);

    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
    this->setMinimumWidth(32);
    this->setMinimumHeight(32);
}

// destructor for the class
ColourSelector::~ColourSelector() {

}

// returns the colour for this selector
const QColor ColourSelector::colour() {
    return QColor(red, green, blue);
}

// returns the selected state of this selector
const bool ColourSelector::selected() {
    return _selected;
}

// set the selected state of this selector
void ColourSelector::setSelected(const bool selected) {
    _selected = selected;
    repaint();
}

// mouse release event that will
void ColourSelector::mouseReleaseEvent(QMouseEvent *event) {
    // if we get a release then we have a full click and can emit the signal
    emit clicked(red, green, blue);

    // mark this tool as selected and repaint
    _selected = true;
    repaint();
}

// overridden paint event
void ColourSelector::paintEvent(QPaintEvent *event) {
    // get the painter for this object and start painting
    QPainter painter;
    painter.begin(this);

    // set the colour and paint a full background square
    painter.setPen(QColor(red, green, blue));
    painter.setBrush(QColor(red, green, blue));
    painter.drawRect(0, 0, 32, 32);

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

    // finish the painting of this widget
    painter.end();
}
