#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"
#include <iostream>
#include <queue>
using namespace std;

//queue<Event> ** contextQueuePointer = &Context::contextQueue;

/*
 * Input Scanner receives input from the keyboard scanner in the form of events
 * The event is then routed to the context of the garage door controller state machine
 */
void * InputScanner::scan(void *arg) {
	while(1) {
		cout << "Please enter input: ";
		KeyboardScanner keyScanner = KeyboardScanner();
		Event inputEvent = keyScanner.scanInput();

		if (inputEvent.eventName != "NoEvent") {
			Context::contextQueue->push(inputEvent);
		}

		// If the event is not NoEvent (a non-recognized event) send it to the context
		/*
		if (inputEvent.eventName != "NoEvent") {
			context.run(inputEvent);
		}
		*/
	}
}
