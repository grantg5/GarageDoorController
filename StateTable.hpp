#include "Event.hpp"
#include "State.hpp"

#include<map>
#include<string>
using namespace std;

#ifndef STATETABLE_H_
#define STATETABLE_H_

class StateTable {
public:
	map<string, State> states;
	string currentState;

	StateTable(map<string, State> statesIn, string currentStateIn);

	void acceptEvent(Event);
	void transitionState(State);
};

#endif
