#include "IO.h"
IO::IO()
{
	keyMsgQ = msgget(0x60, IPC_CREAT | 0777); // or msgget(12613, IPC_CREATE | 0644)
	keyReceive = msgget(0x122, IPC_CREAT | 0777);
	if (keyMsgQ == -1)
	{
		//perror("Error in create");
		exit(-1);
	}
	timeID = shmget(0x200, 1000, IPC_CREAT | 0777);
	time = getTime(timeID);
	pendingQ = new LinkedList;
	SearchFiles(pendingQ);
	// we will fill the linked list in the constructor
}

void IO::sendPending()
{	
	cout<<"*********************************"<<endl;
	cout<<"time : "<<*time<<endl;
	
	bool permission=true;

	cout<<"---Sending---"<<endl;
	cout<<"process : "<<pendingQ->getHead()->process.uniqueNumber <<" with arrival time :" << pendingQ->getHead()->process.arrivalTime<<" vs Time : "<<*time<<endl;
	while (pendingQ->getHead()!= NULL && pendingQ->getHead()->process.arrivalTime == *time && permission==true)
	{
		send.process = pendingQ->getHead()->process;
		
		send.mtype = (*time);
		(send.mtype)++;


		//cout<<"arrival time of process to be sent : "<<pendingQ->getHead()->process.arrivalTime<<endl;
		
		//send the head to the msg queue
		int x=msgsnd(keyMsgQ, &send, sizeof(Process), !IPC_NOWAIT);
		if(x==-1)
		{
			cout<<"sending error : "<<errno<<endl;

		}
		else
		{	
			cout<<"---sent---"<<endl;
			cout<<"process : "<<send.process.uniqueNumber <<" with arrival time :" << send.process.arrivalTime<<" vs Time : "<<*time<<endl;
		}
		pendingQ->deleteFromBeginning();
		if (pendingQ->getHead()->process.flag)
		{
			permission=false;
			break;
		}
	}
	//sleep(1);
}

bool IO::Input(const char* s, Process &P)
{
	ifstream file;
	file.open(s);
	string line2;
	string sub;
	int val;
	int arrivalTime;
	int cpuTime;//remainingBlockTime+runTime
	int runTime;
	int size;
	int priority;
	int uniqueNumber;
	while (!file.eof())
	{
		getline(file, line2);

		if (line2.length() != 0 && line2[0] != '#')
		{
			stringstream line1(line2);

			for (int i = 0; i < 2; i++) {
				if (i == 0)
					line1 >> sub;
				else
					line1 >> val;
			}
			
			if (sub == "size")
				P.size = val;
			else if (sub == "arrival")
				P.arrivalTime = val;
			else if (sub == "cpu")
				P.cpuTime = val;
			else if (sub == "priority")
				P.priority = val;

			P.remainingTime=P.cpuTime;
		}

	}
	if (!file) {
		return false;
	}
	else {
		return true;
	}
	file.close();
}

void IO::SearchFiles(LinkedList * Processes)
{
	int i = 1;
	Process P;
	char* buf = new char[1];
	char* filename = new char[15];
	
	while (true) {

		sprintf(buf, "%d", i);
		strcpy(filename, "process_");
		strcat(filename, buf);
		strcat(filename, ".txt");
		

		if (is_file_exist(filename))
		{
			cout << filename << endl;
			Input(filename, P);
			P.uniqueNumber=i;
			P.flag=false;
			Processes->insertAtEnd(P);
			//We found the file
		}
		else
		{
			P.flag=true;
			Processes->insertAtEnd(P);
			break;
		}

		i++;
	}
}
bool IO::is_file_exist(const char *fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

void IO::readConfiguration(const char * s, configuration &C)
{
	ifstream file;
	file.open(s);
	string line2;
	int count = 0;
	int alg;

	while (!file.eof()) {
		getline(file, line2);

		if (line2.length() != 0 && line2[0] != '#')
		{
			stringstream line1(line2);
			if (count == 0)
			{
				line1 >> alg;
				C.algo = (algorithm)alg;
				count++;
			}
			else if (count == 1)
			{
				line1 >> C.contextSwitching;
				count++;
			}
			else if (count == 2)
			{
				line1 >> C.quantum;
				count++;
			}
			else if (count == 3)
			{
				line1 >> C.transferRate;
				count++;
			}
			else
				break;
		}
	}
	file.close();
}
void IO::outputFile(Process P)
{
	ofstream output_f;
	char* buf = new char[1];
	sprintf(buf, "%d", P.uniqueNumber);
	char* filename = new char[15];
	strcpy(filename, "outputprocess_");
	strcat(filename, buf);
	strcat(filename, ".txt");

	output_f.open(filename, ios::out);

	output_f << "CPU Time " << P.cpuTime << endl;
	output_f << "Finish Time " << P.finishTime << endl;

	for (int i = 0; i < 3; i++)
		output_f << endl;
	output_f << "TurnAround Time  " << P.turnAround;

	for (int i = 0; i < 3; i++)
		output_f << endl;
		output_f << "Weighted TurnAround Time  "<< fixed << setprecision(3) << P.weightedTurnAround;
	//output_f << "Weighted TurnAround Time  " << P.weightedTurnAround;

	output_f.close();
}
void IO::recieveDead()
{
	struct msgbuff message;
	while (msgrcv(keyReceive, &message, sizeof(Process),0, 0777 | IPC_NOWAIT | MSG_NOERROR) != -1 && &message!=NULL)
	{
		outputFile(message.process);
	}
}
int* IO::getTime(int timeID)
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
IO::~IO()
{
}
