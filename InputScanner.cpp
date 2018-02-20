#include <iostream>
#include <thread>
using namespace std;

class InputScanner {
    int scan() {
        while (1) {
            char input;
            cin >> input;

            
        }

        return 0;
    }

    int main() {
        //TODO: call scan
        std::thread scannerThread(scan);
        return 0;
    }
}