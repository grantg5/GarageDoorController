#include "StateTable.hpp"

#include <iostream>
#include <string>
#include <map>
using namespace std;

StateTable::StateTable() {}

bool StateTable::acceptEvent(Event* event) {
	State* fullCurrentState = this->states[currentState];

	// Given the current state and the name of the event coming in, looks for the Transition from said
	// state, that handles said event.
	string nextStateString = "";
	for (unsigned int i = 0; i < fullCurrentState->transitions.size(); i++) {
		Transition* iTransition = fullCurrentState->transitions[i];
		if (iTransition->eventId == event->eventName) {
			nextStateString = iTransition->nextState;
			break;
		}
	}

	// If the next state has a transition for this event set new state
	if (nextStateString != "") {
		currentState = nextStateString;
		cout << "State is now " + currentState << endl;
		return true;
	}
	// If the state does not have a transition for this event explain this
	else {
		cout << "No transition for " + event->eventName + " in state " + currentState<< endl;
		return false;
	}
}
