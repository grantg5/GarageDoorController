/**
 * Thanks to Zachary Campanella for explaining the hardware component of this project in simple terms.
 *
 * Thanks to Ronnie James Dio for the variable name inspiration, and for guiding young JB in
 * Tenacious D and the Pick of Destiny.
 */

#include "GPIOController.hpp"
#include "Context.hpp"

#include <sys/neutrino.h>
#include <sys/mman.h>
#include <unistd.h>
#include <hw/inout.h>
#include <iostream>
using namespace std;


#define DIO_A 0x288		//input
#define DIO_B 0x289		//output
#define DIO_CTRL 0x28B

uintptr_t GPIOController::ctrl;
uintptr_t GPIOController::portA;
uintptr_t GPIOController::portB;
int GPIOController::portAVal = 0x0;
int GPIOController::portBVal = 0x0;

void GPIOController::init() {
	bool *contextSimulationPointer = &Context::simulation;
	bool contextSimulation = *contextSimulationPointer;

	if (!contextSimulation) {
		if (fetchPermission() && mapPorts()) {
			out8(GPIOController::ctrl, 0x91);

			GPIOController::portAVal = 0x0;
			GPIOController::portBVal = 0x0;

			out8(GPIOController::portB, GPIOController::portBVal);
			sleep(1);
			GPIOController::portBVal |= (1u << 7); //Setting pin 5 high
			out8(GPIOController::portB, GPIOController::portBVal);
		}
	}
}

bool GPIOController::fetchPermission() {
	if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
		cout << "Failed to get I/O access permission" << endl;
		return false;
	}

	return true;
}

bool GPIOController::mapPorts() {
	if (fetchPermission()) {
		GPIOController::ctrl = mmap_device_io(1, DIO_CTRL);
		GPIOController::portA = mmap_device_io(1, DIO_A);
		GPIOController::portB = mmap_device_io(1, DIO_B);

		if (GPIOController::ctrl == MAP_DEVICE_FAILED) {
			cout << "ctrl mapping failed." << endl;
			return false;
		}
		if (GPIOController::portA == MAP_DEVICE_FAILED) {
			cout << "port A mapping failed" << endl;
			return false;
		}
		if (GPIOController::portB == MAP_DEVICE_FAILED) {
			cout << "port B mapping failed" << endl;
			return false;
		}

		return true;

		//TODO: Use in16(ctrlHandle) and out16(ctrlHandle) (or whatever bit size is necicarry to send stuff).

		//TODO: define pA_data = out, pB_data = in. call in8(pB_data), to get uint8_t. Mask to find pin #.
		//TODO; Look @ this to find pA and pB pg. 26 http://www.se.rit.edu/~swen-563/resources/helios/Helios%20User%20Manual.pdf
	}

	return false;
}




