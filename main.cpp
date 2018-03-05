#include "InputScanner.hpp"
#include "Context.hpp"
#include "KeyboardScanner.hpp"

#include <pthread.h>

int main() {
	Context context = Context();

    pthread_t inputScannerThread;
    pthread_t gdcThread;

    pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);

    pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);		// initialize thread attributes structure
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&gdcThread, NULL, Context::run, NULL);

    while (true) {}

    return 0;
}
