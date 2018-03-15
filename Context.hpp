#include "Event.hpp"
#include "StateTable.hpp"
#include <queue>

class Context {
	public:
		static bool simulation;
		static bool motorUp;
		static bool motorDown;
		static bool doorOpen;
		static bool doorClosed;
		static bool infraredBeam;
		static int position;
		static bool toRaise;
		static StateTable stateTable;
		static queue<Event> * contextQueue;

		Context();
		static void * run(void *arg);
};
