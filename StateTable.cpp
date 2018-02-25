#include "StateTable.hpp"

#include <string>
#include <map>
using namespace std;

StateTable::StateTable() {}

void StateTable::acceptEvent(Event event) {
	State* fullCurrentState = states[currentState];

	string nextStateString = "";
	for (int i = 0; i < fullCurrentState->transitions.size(); i++) {
		Transition* iTransition = fullCurrentState->transitions[i];
		if (iTransition->eventId == event.eventName) {
			nextStateString = iTransition->nextState;
			break;
		}
	}

	if (nextStateString != "") {
		currentState = nextStateString;
	}
}
