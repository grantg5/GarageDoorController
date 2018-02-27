#include "InputScanner.hpp"
#include "KeyboardScanner.hpp"

#include <pthread.h>

int main() {
    pthread_t inputScannerThread;
    pthread_create(&inputScannerThread, NULL, InputScanner::scan, NULL);

    return 0;
}
