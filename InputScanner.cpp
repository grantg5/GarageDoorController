#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"
#include <iostream>
using namespace std;

void * InputScanner::scan(void *arg) {
	while(1) {
		KeyboardScanner keyScanner = KeyboardScanner();
		Event inputEvent = keyScanner.scanInput();
		cout << inputEvent.keyPressed;
		cout << inputEvent.eventName;

		//TODO: Talk w/ Kadeer on how events get sent to context.
	}
}
