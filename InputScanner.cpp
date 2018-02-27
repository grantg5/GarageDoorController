#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include "Context.hpp"

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
