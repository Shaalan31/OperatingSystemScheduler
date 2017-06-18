#pragma once
#include"Scheduler.h"
#include <cmath>

class MSA :public Scheduler {
protected:
public:
	MSA(CPU cpus[], Dispatcher *d);
	virtual void contextSwitching();
	void SJFAlgo(Process ** finishedProcesses, int & countFinished, int & freeMemory, int & penalty, bool received);
	void RRAlgo(Process ** finishedProcesses, int & countFinished, int & freeMemory, int & penalty);
	void whichAlgorithm(int &selection);
	virtual void algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty);
	void bubbleSort();
};