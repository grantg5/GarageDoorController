#include "Event.hpp"

class Context {
	public:
		bool motorUp;
		bool motorDown;
		bool infraredBeam;

		Context();

		void acceptEvent(Event);
		void startTimer(int);
};
