#include "Event.hpp"

class Context {
	public:
		bool motorUp;
		bool motorDown;
		bool infraredBeam;

		Context();

		static void * run(void* arg);
		void acceptEvent(Event);
		void startTimer(int);
};
