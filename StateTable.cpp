#include "StateTable.hpp"

StateTable::StateTable(map<string, State*> statesIn, string currentStateIn): states(statesIn), currentState(currentStateIn) {}

void StateTable::acceptEvent(Event event) {
	//TODO: Fill in
}

void StateTable::transitionState(State nextState) {
	//TODO: Fill in
}
