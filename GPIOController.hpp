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

	static void init();
	static bool fetchPermission();
	static bool mapPorts();
};

#endif /* GPIOCONTROLLER_H_ */
