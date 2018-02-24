#include "InputScanner.hpp"
#include <pthread.h>

int main() {
    pthread_t keyboardScannerThread;
    pthread_t inputScannerThread;
    pthread_t gdcThread;
    pthread_create(&keyboardScannerThread, NULL, KeyboardScanner::scanInput, NULL);
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);
    pthread_create(&gdcThread, NULL, Context::run, NULL);

    return 0;
}
