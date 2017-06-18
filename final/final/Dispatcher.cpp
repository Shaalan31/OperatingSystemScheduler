#include "Dispatcher.h"

Dispatcher::Dispatcher(int t)
{
	//signal(SIGCHLD, sigChildHandler);
	timeID = t;
}
void Dispatcher::terminateProcess(Process * p1, int &freeMemory)
{
	if (p1 != NULL)
	{
		//kill(p1->id, SIGKILL);
		freeMemory += (p1->size);

		//calculations
		p1->finishTime = *(getTime());
		p1->turnAround = (p1->finishTime) - (p1->arrivalTime);

		if ((p1->cpuTime)!=0)
		p1->weightedTurnAround = ((float)(p1->turnAround) )/ (p1->cpuTime);
		p1->waitingTime = (p1->finishTime) - (p1->arrivalTime) - (p1->cpuTime);
		cout<<"Process "<<p1->uniqueNumber<<" finished at : "<<p1->finishTime<<" with turnAround : "<<p1->turnAround<<endl<<" and weightedTurnAround: "<<p1->weightedTurnAround<<endl;
	}
}

void Dispatcher::contextSwitch(Process * toRun, Process * toStop)
{
	//kill(toStop->id, SIGSTOP);
	toStop->sigStopped = true;
	run(toRun);
}

void Dispatcher::stopProcess(Process *toStop)
{
	//kill(toStop->id, SIGSTOP);
	toStop->sigStopped = true;
}

void Dispatcher::run(Process * p)
{
	if (p!=NULL && p->remainingTime == p->cpuTime)
	{
		//fork
		int pid = fork();
		if (pid == 0)
		{
			char buffer[50];
			sprintf(buffer, "%d", p->cpuTime);

			char* argv[] = { "process",buffer,0 };
			execve("process", &argv[0], NULL);
		}
		p->id = pid;
	}
	else if (p!=NULL && p->sigStopped)
	{
		kill(p->id, SIGCONT);
	}
}

int Dispatcher::getTimeID()
{
	return timeID;
}

int* Dispatcher::getTime()
{
	int *time = (int*)shmat(timeID, (void *)0, 0); //in I/O put SHM_RDONLY as a flag
	if ((long)time == -1)
	{
		return NULL;
	}
	else
	{
		return time;
	}
}
void Dispatcher::sigChildHandler(int sigNum)
{
	//terminateProcess();
}
void Dispatcher::swapProcesses(Process & A, Process & B)
{
	Process temp = A;
	A = B;
	B = temp;
}
