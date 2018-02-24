#include "State.hpp"

State::State(string nameIn, vector<Transition*> transitionsIn): name(nameIn), transitions(transitionsIn) {}

void State::entry(){
	//TODO: Fill in
}

void State::exit() {
	//TODO: Fill in
}

string State::accept(Event event) {
	//TODO: Fill in
}
