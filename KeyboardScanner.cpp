#include "KeyboardScanner.hpp"
#include <iostream>
#include <string>
using namespace std;

/*
 * The keyboard scanner is the class that will be receiving keyboard input
 * The keyboard input creates events that will be ingested elsewhere in the program
 */
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
        	//create no event that will be ignored further in execution
            return Event('n', "NoEvent");
    }
}
