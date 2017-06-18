#pragma once
#include "Scheduler.h"
class HPF :public Scheduler
{
private:
	Process toStop;
	Process toStart;
	int time;
public:
	HPF(CPU cpus[], Dispatcher *d);
	virtual void algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty);
	void bubbleSort();
	virtual void contextSwitching();
	friend int* getTime(int timeID);
	~HPF();
};
