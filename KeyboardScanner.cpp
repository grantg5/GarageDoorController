#include "KeyboardScanner.hpp"
#include <iostream>
#include <string>
using namespace std;

Event KeyboardScanner::scanInput() {
    char input;
    cin >> input;

    switch(input) {
        case 'r':
            return Event('r', "ButtonPress");
        case 'm':
            return Event('m', "Overcurrent");
        case 'i':
            return Event('i', "InfraredBeam");
        default:
            return Event('n', "NoEvent");
    }
}
