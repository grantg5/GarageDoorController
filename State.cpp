#include "State.hpp"

State::State(string nameIn, vector<Transition*> transitionsIn): name(nameIn), transitions(transitionsIn) {}

// logic for any entry methods would go here
void State::entry(){}

// logic for any exit methods would go here
void State::exit() {}

// logic to receive an event was implemented in the context instead because pointers are hard
string State::accept(Event event) {}
