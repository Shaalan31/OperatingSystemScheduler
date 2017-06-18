#include "RR.h"
RR::RR(CPU cpus[], Dispatcher *d,int Q) :Scheduler(cpus, d)
{
	Quantum=Q;
}

void RR::contextSwitching(int penalty)
{
	Node*ptr = ready.getHead();
	if (ptr != NULL)
	{
			cout<<"Process "<<ptr->process.uniqueNumber<<" with remaining time: "<<ptr->process.remainingTime<<" checking if to context switch "<<endl;
			printCPUsDetails();
			if (CPUs[0].runningProcess != NULL && (CPUs[0].status == _running) && CPUs[0].runningProcess->howLong == 0 && contextSwitch0==NULL)
			{
				cout<<"**contextSwitch in CPU 0**"<<endl;
				CPUs[0].status = _contextSwitching;
				CPUs[0].switchPenalty=penalty;
				contextSwitch0=new Node;
				contextSwitch0=ready.deleteFromBeginning();
				cout<<"process "<<contextSwitch0->process.uniqueNumber<<" to be switched and run "<<endl;
				dispatcher->stopProcess(CPUs[0].runningProcess);
				ptr = ptr->next;
			}

			if(CPUs[0].runningProcess != NULL && CPUs[0].runningProcess->howLong == 0)
			{
				CPUs[0].runningProcess->howLong = min(Quantum, CPUs[0].runningProcess->remainingTime);
			}

			if(ptr!=NULL)
			{
				if (CPUs[1].runningProcess != NULL && (CPUs[1].status == _running) && CPUs[1].runningProcess->howLong == 0 && contextSwitch1==NULL)
				{
					cout<<"**contextSwitch in CPU 1**"<<endl;
					CPUs[1].status = _contextSwitching;
					CPUs[1].switchPenalty=penalty;
					contextSwitch1=new Node;
					contextSwitch1=ready.deleteFromBeginning();
					cout<<"process "<<contextSwitch1->process.uniqueNumber<<"to be switched and run "<<endl;
					dispatcher->stopProcess(CPUs[1].runningProcess);
				}

				if(CPUs[1].runningProcess != NULL && CPUs[1].runningProcess->howLong == 0)
				{
				CPUs[1].runningProcess->howLong = min(Quantum, CPUs[1].runningProcess->remainingTime);
				}
			}
		
	}
	else
	{
		//cout<<"Is it too late now to say sorry "<<endl;
		if(CPUs[0].runningProcess != NULL && CPUs[0].runningProcess->howLong == 0)
		{
			CPUs[0].runningProcess->howLong = min(Quantum, CPUs[0].runningProcess->remainingTime);
		}

		if(CPUs[1].runningProcess != NULL && CPUs[1].runningProcess->howLong == 0)
		{
				CPUs[1].runningProcess->howLong = min(Quantum, CPUs[1].runningProcess->remainingTime);
		}
		cout<<"-head is NULL in contextSwitch!"<<endl;
	}
}


void RR::contextSwitching()
{
	Node*ptr = ready.getHead();
	if (ptr != NULL)
	{
		if (CPUs[0].runningProcess != NULL && (CPUs[0].status == _running) && CPUs[0].runningProcess->howLong == 0)
		{
			CPUs[0].status = _contextSwitching;
			returnToReady(0, &(ptr->process));
			dispatcher->contextSwitch(CPUs[0].runningProcess, &(ptr->process));
			ptr = ptr->next;
		}
	}
	if (ptr != NULL)
	{
		if (CPUs[1].runningProcess != NULL && (CPUs[1].status == _running) && CPUs[0].runningProcess->howLong == 0)
		{
			CPUs[1].status = _contextSwitching;
			returnToReady(1, &(ptr->process));
			dispatcher->contextSwitch(CPUs[1].runningProcess, &(ptr->process));
			ptr = ptr->next;
		}
	}
}

