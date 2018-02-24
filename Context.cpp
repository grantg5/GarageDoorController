#include "Context.hpp"
#include "State.hpp"
#include "Transition.hpp"

#include <string>
#include <map>
using namespace std;

Context::Context(): motorUp(false), motorDown(false), infraredBeam(false) {}

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

	stateTable = StateTable(states, "StateClosed");
}

void * Context::run(void *arg) {
	while(1) {

	}
}

void Context::acceptEvent(Event event) {
	stateTable.acceptEvent(event);
}

void Context::startTimer(int position) {
	//TODO: Fill out
}
