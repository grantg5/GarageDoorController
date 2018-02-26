#include "Context.hpp"
#include "State.hpp"
#include "Transition.hpp"
#include "Globals.hpp"

#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <pthread.h>
using namespace std;

bool BUTTON_PRESSED = false;
bool OVERCURRENT = false;
bool INFRARED_BEAM = false;

bool Context::motorUp = false;
bool Context::motorDown = false;
bool Context::infraredBeam = false;
int Context::position = 0;
bool Context::toRaise = true;
StateTable Context::stateTable = StateTable();

Context::Context() {}

void Context::setupStateTable() {
	map<string, State*> states;

	//Closed
	vector<Transition*> stateClosedTransitionVector(1);
	stateClosedTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
	states.insert(make_pair("StateClosed", new State("StateClosed", stateClosedTransitionVector)));

	//Opening
	vector<Transition*> stateOpeningTransitionVector(3);
	stateOpeningTransitionVector.push_back(new Transition("ButtonPress", "StatePausedOpening"));
	stateOpeningTransitionVector.push_back(new Transition("Overcurrent", "StatePausedOpening"));
	stateOpeningTransitionVector.push_back(new Transition("FullyOpen", "StateOpen"));
	states.insert(make_pair("StateOpening", new State("StateOpening", stateOpeningTransitionVector)));

	//PausedOpening
	vector<Transition*> statePausedOpeningTransitionVector(1);
	statePausedOpeningTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StatePausedOpening", new State("StatePausedOpening", statePausedOpeningTransitionVector)));

	//Open
	vector<Transition*> stateOpenTransitionVector(1);
	stateOpenTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StateOpen", new State("StateOpen", stateOpenTransitionVector)));

	//Closing
	vector<Transition*> stateClosingTransitionVector(4);
	stateClosingTransitionVector.push_back(new Transition("FullyClosed", "StateClosed"));
	stateClosingTransitionVector.push_back(new Transition("InfraredBeam", "StatePausedClosing"));
	stateClosingTransitionVector.push_back(new Transition("ButtonPress", "StatePausedClosing"));
	stateClosingTransitionVector.push_back(new Transition("Overcurrent", "StateOpening"));
	states.insert(make_pair("StateClosing", new State("StateClosing", stateClosingTransitionVector)));

	//PausedClosing
	vector<Transition*> statePausedClosingTransitionVector(1);
	statePausedClosingTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
	states.insert(make_pair("StatePausedClosing", new State("StatePausedClosing", statePausedClosingTransitionVector)));

	Context::stateTable.states = states;
	Context::stateTable.currentState = "StateClosed";
}

/*
void * Context::run(void *arg) {
	while(1) {

	}
}
*/

void Context::acceptEvent(Event event) {
	if (event.eventName == "ButtonPress") {
		BUTTON_PRESSED = true;
		OVERCURRENT = false;
		INFRARED_BEAM = false;

		//If the door is in either of these states and the button is pressed, the door should close.
		string statesToCloseFrom[2] = {"StateOpen", "StatePausedOpening"};
		for (int i = 0; i < 2; i++) {
			if (statesToCloseFrom[i] == stateTable.currentState) {
				Context::toRaise = false;
				break;
			}
		}

		//See above comment, but instead the door should open.
		string statesToOpenFrom[2] = {"StateClosed", "StatePausedClosing"};
		for (int i = 0; i < 2; i++) {
			if (statesToOpenFrom[i] == stateTable.currentState) {
				Context::toRaise = true;
				break;
			}
		}

		pthread_t timerThread;
		pthread_create(&timerThread, NULL, Context::startTimer, NULL);
	} else if (event.eventName == "Overcurrent") {
		BUTTON_PRESSED = false;
		OVERCURRENT = true;
		INFRARED_BEAM = false;
	} else if (event.eventName == "InfraredBeam") {
		BUTTON_PRESSED = false;
		OVERCURRENT = false;
		INFRARED_BEAM = true;
	}

	stateTable.acceptEvent(event);
}

void * Context::startTimer(void *arg) {
	//NOTE: This is based on the assumption that the timer thread and input threads don't lock each
	//		other out. If this proves to not be the case, the mutex soltution will be required.
	while (Context::position > 0 || Context::position < 10) {
		cout << Context::position << endl;

		if (BUTTON_PRESSED == false && OVERCURRENT == false && INFRARED_BEAM == false) {
			if (Context::toRaise) {
				Context::motorUp = true;
				Context::position++;
			} else {
				Context::motorDown = true;
				Context::position--;
			}
			sleep(1);
		} else {
			//TODO: May or may not have to transition state in here, acceptEvent() should handle it.

			break;
		}
	}

	if (position == 0) {
		//TODO: Create event.
		Event closedEvent = Event('c', "FullyClosed");
		Context::stateTable.acceptEvent(closedEvent);
	} else if (position == 10) {
		Event openEvent = Event('o', "FullyOpen");
		Context::stateTable.acceptEvent(openEvent);
	}

	Context::motorUp = false;
	Context::motorDown = false;
}
