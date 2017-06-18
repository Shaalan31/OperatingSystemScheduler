#pragma once
#include "LinkedList.h"
#include "Process.h"
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cerrno>
using namespace std;

struct msgbuff
{
	long mtype;
	Process process;
};

enum algorithm
{
	SRTN,
	_RR,
	_HPF,
	Custom
};

struct configuration
{
	algorithm algo;
	int contextSwitching;
	int quantum;
	int transferRate;
};

class IO
{
private:
	LinkedList *pendingQ;
	int* time;
	int keyTime;
	key_t keyMsgQ;
	key_t keyReceive;
	struct msgbuff send;
	int timeID;
	// we might need to store also the ID for faster communication
public:
	IO();
	void sendPending();
	int* getTime(int timeID);
	bool Input(const char* s, Process &P);
	void SearchFiles(LinkedList * Processes);
	bool is_file_exist(const char *fileName);
	void readConfiguration(const char* s, configuration &);

	void outputFile(Process P);
	void recieveDead();


	~IO();
};
