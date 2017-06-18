#pragma once
#include "Scheduler.h"
class RR : public Scheduler
{
	private:
		int Quantum;
		Node* contextSwitch0;
		Node* contextSwitch1;
		
	public:
		//RR();
		RR(CPU cpus[], Dispatcher *d,int Q);
		virtual void contextSwitching(int penalty);
		void contextSwitching();
		void decQuantum();
		virtual void algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty);
		~RR();
};
