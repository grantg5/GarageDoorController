#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"

#include <iostream>
using namespace std;

// Note: The below method will be used when proper Context/InputScanner threading is implemented.
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

// Note: The below code will be deleted and replaced by the above function when threading is implemented.
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
