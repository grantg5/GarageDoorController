#include "InputScanner.hpp"
#include <pthread.h>

int main() {
    pthread_t thread1;
    pthread_create(&thread1, NULL, InputScanner::scan, NULL);

    return 0;
}
