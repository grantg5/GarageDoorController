#include "Context.hpp"
#include "State.hpp"
#include "Transition.hpp"
#include "GPIOController.hpp"

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <unistd.h>
#include <pthread.h>
using namespace std;

bool Context::motorUp = false;
bool Context::motorDown = false;
bool Context::doorOpen = false;
bool Context::doorClosed = true;
bool Context::infraredBeam = false;
int Context::position = 0;
bool Context::toRaise = true;
bool Context::simulation = false;

StateTable * stateTable = new StateTable();

queue<Event>* Context::contextQueue = new queue<Event>();

Context::Context() {
	// Initializing our state machine's states
	// First create the state map
	map<string, State*> states;

	// Create the vector of transitions for a state then insert the new state into the state map

	//Closed
	vector<Transition*> stateClosedTransitionVector(0);
	stateClosedTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
	states.insert(make_pair("StateClosed", new State("StateClosed", stateClosedTransitionVector)));

	//Opening
	vector<Transition*> stateOpeningTransitionVector(0);
	stateOpeningTransitionVector.push_back(new Transition("ButtonPress", "StatePausedOpening"));
	stateOpeningTransitionVector.push_back(new Transition("Overcurrent", "StatePausedOpening"));
	stateOpeningTransitionVector.push_back(new Transition("FullyOpen", "StateOpen"));
	states.insert(make_pair("StateOpening", new State("StateOpening", stateOpeningTransitionVector)));

	//PausedOpening
	vector<Transition*> statePausedOpeningTransitionVector(0);
	statePausedOpeningTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StatePausedOpening", new State("StatePausedOpening", statePausedOpeningTransitionVector)));

	//Open
	vector<Transition*> stateOpenTransitionVector(0);
	stateOpenTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StateOpen", new State("StateOpen", stateOpenTransitionVector)));

	//Closing
	vector<Transition*> stateClosingTransitionVector(0);
	stateClosingTransitionVector.push_back(new Transition("FullyClosed", "StateClosed"));
	stateClosingTransitionVector.push_back(new Transition("InfraredBeam", "StateOpening"));
	stateClosingTransitionVector.push_back(new Transition("ButtonPress", "StatePausedClosing"));
	stateClosingTransitionVector.push_back(new Transition("Overcurrent", "StateOpening"));
	states.insert(make_pair("StateClosing", new State("StateClosing", stateClosingTransitionVector)));

	//PausedClosing
	vector<Transition*> statePausedClosingTransitionVector(0);
	statePausedClosingTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
	states.insert(make_pair("StatePausedClosing", new State("StatePausedClosing", statePausedClosingTransitionVector)));

	// Save all of this into the state table
	::stateTable->states = states;
	::stateTable->currentState = "StateClosed";
	cout << "Starting state is... " + ::stateTable->currentState << endl;
}

// Method for execution of GarageDoorController state machine
void * Context::run(void *arg) {
	GPIOController gpioController = GPIOController();
	gpioController.init();

	while(true) {
		// Ingest event from event queue if one exists
		if (!Context::contextQueue->empty()){
			Event event = Context::contextQueue->front();
			Context::contextQueue->pop();
			//send event into the state table
			::stateTable->acceptEvent(new Event(event.keyPressed, event.eventName));
		}

		if (Context::simulation) {
			// Logic for timer while opening
			if (::stateTable->currentState == "StateOpening"){
				position++;
				cout << "Opening... ";
				cout << position << endl;
				if (position == 10){
					// self trigger an even for the door being fully open
					::stateTable->acceptEvent(new Event('o', "FullyOpen"));
				}
			}
			// Logic for timer while closing
			if (::stateTable->currentState == "StateClosing"){
				Context::infraredBeam = true;
				position--;
				cout << "Closing... ";
				cout << position << endl;

				if (position == 0) {
					// self trigger an even for the door being fully closed
					::stateTable->acceptEvent(new Event('c', "FullyClosed"));
					Context::infraredBeam = false;
				}
			}

			// Sleep for one second to mimic the cycling
			usleep(999999);
		} else {
			if (::stateTable->currentState == "StateOpening") {
				gpioController.raiseDoor();
			} else if (::stateTable->currentState == "StateClosing") {
				gpioController.lowerDoor();
			} else {
				gpioController.stopDoor();
			}
		}
	}
}
