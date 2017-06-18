#include "Scheduler.h"

Scheduler::Scheduler(CPU cpus[], Dispatcher* d)
{
	msgQueueID = msgget(0x60, IPC_CREAT | 0777); 
	msgQueueFinished = msgget(0x122, IPC_CREAT | 0777);
	CPUs[0] = cpus[0];
	CPUs[0].status = idle;
	CPUs[0].runningProcess=NULL;
	CPUs[0].switchPenalty=2;
	CPUs[1] = cpus[1];
	CPUs[1].status = idle;
	CPUs[1].runningProcess=NULL;
	CPUs[1].switchPenalty=2;
	dispatcher = d;
	exited = false;

}

bool Scheduler::receiveProcess(int &freeMemory)
{
	struct msgbuff message;
	bool rec = false ;
	if(*(dispatcher->getTime())>100)
			{
				exited=true;
			}
	while (msgrcv(msgQueueID, &message, sizeof(Process), *(dispatcher->getTime())+1 , 0777 | IPC_NOWAIT | MSG_NOERROR) != -1)
	{
		
		if(&message!=NULL)
		{	
			if (message.process.flag)
			{
				cout<<"Exited Receiving"<<endl;
				exited=true;
				break;
			}

			cout<<"received :)"<<endl;
			cout<<"-----------------"<<endl;
			cout<<"Process "<<message.process.uniqueNumber<< " at time : " <<*(dispatcher->getTime()) <<" with arrival time : "<<message.process.arrivalTime<<endl;
			cout<<"cpu Time : "<<message.process.cpuTime<<" size: "<<message.process.size<<" priority : "<<message.process.priority<<endl;
			if ( &(message.process) !=NULL && message.process.size <= freeMemory)
			{
				cout<<"Inserted into ReadyQueue"<<endl;
				ready.insertAtEnd(message.process);
				ready.Print();//khairy
				freeMemory -= message.process.size;
				rec =  true;
			}
			else
			{	cout<<"BlockedQueue"<<endl;
				cout<<"Inserted into BlockedQueue"<<endl;
				blockedMemory.insertAtEnd(message.process);
				//return false;
			}
		}
		
	}
	return rec;
}

//Finds Occupied Memory (Not Needed)
int Scheduler::calculateMemory()
{
	int sizeOccupied = 0;
	if (CPUs[0].status == _running)
	{
		sizeOccupied += CPUs[0].runningProcess->size;
	}
	if (CPUs[1].status == _running)
	{
		sizeOccupied += CPUs[1].runningProcess->size;
	}

	if (ready.getHead() == NULL)
	{
		return sizeOccupied;
	}
	Node* ptr = ready.getHead();

	while (ptr)
	{
		sizeOccupied += (ptr->process.size);
		ptr = ptr->next;
	}
	return sizeOccupied;
}

void Scheduler::checkTermination(int & index1, int & index2)
{
	if (CPUs[0].runningProcess != NULL && CPUs[0].runningProcess->remainingTime == 0 && CPUs[0].status==_running)
	{
		index1 = 0;
	}
	else
	{
		index1 = -1;
	}

	if (CPUs[1].runningProcess != NULL &&  CPUs[1].runningProcess->remainingTime == 0 && CPUs[1].status==_running)
	{
		index2 = 1;
	}
	else
	{
		index2 = -1;
	}
}

void Scheduler::endProcesses(Process ** finishedProcesses, int countFinished, int &freeMemory)
{
	int index1, index2;
	checkTermination(index1, index2);

	if (index1 == -1 && index2 == -1)
	{
		cout<<"nothing terminates"<<endl;
		return;
	}

	if (index1 == 0)
	{
		cout<<"finishing process at cpu 0 at time: "<<*(dispatcher->getTime())<<" Process num: "<<CPUs[0].runningProcess->uniqueNumber<<endl;
		dispatcher->terminateProcess(CPUs[0].runningProcess,freeMemory);
		struct msgbuff message;
		message.process=*(CPUs[0].runningProcess);
		int x=msgsnd(msgQueueFinished, &message, sizeof(Process), !IPC_NOWAIT);
		if(x==-1)
		{
			cout<<"error sending dead process with errno : "<<errno<<endl;
		}
		else
		{
			cout<<"process "<<message.process.uniqueNumber<<" is finished and sent with arrival time : "<<message.process.arrivalTime<<endl;
		}
		CPUs[0].runningProcess = NULL;
		if (ready.getHead() == NULL)
		{
			CPUs[0].status = idle;
			cout<<"cpu 0 is idle"<<endl;
		}
		else
		{
			CPUs[0].status = _contextSwitching;
			CPUs[0].switchPenalty=2;
			cout<<"cpu 0 is in contextSwitching mode"<<endl;
		}
	}
	if (index2 == 1)
	{
		cout<<"finishing process at cpu 1 at time: "<<*(dispatcher->getTime())<<endl;
		dispatcher->terminateProcess(CPUs[1].runningProcess, freeMemory);
		struct msgbuff message;
		message.process=*(CPUs[1].runningProcess);
		int x=msgsnd(msgQueueFinished, &message, sizeof(Process), !IPC_NOWAIT);
		if(x==-1)
		{
			cout<<"errno : "<<errno<<endl;
		}
		else
		{
			cout<<"process "<<message.process.uniqueNumber<<" is finished and sent with arrival time : "<<message.process.arrivalTime<<endl;
		}
		CPUs[1].runningProcess = NULL;

		if (ready.getHead() == NULL)
		{
			CPUs[1].status = idle;
			cout<<"cpu 1 is idle"<<endl;
		}
		else
		{
			CPUs[1].status = _contextSwitching;
			CPUs[1].switchPenalty=2;
			cout<<"cpu 1 is in contextSwitching mode"<<endl;
		}
	}
}

