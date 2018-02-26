#include "InputScanner.hpp"
#include "Context.hpp"
#include "KeyboardScanner.hpp"

#include <pthread.h>

int main() {
	Context context = Context();

    pthread_t keyboardScannerThread;
    pthread_t inputScannerThread;
    pthread_t gdcThread;
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);
    //pthread_create(&gdcThread, NULL, Context::run, NULL);

    return 0;
}
