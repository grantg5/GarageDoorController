/**
 * Thanks to Zachary Campanella & Emily Filmer for help and
 * for explaining the hardware component of this project in simple terms.
 */

#include "GPIOController.hpp"
#include "Context.hpp"

#include <sys/neutrino.h>
#include <sys/mman.h>
#include <unistd.h>
#include <hw/inout.h>
#include <time.h>
#include <iostream>
using namespace std;

#define DIO_A 0x288	//input
#define DIO_B 0x289	//output
#define DIO_CTRL 0x28B //control

#define CHECK_BIT(var,pos) !!((var) & (1<<(pos)))
#define BOUNCE 0.2 // In seconds, how soon after receiving an input should a new input be accepted.

timespec openTimer;
timespec closeTimer;
timespec infraredTimer;
timespec motorTimer;
timespec pushButtonTimer;

bool GPIOController::init() {
	bool *contextSimulationPointer = &Context::simulation;
	bool contextSimulation = *contextSimulationPointer;

	if (!contextSimulation) {
		if (fetchPermission() && mapPorts()) {
			//Init-ing timers for bounce implementation
			clock_gettime(CLOCK_MONOTONIC, &openTimer);
			clock_gettime(CLOCK_MONOTONIC, &closeTimer);
			clock_gettime(CLOCK_MONOTONIC, &pushButtonTimer);
			clock_gettime(CLOCK_MONOTONIC, &infraredTimer);
			clock_gettime(CLOCK_MONOTONIC, &motorTimer);

			out8(ctrl, 0x91); //Setting control

			portAVal = 0x0;
			portBVal = 0x0;

			out8(portB, portBVal);
			sleep(1);
			portBVal |= (1u << 7); //Setting pin 16 high
			out8(portB, portBVal);

			return true;
		}
	}

	return false;
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
		ctrl = mmap_device_io(1, DIO_CTRL);
		portA = mmap_device_io(1, DIO_A);
		portB = mmap_device_io(1, DIO_B);

		if (ctrl == MAP_DEVICE_FAILED) {
			cout << "ctrl mapping failed." << endl;
			return false;
		}
		if (portA == MAP_DEVICE_FAILED) {
			cout << "port A mapping failed" << endl;
			return false;
		}
		if (portB == MAP_DEVICE_FAILED) {
			cout << "port B mapping failed" << endl;
			return false;
		}

		return true;
	}

	return false;
}

void GPIOController::scanA() {
	if (!Context::simulation) {
		portAVal = in8(portA);
	}
}

Event GPIOController::translateInput() {
	double timeElapsed;
	timespec tempTimer;
	clock_gettime(CLOCK_MONOTONIC, &tempTimer);

	if (CHECK_BIT(portAVal, 2) == 1) {
		//Bounce implementation used to prevent multiple signals from one button push.
		timeElapsed = (tempTimer.tv_sec - infraredTimer.tv_sec);
		timeElapsed += (tempTimer.tv_nsec - infraredTimer.tv_nsec) / 1000000000.0;

		if (timeElapsed >= BOUNCE) {
			clock_gettime(CLOCK_MONOTONIC, &infraredTimer);
			return Event('i', "InfraredBeam");
		}
	}
	if (CHECK_BIT(portAVal, 3) == 1) {
		timeElapsed = (tempTimer.tv_sec - motorTimer.tv_sec);
		timeElapsed += (tempTimer.tv_nsec - motorTimer.tv_nsec) / 1000000000.0;

		if (timeElapsed >= BOUNCE) {
			clock_gettime(CLOCK_MONOTONIC, &motorTimer);
			return Event('m', "Overcurrent");
		}
	}
	if (CHECK_BIT(portAVal, 4) == 1) {
		timeElapsed = (tempTimer.tv_sec - pushButtonTimer.tv_sec);
		timeElapsed += (tempTimer.tv_nsec - pushButtonTimer.tv_nsec) / 1000000000.0;

		if (timeElapsed >= BOUNCE) {
			clock_gettime(CLOCK_MONOTONIC, &pushButtonTimer);
			return Event('r', "ButtonPress");
		}
	}
	if (CHECK_BIT(portAVal, 0) == 1) {
		timeElapsed = (tempTimer.tv_sec - openTimer.tv_sec);
		timeElapsed += (tempTimer.tv_nsec - openTimer.tv_nsec) / 1000000000.0;

		if (timeElapsed >= BOUNCE) {
			clock_gettime(CLOCK_MONOTONIC, &openTimer);
			return Event('o', "FullyOpen");
		}
	}
	if (CHECK_BIT(portAVal, 1) == 1) {
		timeElapsed = (tempTimer.tv_sec - closeTimer.tv_sec);
		timeElapsed += (tempTimer.tv_nsec - closeTimer.tv_nsec) / 1000000000.0;

		if (timeElapsed >= BOUNCE) {
			clock_gettime(CLOCK_MONOTONIC, &closeTimer);
			return Event('c', "FullyClosed");
		}
	}

	//create no event that will be ignored further in execution
	return Event('n', "NoEvent");
}

void GPIOController::raiseDoor() {
	portBVal |= (1u << 0); //Setting pin 9 high
	portBVal &= ~(1u << 1); //Setting pin 10 low
	portBVal &= ~(1u << 2); //Setting pin 11 low (for the beam)
	out8(portB, portBVal);
	Context::motorUp = true;
	Context::infraredBeam = false;
}

void GPIOController::lowerDoor() {
	portBVal &= ~(1u << 0); //Setting pin 9 low
	portBVal |= (1u << 1); //Setting pin 10 high
	Context::motorDown = true;

	portBVal |= (1u << 2); //Setting pin 11 high (for the beam)
	Context::infraredBeam = true;
	out8(portB, portBVal);
}

void GPIOController::stopDoor() {
	portBVal &= ~(1u << 0); //Setting pin 9 low
	portBVal &= ~(1u << 1); //Setting pin 10 low
	Context::motorUp = false;
	Context::motorDown = false;

	portBVal &= ~(1u << 2); //Setting pin 11 low (for the beam)
	Context::infraredBeam = false;
	out8(portB, portBVal);
}



