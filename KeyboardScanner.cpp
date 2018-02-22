<<<<<<< HEAD
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
=======
#include <iostream>
#include <thread>
using namespace std;

class InputScanner {
    int scanInput() {
        while (1) {
            char input;
            cin >> input;

            
        }

        return 0;
    }
}
>>>>>>> 48b4fa8eb874bea94648a94488ad3735a821693d
