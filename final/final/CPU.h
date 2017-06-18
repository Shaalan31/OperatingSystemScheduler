#pragma once
#include"Process.h"

enum cpuStatus {
	_running,
	idle,
	_contextSwitching
};
struct CPU
{
	Process *runningProcess;
	cpuStatus status;
	int switchPenalty;
};
