#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"

#include <iostream>
using namespace std;

/*
void * InputScanner::scan(void *arg) {
	Context context = Context();
	context.setupStateTable();

	while(1) {
		KeyboardScanner keyScanner = KeyboardScanner();
		Event inputEvent = keyScanner.scanInput();

		if (inputEvent.eventName != "NoEvent") {
			context.acceptEvent(inputEvent);
		}
	}
}
*/

void InputScanner::scan() {
	Context context = Context();
	context.setupStateTable();

	while(1) {
		cout << "Please enter input: ";
		KeyboardScanner keyScanner = KeyboardScanner();
		Event inputEvent = keyScanner.scanInput();

		if (inputEvent.eventName != "NoEvent") {
			context.acceptEvent(inputEvent);
		}
	}
}
