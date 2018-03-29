#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"
#include "GPIOController.hpp"

#include <iostream>
#include <queue>
#include <unistd.h>
using namespace std;

/*
 * Input Scanner receives input from the keyboard scanner in the form of events
 * The event is then routed to the context of the garage door controller state machine
 */
void * InputScanner::scan(void *arg) {
	int prevPortAVal = 0x0;
	bool gpioInitialized = true;

	GPIOController gpioController = GPIOController();

	if (!Context::simulation) {
		if (!gpioController.init()) {
			gpioInitialized = false;
			cout << "GPIO Controller initialization failed.";
			return 0;
		}
	}

	while(1) {
		if (Context::simulation) {
			cout << "Please enter input: ";
			KeyboardScanner keyScanner = KeyboardScanner();
			Event inputEvent = keyScanner.scanInput();

			if (inputEvent.eventName != "NoEvent") {
				Context::contextQueue->push(inputEvent);
			}
		} else {
			gpioController.scanA();

			//if statement is a further safeguard against multiple actions resulting from one button press.
			if (gpioController.portAVal != prevPortAVal) {
				prevPortAVal = gpioController.portAVal;
				Event inputEvent = gpioController.translateInput();
				Context::contextQueue->push(inputEvent);
			}
		}

		usleep(200000);
	}
}
