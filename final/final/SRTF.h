#pragma once
#include"Scheduler.h"

class SRTF :public Scheduler {
protected:
	//Node* runningProcess;
	//Node* processToRun;
	Node* contextSwitch0;
	Node* contextSwitch1;
public:
	SRTF(CPU cpus[], Dispatcher *d);
	virtual void contextSwitching(int penalty);
	virtual void algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty);
	void bubbleSort();
};