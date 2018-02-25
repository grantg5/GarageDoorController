#include "Event.hpp"
#include "State.hpp"

#include<map>
#include<string>
using namespace std;

#ifndef STATETABLE_H_
#define STATETABLE_H_

class StateTable {
public:
	map<string, State*> states;
	string currentState;

	StateTable();

	void acceptEvent(Event);
};

#endif