void RR::decQuantum()
{
	if(CPUs[0].runningProcess != NULL )
		{
			(CPUs[0].runningProcess->howLong)--;;
		}

		if(CPUs[1].runningProcess != NULL )
		{
			(CPUs[1].runningProcess->howLong)--;;
		}
}
/*
void RR::algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty)
{
	if (getExited())
	{
		cout<<"lol \n";
		return;
	}
	cpuDecPenalty();
	cout<<"penalty decremented"<<endl;
	receiveProcess(freeMemory);
	endProcesses(finishedProcesses, countFinished, freeMemory);
	unblockMemProcesses(freeMemory);

	if (ready.getHead() != NULL)
	{
		if (CPUs[0].switchPenalty == 0 || (CPUs[0].status == idle))
		{
			if (CPUs[0].runningProcess == NULL)
			{
				//int Quantum; //we will use it for now
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
					//int Quantum;
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
	printCPUsDetails();
}
*/

void RR::algorithm(Process **finishedProcesses, int &countFinished, int &freeMemory, int &penalty)
{
	if (getExited())
	{
		cout<<"lol \n";
		return;
	}
	cpuDecPenalty();
	
	//cout<<"penalty decremented"<<endl;
	unblockMemProcesses(freeMemory);
	receiveProcess(freeMemory);
	cout<<"ready details"<<endl;
	ready.Print();
	
	if (CPUs[0].runningProcess!=NULL && contextSwitch0!=NULL && CPUs[0].switchPenalty == 0 && CPUs[0].status==_contextSwitching )
	{
		cout<<"swapping in CPU 0"<<endl;
		returnToReady(0,&(contextSwitch0->process));
		CPUs[0].runningProcess->howLong =  min(Quantum, CPUs[0].runningProcess->remainingTime)
		dispatcher->run(CPUs[0].runningProcess);
		CPUs[0].status = _running;
		CPUs[0].switchPenalty = penalty;
		contextSwitch0=NULL;
		//printCPUsDetails();
	}
	if (CPUs[1].runningProcess!=NULL && contextSwitch1!=NULL && CPUs[1].switchPenalty == 0 && CPUs[1].status==_contextSwitching)
	{
		cout<<"swapping in CPU 1"<<endl;
		returnToReady(1,&(contextSwitch1->process));
		CPUs[0].runningProcess->howLong =  min(Quantum, CPUs[0].runningProcess->remainingTime)
		dispatcher->run(CPUs[1].runningProcess);
		CPUs[1].status = _running;
		CPUs[1].switchPenalty = penalty;
		contextSwitch1=NULL;
		//printCPUsDetails();
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
				cout<<"trying to insert at cpu 0"<<endl;
				dispatcher->run(&(ready.getHead()->process));
				ready.getHead()->process.howLong = min(Quantum, ready.getHead()->process.remainingTime);
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
			cout<<"head was not null before so lets check on the next conditions "<<endl;
			if (CPUs[1].switchPenalty == 0 || (CPUs[1].status == idle))
			{
				cout<<"before checking on runningProcess in CPU 1 if it's null"<<endl;
				if (CPUs[1].runningProcess == NULL)
				{
					cout<<"trying to insert at cpu 1"<<endl<<endl<<":)"<<endl;
					dispatcher->run(&(ready.getHead()->process));
					ready.getHead()->process.howLong = min(Quantum, ready.getHead()->process.remainingTime);
					CPUs[1].runningProcess = &(ready.getHead()->process);
					cout<<"Process "<<CPUs[1].runningProcess->uniqueNumber<<" is inserted into CPU 1 "<<endl;
					ready.deleteFromBeginning();
					cout<<"ready details after deletion"<<endl;
					ready.Print();
					cout<<" TESTING IF Process 2 WAS INSERTED "<<endl<<CPUs[1].runningProcess.uniqueNumber<<endl;
					CPUs[1].status = _running;
					CPUs[1].switchPenalty = penalty;


					int x;
					cin>>x;
				}
			}
		}
		
		//printCPUsDetails();
	}


	// Removing from CPU if there is n
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
	decQuantum();
	decRemainingTime();
	cout<<endl;
	printCPUsDetails();
	contextSwitching(penalty);
	//cout<<"remainingTime decremented"<<endl;
}

RR::~RR()
{
	
}