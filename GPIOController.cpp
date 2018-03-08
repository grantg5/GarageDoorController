#include "GPIOController.hpp"
#include <sys/neutrino.h>
#include <stdint.h>
#include <sys/mman.h>
#include <iostream>
using namespace std;

#define IO_PORT_SIZE 1
#define CTRL_ADDRESS 0x280

bool fetchPermission() {
	if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
		cout << "Failed to get I/O access permission" << endl;
		return false;
	}

	return true;
}

bool mapPorts() {
	if (fetchPermission()) {
		uintptr_t ctrlHandle;
		ctrlHandle = mmap_device_io(IO_PORT_SIZE, CTRL_ADDRESS);

		if (ctrlHandle != MAP_DEVICE_FAILED) {
			return true;
		}
	}

	return false;
}




