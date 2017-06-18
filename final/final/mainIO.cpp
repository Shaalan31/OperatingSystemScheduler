#include"IO.h"
#include <iostream>
#include "semaphore.h"

using namespace std;

//void syncTime(int myID, int yourID, int* Time);
struct msgbuff3
{
	long type;
	int Time;
};

int main()
{
	IO *inputOutput = new IO();
	int sem_y = semget(80, 1, 0666|IPC_CREAT);
	int sem_x = semget(40, 1, 0666|IPC_CREAT); //semaphore for time
  	union semun Semun;
  	if(sem_y == -1)
  	{
  	  perror("Error in getting semaphore");
  	  exit(-1);
  	}
	Semun.val = 0;
  	if(semctl(sem_y, 0, SETVAL, Semun) == -1)
  	{
  	  perror("Error in semctl");
  	  exit(-1);
  	}
  	if(semctl(sem_y, 0, SETVAL, Semun) == -1)
  	{
  	  perror("Error in semctl");
  	  exit(-1);
  	}
  	
	sleep(2);
	while (true)
	{
		down(sem_x);
		inputOutput->sendPending();
		inputOutput->recieveDead();
		//syncTime(9, 4, inputOutput->getTime(timeID));
		//sleep(0.5);
		up(sem_y);
		sleep(1);
	}
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

