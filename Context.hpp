#include "Event.hpp"
#include "StateTable.hpp"
#include <queue>

class Context {
	public:
		static bool motorUp;
		static bool motorDown;
		static bool infraredBeam;
		static int position;
		static bool toRaise;
		static StateTable stateTable;
		static queue<Event> contextQueue;

		Context();

		static void setupStateTable();
		//static void * run(void* arg);
		void moveDoor(bool);
		void acceptEvent(Event);
		static void * startTimer(void* arg);
};
