// colourselector.cpp
//
// implements everything described in colourselector.hpp

// includes
#include <QColor>
#include <QMouseEvent>
#include "colourselector.hpp"

// default constructor for the class that will default the background to black
ColourSelector::ColourSelector(QWidget *parent)
: QWidget(parent), red(0), green(0), blue(0)
{
    // set a palette that we will use to paint the background to the provided colour
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
}

// constructor for the class that will take in an RGB value and will set it for this selector
ColourSelector::ColourSelector(int red, int green, int blue, QWidget *parent)
: QWidget(parent), red(red), green(green), blue(blue)
{
    // set a palette that we will use to paint the background to the provided colour
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
}

// destructor for the class
ColourSelector::~ColourSelector() {

}

// mouse release event that will
void ColourSelector::mouseReleaseEvent(QMouseEvent *event) {
    // if we get a release then we have a full click and can emit the signal
    emit clicked(red, green, blue);
}
