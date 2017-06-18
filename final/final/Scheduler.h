#pragma once
#include"LinkedList.h"
#include"Process.h"
#include"Dispatcher.h"
#include"CPU.h"
#include"IO.h"
#include <math.h>
#include <cerrno>
using namespace std;

class Scheduler
{
public:
	LinkedList ready;
	LinkedList blockedIO;
	LinkedList blockedMemory;
	key_t msgQueueID;
	key_t msgQueueFinished;
	Dispatcher* dispatcher;
	CPU CPUs[2];
	bool exited;
public:
	Scheduler(CPU cpus[], Dispatcher *d);
	virtual void contextSwitching(int penalty) = 0;
	virtual void algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty) = 0;
	bool receiveProcess(int & freeMemory);
	int calculateMemory();
	void checkTermination(int &index1, int &index2);
	void endProcesses(Process **finishedProcesses, int countFinished, int &freeMemory);
	void unblockMemProcesses(int &freeMemory);
	void returnToReady(const int indexCPU, Process *p);
	void cpuDecPenalty();
	void decRemainingTime();
	int min(int, int);
	double getAverageRunTime();
	double getCV(double avgRunTime);
	bool getExited();
	void printCPUsDetails();
};
