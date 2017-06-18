#include "SRTF.h"

SRTF::SRTF(CPU cpus[], Dispatcher *d) :Scheduler(cpus, d)
{
	contextSwitch0=NULL;
	contextSwitch1=NULL;
}

void SRTF::contextSwitching(int penalty)
{
	Node*ptr = ready.getHead();
	if (ptr != NULL)
	{
		while (ptr)
		{
			cout<<"Process "<<ptr->process.uniqueNumber<<" with remaining time: "<<ptr->process.remainingTime<<" checking if to context switch "<<endl;
			printCPUsDetails();
			if (CPUs[0].runningProcess != NULL && (CPUs[0].status == _running) && ptr->process.remainingTime < CPUs[0].runningProcess->remainingTime && contextSwitch0==NULL)
			{
				cout<<"**contextSwitch in CPU 0**"<<endl;
				CPUs[0].status = _contextSwitching;
				CPUs[0].switchPenalty=penalty;
				contextSwitch0=new Node;
				contextSwitch0=ready.deleteFromBeginning();
				cout<<"process "<<contextSwitch0->process.uniqueNumber<<" to be switched and run "<<endl;
				dispatcher->stopProcess(CPUs[0].runningProcess);
			}
			else if (CPUs[1].runningProcess != NULL && (CPUs[1].status == _running) && ptr->process.remainingTime < CPUs[1].runningProcess->remainingTime && contextSwitch1==NULL)
			{
				cout<<"**contextSwitch in CPU 1**"<<endl;
				CPUs[1].status = _contextSwitching;
				CPUs[1].switchPenalty=penalty;
				contextSwitch1=new Node;
				contextSwitch1=ready.deleteFromBeginning();
				cout<<"process "<<contextSwitch1->process.uniqueNumber<<"to be switched and run "<<endl;
				dispatcher->stopProcess(CPUs[1].runningProcess);
			}
			else
			{
				break;
			}
			ptr=ready.getHead();
		}
	}
	else
	{
		cout<<"-head is NULL in contextSwitch!"<<endl;
	}
}

void SRTF::algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty)
{
	if (getExited())
	{
		return;
	}
	cpuDecPenalty();
	//cout<<"penalty decremented"<<endl;
	unblockMemProcesses(freeMemory);
	bool received = receiveProcess(freeMemory);
	if (received)
	{
		bubbleSort();
	}
	cout<<"ready details after sorting"<<endl;
	ready.Print();
	printCPUsDetails();
	if (CPUs[0].runningProcess!=NULL && contextSwitch0!=NULL && CPUs[0].switchPenalty == 0 && CPUs[0].status==_contextSwitching )
	{
		cout<<"swapping in CPU 0"<<endl;
		returnToReady(0,&(contextSwitch0->process));
		dispatcher->run(CPUs[0].runningProcess);
		CPUs[0].status = _running;
		CPUs[0].switchPenalty = penalty;
		contextSwitch0=NULL;
		printCPUsDetails();
	}
	if (CPUs[1].runningProcess!=NULL && contextSwitch1!=NULL && CPUs[1].switchPenalty == 0 && CPUs[1].status==_contextSwitching)
	{
		cout<<"swapping in CPU 1"<<endl;
		returnToReady(1,&(contextSwitch1->process));
		dispatcher->run(CPUs[1].runningProcess);
		CPUs[1].status = _running;
		CPUs[1].switchPenalty = penalty;
		contextSwitch1=NULL;
		printCPUsDetails();
	}

	endProcesses(finishedProcesses, countFinished, freeMemory);

	if (ready.getHead() != NULL)
	{
		//cout<<"head of ready not equal NULL"<<endl;
		if ( CPUs[0].switchPenalty == 0 || (CPUs[0].status == idle) ) 
		{
			//cout<<"before checking on runningProcess in CPU 0 if it's null"<<endl;
			if (CPUs[0].runningProcess == NULL)
			{
				//cout<<"trying to insert at cpu 0"<<endl;
				dispatcher->run(&(ready.getHead()->process));
				CPUs[0].runningProcess = &(ready.getHead()->process);
				cout<<"Process "<<CPUs[0].runningProcess->uniqueNumber<<" is inserted into CPU 0 "<<endl;
				ready.deleteFromBeginning();
				cout<<"ready details after deletion"<<endl;
				ready.Print();
				CPUs[0].status = _running;
				CPUs[0].switchPenalty = penalty;
			}
		}
		if (ready.getHead() != NULL)
		{
			if (CPUs[1].switchPenalty == 0 || (CPUs[1].status == idle))
			{
				//cout<<"before checking on runningProcess in CPU 1 if it's null"<<endl;
				if (CPUs[1].runningProcess == NULL)
				{
					//cout<<"trying to insert at cpu 1"<<endl;
					dispatcher->run(&(ready.getHead()->process));
					CPUs[1].runningProcess = &(ready.getHead()->process);
					cout<<"Process "<<CPUs[1].runningProcess->uniqueNumber<<" is inserted into CPU 1 "<<endl;
					ready.deleteFromBeginning();
					cout<<"ready details after deletion"<<endl;
					ready.Print();
					CPUs[1].status = _running;
					CPUs[1].switchPenalty = penalty;
				}
			}
		}
		if (received)
		{
			cout<<"checking context switching.."<<endl;
			contextSwitching(penalty);
		}
		printCPUsDetails();
	}
	//cout<<"changing status of CPUs"<<endl;
	if (contextSwitch0==NULL && ready.getHead()==NULL && CPUs[0].runningProcess==NULL)
	{
		cout<<"CPU 0 is now idle"<<endl;
		CPUs[0].status=idle;
	}
	if (contextSwitch1==NULL && ready.getHead()==NULL && CPUs[1].runningProcess==NULL)
	{
		cout<<"CPU 1 is now idle"<<endl;
		CPUs[1].status=idle;
	}
	//cout<<"changed statuses successfully"<<endl;
	decRemainingTime();
	//cout<<"remainingTime decremented"<<endl;
}

void SRTF::bubbleSort()
{
	ready.bubbleSort(1);
}