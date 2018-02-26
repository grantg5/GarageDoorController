#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"

#include <iostream>
using namespace std;

void * InputScanner::scan(void *arg) {
	Context context = Context();

	while(1) {
		KeyboardScanner keyScanner = KeyboardScanner();
		Event inputEvent = keyScanner.scanInput();

		if (inputEvent.eventName != "NoEvent") {
			context.acceptEvent(inputEvent);
		}
	}
}
