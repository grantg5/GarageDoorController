/**
 * Thanks to Zachary Campanella & Emily Filmer for help and
 * for explaining the hardware component of this project in simple terms.
 *
 * Thanks to Ronnie James Dio for inspiring digital input/output, and for guiding young JB in
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


#define DIO_A 0x288	//input
#define DIO_B 0x289	//output
#define DIO_CTRL 0x28B

#define CHECK_BIT(var,pos) !!((var) & (1<<(pos)))

uintptr_t GPIOController::ctrl;
uintptr_t GPIOController::portA;
uintptr_t GPIOController::portB;
int GPIOController::portAVal = 0x0;
int GPIOController::portBVal = 0x0;

bool GPIOController::init() {
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

		//TODO; Look @ this to find pA and pB pg. 26 http://www.se.rit.edu/~swen-563/resources/helios/Helios%20User%20Manual.pdf
	}

	return false;
}

void GPIOController::scanA() {
	if (!Context::simulation) {
		GPIOController::portAVal = in8(GPIOController::portA);
	}
}

Event GPIOController::translateInput() {
	if (CHECK_BIT(GPIOController::portAVal, 0) == 1) {
		return Event('o', "FullyOpen");
	}
	if (CHECK_BIT(GPIOController::portAVal, 1) == 1) {
		return Event('c', "FullyClosed");
	}
	if (CHECK_BIT(GPIOController::portAVal, 2) == 1) {
		return Event('i', "InfraredBeam");
	}
	if (CHECK_BIT(GPIOController::portAVal, 3) == 1) {
		return Event('m', "Overcurrent");
	}
	if (CHECK_BIT(GPIOController::portAVal, 4) == 1) {
		return Event('r', "ButtonPress");
	}

	//create no event that will be ignored further in execution
	return Event('n', "NoEvent");
}

void GPIOController::raiseDoor() {
	GPIOController::portBVal |= (1u << 0); //Setting pin 9 high
	GPIOController::portBVal &= ~(1u << 1); //Setting pin 10 low
	Context::motorUp = true;
}

void GPIOController::lowerDoor() {
	GPIOController::portBVal &= ~(1u << 0); //Setting pin 9 low
	GPIOController::portBVal |= (1u << 1); //Setting pin 10 high
	Context::motorDown = true;

	GPIOController::portBVal |= (1u << 2); //Setting pin 11 high (for the beam)
	Context::infraredBeam = true;
}

void GPIOController::stopDoor() {
	GPIOController::portBVal &= ~(1u << 0); //Setting pin 9 low
	GPIOController::portBVal &= ~(1u << 1); //Setting pin 10 low
	Context::motorUp = false;
	Context::motorDown = false;

	GPIOController::portBVal &= ~(1u << 2); //Setting pin 11 low (for the beam)
	Context::infraredBeam = false;
}



