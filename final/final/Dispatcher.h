#pragma once
#include <signal.h>
#include "IO.h"
#include "Process.h"
#include "LinkedList.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <string.h>
#define STDIN_FILENO   0
#define STDOUT_FILENO  1

class Dispatcher
{
private:
	int timeID;
public:
	Dispatcher(int timeID);
	void swapProcesses(Process &currentProcess, Process &switchingProcess);
	void terminateProcess(Process* p1, int &freeMemory);
	void contextSwitch(Process *toRun, Process *toStop);
	void stopProcess(Process *toStop);
	void run(Process *p);
	int getTimeID();
	int* getTime();
	void sigChildHandler(int sigNum);
};
