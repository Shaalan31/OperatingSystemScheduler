#pragma once

enum Status
{
	running,
	blocked,
	ready
};

typedef struct 
{
	int arrivalTime;
	int cpuTime;//remainingBlockTime+runTime
	//int remainingBlockTime;
	//int runTime;
	int finishTime;
	int turnAround;
	float weightedTurnAround;
	int size;
	int id;
	int priority;
	Status status;
	int remainingTime;
	bool sigStopped;
	int howLong;
	int waitingTime;
	int uniqueNumber;
	bool flag;
	bool toBeSwitched;
}Process;
