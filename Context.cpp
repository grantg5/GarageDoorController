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

StateTable Context::stateTable = StateTable();

queue<Event> contextQueue = new queue<Event>();

Context::Context() {}

void Context::setupStateTable() {
	map<string, State*> states;

	//Closed
	vector<Transition*> stateClosedTransitionVector(1);
	stateClosedTransitionVector.push_back(new Transition("ButtonPress", "StateOpening"));
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

	//Open
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
		int count = 1;
		while (count <= 10) {
			cout << count << endl;
			usleep(999999);
			count++;
		}
	} else {
		int count = 10;
		while (count >= 1) {
			count << count << endl;
			usleep(999999);
			count--;
		}
	}
}

void Context::acceptEvent(Event event) {
	//if (event.eventName == "ButtonPress") {
	//	pthread_t timerThread;
	//	pthread_create(&timerThread, NULL, Context::startTimer, NULL);
	//}

	//TODO: Keep this like this, if it doesn't work, change to just check current state.
	//		If closed, count to 10 then open. If open, vice versa.
	stateTable.acceptEvent(event);
	if (stateTable.currentState = "StateClosed") {
		Context::moveDoor(true);
	} else {
		Context::moveDoor(false);
	}
}
/*
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
*/
