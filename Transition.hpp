#include <string>
using namespace std;

#ifndef TRANSITION_HPP_
#define TRANSITION_HPP_

class Transition {
public:
	string eventId;
	string nextState;

	Transition(string eventIdIn, string nextStateIn);

	bool guard();
	string acceptEvent();
};

#endif
