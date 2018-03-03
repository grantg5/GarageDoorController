#include "Transition.hpp"

/*
 * Transition objects contain the event information and the string indicating what the next state is
 */
Transition::Transition(string eventIdIn, string nextStateIn)
{
	eventId = eventIdIn;
	nextState = nextStateIn;
}

// currently unimplemented guards
bool Transition::guard() {}

// currently unimplemented accepting logic if entry and exit states existed
string Transition::acceptEvent() {}
