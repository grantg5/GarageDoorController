#ifndef GPIOCONTROLLER_H_
#define GPIOCONTROLLER_H_

class GPIOController {
public:
	static bool fetchPermission();
	static bool mapPorts();
};

#endif /* GPIOCONTROLLER_H_ */
