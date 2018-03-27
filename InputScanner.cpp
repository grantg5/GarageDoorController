#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"
#include "GPIOController.hpp"

#include <iostream>
#include <queue>
#include <time.h>
#include <unistd.h>
using namespace std;

timespec openTimer;
timespec closeTimer;
timespec infraredTimer;
timespec motorTimer;
timespec pushButtonTimer;

/*
 * Input Scanner receives input from the keyboard scanner in the form of events
 * The event is then routed to the context of the garage door controller state machine
 */
void * InputScanner::scan(void *arg) {
	int prevPortAVal = 0x0;
	bool gpioInitialized = true;

	if (!Context::simulation) {
		if (!GPIOController::init()) {
			gpioInitialized = false;
			cout << "GPIO Controller initialization failed.";
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
			if (!gpioInitialized) {
				cout << "init failed" << endl;
				break;
			}
			/*
			clock_gettime(CLOCK_MONOTONIC, &openTimer);
			clock_gettime(CLOCK_MONOTONIC, &closeTimer);
			clock_gettime(CLOCK_MONOTONIC, &pushButtonTimer);
			clock_gettime(CLOCK_MONOTONIC, &infraredTimer);
			clock_gettime(CLOCK_MONOTONIC, &motorTimer);
			*/

			GPIOController::scanA();

			if (GPIOController::portAVal != prevPortAVal) {
				prevPortAVal = GPIOController::portAVal;
				Event inputEvent = GPIOController::translateInput();
				Context::contextQueue->push(inputEvent);
			}
		}

		usleep(300000);
	}
}
