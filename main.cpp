#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"

#include <pthread.h>
#include <iostream>

int main() {
	/*
    pthread_t inputScannerThread;
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);
	*/

	InputScanner::scan();

    return 0;
}
