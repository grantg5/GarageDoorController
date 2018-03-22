#include "Event.hpp"
#include <stdint.h>

#ifndef GPIOCONTROLLER_H_
#define GPIOCONTROLLER_H_

class GPIOController {
public:
	static uintptr_t ctrl;
	static uintptr_t portA;
	static uintptr_t portB;
	static int portAVal;
	static int portBVal;

	static bool init();
	static bool fetchPermission();
	static bool mapPorts();
	static void scanA();
	static Event translateInput();
	static void raiseDoor();
	static void lowerDoor();
	static void stopDoor();
};

#endif /* GPIOCONTROLLER_H_ */
