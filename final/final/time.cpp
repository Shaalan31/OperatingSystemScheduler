#include<iostream>
#include"SRTF.h"
#include"HPF.h"
#include"LinkedList.h"
#include"Process.h"
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
struct msgbuff3
{
	long type;
	int Time;
};
int main()
{
	int timeID = shmget(0x200, 1000, IPC_CREAT | 0777);
	int *clock = getTime(timeID);
	*clock=-1;
	//sleep(1);
	//int count=0;

	int sem_x = semget(40, 1, 0666|IPC_CREAT); //semaphore for time
  	int sem_z = semget(60, 1, 0666|IPC_CREAT);
  	union semun Semun;

  	if(sem_x == -1)
  	{
  	  perror("Error in creating semaphore");
  	  exit(-1);
  	}
  	Semun.val = 1;
  	if(semctl(sem_x, 0, SETVAL, Semun) == -1)
  	{
  	  perror("Error in semctl");
  	  exit(-1);
  	}
  	sleep(2);
	while (true)
	{
		down(sem_z);
		//while();
		cout<<"-------------------------"<<endl;
		cout<<"-------------------------"<<endl;
		cout <<"clock : "<<*clock<< endl;
		
		(*clock)= (*clock)+1;
		up(sem_x);
		sleep(1);
	}

}




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
