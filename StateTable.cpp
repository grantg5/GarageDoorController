#include "StateTable.hpp"

#include <iostream>
#include <string>
#include <map>
using namespace std;

StateTable::StateTable() {}

void StateTable::acceptEvent(Event event) {
	cout << "In accept event" << endl;
	State* fullCurrentState = states[currentState];
	cout << fullCurrentState->name << endl;

	// Given the current state and the name of the event coming in, looks for the Transition from said
	// state, that handles said event.
	string nextStateString = "";
	for (unsigned int i = 0; i < fullCurrentState->transitions.size(); i++) {
		cout << "In for" << endl; //DEBUG
		Transition* iTransition = fullCurrentState->transitions[i]; //TODO; Breaking here.
		cout << iTransition->eventId << endl;
		if (iTransition->eventId == event.eventName) {
			nextStateString = iTransition->nextState;
			break;
		}
	}

	if (nextStateString != "") {
		currentState = nextStateString;
	}
}
