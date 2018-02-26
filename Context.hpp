#include "Event.hpp"
#include "StateTable.hpp"

class Context {
	public:
		static bool motorUp;
		static bool motorDown;
		static bool infraredBeam;
		static int position;
		static bool toRaise;
		static StateTable stateTable;

		Context();

		static void setupStateTable();
		//static void * run(void* arg);
		void acceptEvent(Event);
		static void * startTimer(void* arg);
};
