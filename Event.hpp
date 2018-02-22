#include <string>

#ifndef MY_HEADER_H
#define MY_HEADER_H

class Event {
    public:
        char keyPressed;
        std::string eventName;

        Event(char keyPressedIn, std::string eventNameIn);
};

#endif
