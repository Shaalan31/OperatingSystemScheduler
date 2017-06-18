#include <iostream>
#include "SRTF.h"
#include "HPF.h"
#include "RR.h"
#include "LinkedList.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "semaphore.h"
using namespace std;

int* getTime(int);
void outputFile(Process P);
void syncTime(int myID, int yourID, int* Time);
struct msgbuff3
{
	long type;
	int Time;
};
int main()
{
	int timeID = shmget(0x200, 1000, IPC_CREAT | 0777);
	int *clock = getTime(timeID);
	//*clock=0;
	//Needed initializations
	int countFinished = 0;
	int penalty = 2;
	int freeMemory = 5000;
	CPU CPUs[2];
	CPUs[0].switchPenalty = 2;
	CPUs[1].switchPenalty = 2;
	Dispatcher *d = new Dispatcher(timeID);
	Process **finishedProcesses = new Process*[4];
	Scheduler *scheduler = new RR(CPUs, d , 5);
	//sleep(1);
	//int count=0;

	int sem_z = semget(60, 1, 0666|IPC_CREAT);
	int sem_y = semget(80, 1, 0666|IPC_CREAT);
  	union semun Semun;

  	if(sem_z == -1)
  	{
  	  perror("Error in getting semaphore");
  	  exit(-1);
  	}
	Semun.val = 1;
  	if(semctl(sem_z, 0, SETVAL, Semun) == -1)
  	{
  	  perror("Error in semctl");
  	  exit(-1);
  	}
  	sleep(2);
	while (true)
	{
		down(sem_y);
		cout<<"-------------------------"<<endl;
		cout<<"-------------------------"<<endl;
		cout <<"clock : "<<*clock<< endl;
		scheduler->algorithm(finishedProcesses, countFinished, freeMemory, penalty);
		if (scheduler->getExited())
		{
			break;
		}
		up(sem_z);
		sleep(1);
	}
	(*clock)=0;
}



/*void syncTime(int myID, int yourID, int* Time)
{
	struct msgbuff3 something;
	int x;
	key_t myQ;
	key_t yourQ;
	something.Time = *Time;
	something.type = myID;
	myQ = msgget(myID, IPC_CREAT | 0777);
	x = msgsnd(myQ, &something, sizeof(int), !IPC_NOWAIT);
	yourQ = msgget(yourID, IPC_CREAT | 0777);
	msgrcv(yourQ, &something, sizeof(int), yourID, 0777 | !IPC_NOWAIT );
	return;

}*/

int* getTime(int timeID)
{
	int *_time = (int*)shmat(timeID, (void *)0, 0); //in I/O put SHM_RDONLY as a flag
	if ((long)_time == -1)
	{
		return NULL;
	}
	else
	{
		return _time;
	}
}


void printCPUsDetails(CPU CPUs[])
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