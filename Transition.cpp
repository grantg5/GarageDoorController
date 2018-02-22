#include "Transition.hpp"

Transition::Transition(string eventIdIn, string nextStateIn): eventId(eventIdIn), nextState(nextStateIn) {}

bool Transition::guard() {
	//TODO: fill in
}

string Transition::acceptEvent() {
	//TODO: fill in
}
