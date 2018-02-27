#include "Context.hpp"
#include "State.hpp"
#include "Transition.hpp"

#include <iostream>
#include <string>
#include <map>
//#include <queue>
#include <unistd.h>
//#include <pthread.h>
using namespace std;

bool Context::motorUp = false;
bool Context::motorDown = false;
bool Context::infraredBeam = false;
//int Context::position = 0;
//bool Context::toRaise = true;

StateTable Context::stateTable = StateTable();

//queue<Event> contextQueue = new queue<Event>();

Context::Context() {}

void Context::setupStateTable() {
	map<string, State*> states;

	/*
	//Closed
	vector<Transition*> stateClosedTransitionVector(1);
	stateClosedTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
	states.insert(make_pair("StateClosed", new State("StateClosed", stateClosedTransitionVector)));
	*/

	//Closed (backup version)
	vector<Transition*> stateClosedTransitionVector(1);
	stateClosedTransitionVector.push_back(new Transition("ButtonPress", "StateOpen"));
	states.insert(make_pair("StateClosed", new State("StateClosed", stateClosedTransitionVector)));

	/*
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
	*/

	/*
	//Open
	vector<Transition*> stateOpenTransitionVector(1);
	stateOpenTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StateOpen", new State("StateOpen", stateOpenTransitionVector)));
	*/

	//Open (backup version)
	vector<Transition*> stateOpenTransitionVector(1);
	stateOpenTransitionVector.push_back(new Transition("ButtonPress", "StateClosing"));
	states.insert(make_pair("StateOpen", new State("StateOpen", stateOpenTransitionVector)));

	/*
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
	*/

	Context::stateTable.states = states;
	Context::stateTable.currentState = "StateClosed";
}

// Note: This is the context method that implements multi-threading and fulfills all requirements of the project.
/*
void * Context::run(void *arg) {
	while(1) {
		if (!contextQueue.empty()){
			Event event = contextQueue.pop();
			acceptEvent(event);
		}
		if (stateTable.currentState.equals("StateOpening")){
			position++;
			if (position == 10){
				stateTable.acceptEvent(new Event("o", "FullyOpen"));
			}
		}
		if (stateTable.currentState.equals("StateClosing")){
			position--;
			if (position == 0) {
				stateTable.acceptEvent(new Event("c", "FullyClosed"));
			}
		}

		usleep(999999);
	}
}
*/

// Note: This function only exists as a backup to expected functionality. If the system is fully
//		 functional, then this function will be deleted. All it does is move the door up and down.
//		 It does not handle threading, nor does it allow for interrupts. Little Johnny just needs
//		 to be careful if he's walking under a closing garage door.
void Context::moveDoor(bool opening) {
	if (opening) {
		cout << "Opening Door" << endl;
		int count = 1;
		Context::motorUp = true;

		while (count <= 10) {
			cout << "Door position: " << count << endl;
			usleep(999999);
			count++;
		}

		Context::motorUp = false;
		cout << "Door's Open" << endl;
	} else {
		cout << "Closing Door" << endl;
		int count = 10;
		Context::motorDown = true;
		Context::infraredBeam = true;

		while (count >= 1) {
			cout << "Door position: " << count << endl;
			usleep(999999);
			count--;
		}

		Context::infraredBeam = false;
		Context::motorDown = false;
		cout << "Door's Closed" << endl;
	}
}

void Context::acceptEvent(Event event) {
	//TODO: Keep this like this, if it doesn't work, change to just check current state.
	//		If closed, count to 10 then open. If open, vice versa.
	//stateTable.acceptEvent(event);

	if (stateTable.currentState == "StateClosed") {
		stateTable.currentState = "StateOpening";
		Context::moveDoor(true);
		stateTable.currentState = "StateOpen";
	} else {
		stateTable.currentState = "StateClosing";
		Context::moveDoor(false);
		stateTable.currentState = "StateClosed";
	}
}
