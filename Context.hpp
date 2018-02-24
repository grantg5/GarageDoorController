#include "Event.hpp"
#include "StateTable.hpp"

class Context {
	public:
		bool motorUp;
		bool motorDown;
		bool infraredBeam;
		StateTable stateTable;

		Context();

		void setupStateTable();
		static void * run(void* arg);
		void acceptEvent(Event);
		void startTimer(int);
};
