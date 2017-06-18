#include<iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
using namespace std;

int main(int argc, char **argv)
{
	int cpuTime = atoi(argv[1]);
	sleep(cpuTime);
}
