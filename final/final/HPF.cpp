#include "HPF.h"
HPF::HPF(CPU cpus[], Dispatcher * d) :Scheduler(cpus, d)
{
}
void HPF::algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty)
{
	if (getExited())
	{
		return;
	}
	cpuDecPenalty();
	unblockMemProcesses(freeMemory);
	receiveProcess(freeMemory);
	cout<<"Count of ready queue:"<<ready.getCount()<<endl;
	decRemainingTime();
	endProcesses(finishedProcesses, countFinished, freeMemory);
	cout<<"Count of ready queue before check on null:"<<ready.getCount()<<endl;
	if(ready.getHead() == NULL)
	{
		cout<<"Count of ready queue inside check on null:"<<ready.getCount()<<endl;
			cout<<"Head is NULL"<<endl;
	}

	if (ready.getHead() != NULL)
	{
		if (CPUs[0].switchPenalty == 0 || (CPUs[0].status == idle))
		{
			bubbleSort();
			dispatcher->run(&(ready.getHead()->process));
			CPUs[0].runningProcess = &(ready.deleteFromBeginning()->process);
			CPUs[0].switchPenalty = penalty;
			CPUs[0].status = _running;
		}

		if(ready.getHead() == NULL)
		{
			//cout<<"Head is NULL"<<endl;
		}
		if (ready.getHead() != NULL)
		{
			cout<<"CPU 1 Switch Penalty"<<CPUs[1].switchPenalty<<"Cpu 1 Status"<<CPUs[1].status<<endl;
			if (CPUs[1].switchPenalty == 0 || (CPUs[1].status == idle))
			{
				cout<<"in cpu 1"<<endl;
				bubbleSort();
				dispatcher->run(&(ready.getHead()->process));
				CPUs[1].runningProcess = &(ready.deleteFromBeginning()->process);
				CPUs[1].status = _running;
				CPUs[1].switchPenalty = penalty;
			}
		}
	}

	//cout<<"cpu0 penalty"<<CPUs[0].switchPenalty<<endl;
	//cout<<"cpu1 penalty"<<CPUs[1].switchPenalty<<endl;
	//cout<<"FreeMemory"<<freeMemory<<endl;
	//if(CPUs[0].status==_running)
	//cout<<"inside algo:"<<"CPU0 Process "<<CPUs[0].runningProcess->uniqueNumber<<" RemTime: "<<CPUs[0].runningProcess->remainingTime<<endl;
	//if(CPUs[1].status==_running)
	//cout<<"inside algo:"<<"CPU1 Process"<<CPUs[1].runningProcess->uniqueNumber<<"RemTime: "<<CPUs[1].runningProcess->remainingTime<<endl;
	
}
void HPF::bubbleSort()
{
	ready.bubbleSort(0);
}
void HPF::contextSwitching()
{
	int x = 0;
	return;
}
HPF::~HPF()
{
}