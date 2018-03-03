#include "Event.hpp"
#include <string>
using namespace std;

/*
 * Event class for containing data of the keypress and event name
 */
Event::Event(char keyPressedIn, string eventNameIn): keyPressed(keyPressedIn), eventName(eventNameIn) {}
