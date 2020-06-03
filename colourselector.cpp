// colourselector.cpp
//
// implements everything described in colourselector.hpp

// includes
#include <QColor>
#include "colourselector.hpp"

// default constructor for the class that will default the background to black
ColourSelector::ColourSelector(QWidget *parent)
: QWidget(parent)
{
    // set a palette that we will use to paint the background to the provided colour
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0, 0, 0));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // force a max width and height
    this->setMaximumWidth(32);
    this->setMaximumHeight(32);
}

// constructor for the class that will take in an RGB value and will set it for this selector
ColourSelector::ColourSelector(int red, int green, int blue, QWidget *parent)
: QWidget(parent)
{
    // set a palette that we will use to paint the background to the provided colour
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(red, green, blue));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

// destructor for the class
ColourSelector::~ColourSelector() {

}
