#include "Context.hpp"
#include "State.hpp"
#include "Transition.hpp"

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <unistd.h>
#include <pthread.h>
using namespace std;

bool Context::motorUp = false;
bool Context::motorDown = false;
bool Context::infraredBeam = false;
int Context::position = 0;
bool Context::toRaise = true;

StateTable * stateTable = new StateTable();

queue<Event> * contextQueue = new queue<Event>();

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
	stateClosingTransitionVector.push_back(new Transition("InfraredBeam", "StatePausedClosing"));
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
	/*
	cout << "Run with event " + eventIn.eventName << endl;
	// If we could get threading working, the input scanner thread would write to this queue itself
	::contextQueue->push(eventIn);
	*/

	while(true) {
		// Ingest event from event queue if one exists
		if (!::contextQueue->empty()){
			Event event = ::contextQueue->front();
			::contextQueue->pop();
			//send event into the state table
			if(!::stateTable->acceptEvent(new Event(event.keyPressed, event.eventName))){
				break;
			}
		}
		// Logic for timer while opening
		if (::stateTable->currentState == "StateOpening"){
			position++;
			cout << "Opening... ";
			cout << position << endl;
			if (position == 10){
				// self trigger an even for the door being fully open
				::stateTable->acceptEvent(new Event('o', "FullyOpen"));
				break;
			}
		}
		// Logic for timer while closing
		if (::stateTable->currentState == "StateClosing"){
			position--;
			cout << "Closing... ";
			cout << position << endl;
			if (position == 0) {
				// self trigger an even for the door being fully closed
				::stateTable->acceptEvent(new Event('c', "FullyClosed"));
				break;
			}
		}
		// Sleep for one second to mimic the cycling
		usleep(999999);
	}
}
