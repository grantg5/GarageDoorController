#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"

#include <pthread.h>
#include <iostream>

int main() {
	// Note: Due to issues with threading, the below code is commented out for now.
	/*
    pthread_t inputScannerThread;
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);
	*/

	// This method call will be deleted when threading works.
	InputScanner::scan();

    return 0;
}
