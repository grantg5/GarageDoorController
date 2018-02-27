#include "Transition.hpp"

Transition::Transition(string eventIdIn, string nextStateIn): eventId(eventIdIn), nextState(nextStateIn) {}

bool Transition::guard() {
	// Note: Guards are not implemented as of R1.
}

string Transition::acceptEvent() {
	//Note: This has not been implemented as of R1.
}
