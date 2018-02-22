#include "Transition.hpp"
#include "Event.hpp"
#include<string>
#include<vector>
using namespace std;

#ifndef STATE_H_
#define STATE_H_


class State {
public:
	string name;
	vector<Transition> transitions;

	State(string nameIn, vector<Transition> transitionsIn);

	void entry();
	void exit();
	string accept(Event);
};

#endif
