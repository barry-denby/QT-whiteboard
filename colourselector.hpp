#ifndef _COLOURSELECTOR_HPP
#define _COLOURSELECTOR_HPP

// colourselector.hpp
//
// defines a colourselector class that will be a simple widget that will display a colour and if clicked will emit
// a signal with that colour

// includes
#include <QWidget>

// class definition
class ColourSelector : public QWidget {
    // needed to get access to the signals and slots mechanism
    Q_OBJECT
// public section of the class
public:
    // default constructor for the class
    ColourSelector(QWidget *parent = 0);
    // constructor for the class that takes in an RGB value to display on this selector
    ColourSelector(int red, int green, int blue, QWidget *parent = 0);
    // destructor for the class
    ~ColourSelector();
};

#endif // _COLOURSELECTOR_HPP
