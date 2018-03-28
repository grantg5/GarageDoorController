#include "Event.hpp"
#include <stdint.h>

#ifndef GPIOCONTROLLER_H_
#define GPIOCONTROLLER_H_

class GPIOController {
public:
	uintptr_t ctrl;
	uintptr_t portA;
	uintptr_t portB;
	uint8_t portAVal;
	uint8_t portBVal;

	bool init();
	bool fetchPermission();
	bool mapPorts();
	void scanA();
	Event translateInput();
	void raiseDoor();
	void lowerDoor();
	void stopDoor();
};

#endif /* GPIOCONTROLLER_H_ */
