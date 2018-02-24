#include "State.hpp"

State::State(string nameIn, vector<Transition*> transitionsIn): name(nameIn), transitions(transitionsIn) {}

void State::entry(){
	//TODO: Fill in
	//PROBLEM? How to have seperate entry & exit functions for each state?
}

void State::exit() {
	//TODO: Fill in
}

string State::accept(Event event) {
	//TODO: Fill in
}