void Scheduler::unblockMemProcesses(int &freeMemory)
{
	Node* ptr = blockedMemory.getHead();
	if (ptr != NULL)
	{
		while (ptr)
		{
			if (freeMemory == 0)
			{
				break;
			}
			if (ptr->process.size <= freeMemory)
			{
				cout<<"in unblockmemory"<<endl;
				ready.insertAtEnd(ptr->process);
				freeMemory -= ptr->process.size;
				blockedMemory.deleteFromBeginning();
			}
			ptr = ptr->next;
		}
	}
}

void Scheduler::returnToReady(const int indexCPU, Process  *p)
{
	Process temp;
	temp = *(CPUs[indexCPU].runningProcess);
	CPUs[indexCPU].runningProcess = p;
	ready.insertAtEnd(temp);
}

void Scheduler::cpuDecPenalty()
{
	if (CPUs[0].status == _contextSwitching)
	{
		(CPUs[0].switchPenalty)--;
	}
	if (CPUs[1].status == _contextSwitching)
	{
		(CPUs[1].switchPenalty)--;
	}
}

void Scheduler::decRemainingTime()
{
	if (CPUs[0].status == _running && CPUs[0].runningProcess!=NULL)
	{
		(CPUs[0].runningProcess->remainingTime)--;
		cout<<"cpu 0 remainingTime : "<<CPUs[0].runningProcess->remainingTime<<endl;
		cout<< "CPU 0 runningProcess # "<<CPUs[0].runningProcess->uniqueNumber<<endl;
		cout<<" CPU 0 quantum remaining "<<CPUs[0].runningProcess->howLong<<endl;
	}
	if (CPUs[1].status == _running && CPUs[1].runningProcess!=NULL )
	{
		(CPUs[1].runningProcess->remainingTime)--;
		cout<<"cpu 1 remainingTime : "<<CPUs[1].runningProcess->remainingTime<<endl;
		cout<< "CPU 1 runningProcess # "<<CPUs[1].runningProcess->uniqueNumber<<endl;
		cout<<" CPU 1 quantum remaining "<<CPUs[1].runningProcess->howLong<<endl;
	}
}

int Scheduler::min(int a, int b)
{
	return !(b < a) ? a : b;
}

double Scheduler::getAverageRunTime()
{
	//get average run time
	Node* ptr = ready.getHead();
	int sumRunTime = 0;
	while (ptr)
	{
		sumRunTime += ptr->process.cpuTime;
		ptr = ptr->next;
	}
	return sumRunTime / ready.getCount();
}

double Scheduler::getCV(double avgRunTime)
{
	Node *ptr = ready.getHead();
	double sumDeviation = 0;
	while (ptr)
	{
		sumDeviation += pow((ptr->process.cpuTime - avgRunTime), 2);
		ptr = ptr->next;
	}
	double standardDeviation = sqrt(sumDeviation / ready.getCount());
	return (standardDeviation / avgRunTime);
}

bool Scheduler::getExited()
{
	return exited;
}

void Scheduler::printCPUsDetails()
{

	if (CPUs[0].runningProcess!=NULL)
	{
		cout<<endl<<endl<<endl<<endl;
		cout<<"--CPU 0 Details--"<<endl;
		cout<<"Process "<<CPUs[0].runningProcess->uniqueNumber<<" with remaining time "<<CPUs[0].runningProcess->remainingTime<<endl;
	}
	if (CPUs[1].runningProcess!=NULL)
	{
		cout<<endl<<endl<<endl<<endl;
		cout<<"--CPU 1 Details--"<<endl;
		cout<<"Process "<<CPUs[1].runningProcess->uniqueNumber<<" with remaining time "<<CPUs[1].runningProcess->remainingTime<<endl;
		cout<<endl<<endl<<endl<<endl;
	}
}
