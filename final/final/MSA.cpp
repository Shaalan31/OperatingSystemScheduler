#include "MSA.h"

MSA::MSA(CPU cpus[], Dispatcher * d) :Scheduler(cpus, d)
{
}

void MSA::contextSwitching()
{
	Node*ptr = ready.getHead();
	if (ptr != NULL)
	{
		if (CPUs[0].runningProcess != NULL && (CPUs[0].status = _running) && CPUs[0].runningProcess->howLong == 0)
		{
			CPUs[0].status = _contextSwitching;
			returnToReady(0, &(ptr->process));
			dispatcher->contextSwitch(CPUs[0].runningProcess, &(ptr->process));
			ptr = ptr->next;
		}
	}
	if (ptr != NULL)
	{
		if (CPUs[1].runningProcess != NULL && (CPUs[1].status = _running) && CPUs[0].runningProcess->howLong == 0)
		{
			CPUs[1].status = _contextSwitching;
			returnToReady(1, &(ptr->process));
			dispatcher->contextSwitch(CPUs[1].runningProcess, &(ptr->process));
			ptr = ptr->next;
		}
	}
}

void MSA::SJFAlgo(Process ** finishedProcesses, int & countFinished, int & freeMemory, int & penalty, bool received)
{
	if (getExited())
	{
		return;
	}
	if (received)
	{
		bubbleSort();
	}
	endProcesses(finishedProcesses, countFinished, freeMemory);
	if (ready.getHead() != NULL)
	{
		if (CPUs[0].switchPenalty == 0 || (CPUs[0].status == idle))
		{
			if (CPUs[0].runningProcess == NULL)
			{
				dispatcher->run(&(ready.getHead()->process));
				CPUs[0].runningProcess = &(ready.getHead()->process);
				ready.deleteFromBeginning();
			}
			CPUs[0].status = _running;
			CPUs[0].switchPenalty = penalty;
		}
		if (ready.getHead() != NULL)
		{
			if (CPUs[1].switchPenalty == 0 || (CPUs[1].status == idle))
			{
				if (CPUs[1].runningProcess == NULL)
				{
					dispatcher->run(&(ready.getHead()->process));
					CPUs[1].runningProcess = &(ready.getHead()->process);
					ready.deleteFromBeginning();
				}
				CPUs[1].status = _running;
				CPUs[1].switchPenalty = penalty;
			}
		}
		decRemainingTime();
	}
}

void MSA::RRAlgo(Process ** finishedProcesses, int & countFinished, int & freeMemory, int & penalty)
{
	if (getExited())
	{
		return;
	}
	if (ready.getHead() != NULL)
	{
		if (CPUs[0].switchPenalty == 0 || (CPUs[0].status == idle))
		{
			if (CPUs[0].runningProcess == NULL)
			{
				int Quantum; //we will use it for now
				dispatcher->run(&(ready.getHead()->process));
				ready.getHead()->process.howLong = min(Quantum, ready.getHead()->process.remainingTime);
				CPUs[0].runningProcess = &(ready.getHead()->process);
				ready.deleteFromBeginning();
			}
			CPUs[0].status = _running;
			CPUs[0].switchPenalty = penalty;
		}
		if (ready.getHead() != NULL)
		{
			if (CPUs[1].switchPenalty == 0 || (CPUs[1].status == idle))
			{
				if (CPUs[1].runningProcess == NULL)
				{
					int Quantum;
					dispatcher->run(&(ready.getHead()->process));
					ready.getHead()->process.howLong = min(Quantum, ready.getHead()->process.remainingTime);
					CPUs[1].runningProcess = &(ready.getHead()->process);
					ready.deleteFromBeginning();
				}
				CPUs[1].status = _running;
				CPUs[1].switchPenalty = penalty;
			}
		}
		contextSwitching();
	}
	decRemainingTime();
}

void MSA::whichAlgorithm(int &selection)
{
	int count = ready.getCount();
	if (count == 0)
	{
		selection = -1;
	}
	else if (count < 3)
	{
		selection = 0;
	}
	else
	{
		double avgRunTime = getAverageRunTime();
		double CV = getCV(avgRunTime);
		if (CV >= 1)
		{
			selection = 0;
		}
		else
		{
			selection = 1;
		}
	}
}

void MSA::bubbleSort()
{
	ready.bubbleSort(2);
}

void MSA::algorithm(Process ** finishedProcesses, int & countFinished, int & freeMemory, int & penalty)
{
	cpuDecPenalty();
	unblockMemProcesses(freeMemory);
	bool received = receiveProcess(freeMemory);
	int selection;
	whichAlgorithm(selection);
	switch (selection)
	{
	case 0:
		//sjf
		SJFAlgo(finishedProcesses, countFinished, freeMemory, penalty, received);
		break;
	case 1:
		//RR
		RRAlgo(finishedProcesses, countFinished, freeMemory, penalty);
		break;
	case -1:
		break;
	}
}
