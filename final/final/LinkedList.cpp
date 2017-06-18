#include "LinkedList.h"

LinkedList::LinkedList()
{
	head = NULL;
	tail = NULL;
	count = 0;
}

void LinkedList::insertAtEnd(Process p)
{
	Node* addedProcess = new Node;
	addedProcess->next = NULL;
	addedProcess->process = p;
	if (head == NULL && tail == NULL && count==0)
	{
		head = addedProcess;
		tail = addedProcess;
		count++;
		return;
	}
	tail->next = addedProcess;
	tail = addedProcess;
	count++;
	return;
}
void LinkedList::bubbleSort(int criteria)
{
	bool swapped = true;
	while (swapped)
	{
		swapped = false;
		Node* ptr = head;

		//sort by priority
		if (criteria == 0)
		{
			while (ptr && ptr->next)
			{
				if (ptr->process.priority < ptr->next->process.priority)
				{
					swapProcesses(ptr->process, ptr->next->process);
					swapped = true;
				}
				ptr = ptr->next;
			}
		}
		//sort by remaining time
		else if (criteria == 1)
		{
			while (ptr && ptr->next)
			{
				if (ptr->process.remainingTime > ptr->next->process.remainingTime)
				{
					swapProcesses(ptr->process, ptr->next->process);
					swapped = true;
				}
				ptr = ptr->next;
			}
		}
		else if (criteria == 2)
		{
			while (ptr && ptr->next)
			{
				if (ptr->process.cpuTime > ptr->next->process.cpuTime)
				{
					swapProcesses(ptr->process, ptr->next->process);
					swapped = true;
				}
				ptr = ptr->next;
			}
		}
	}
}

Node * LinkedList::getHead()
{
	if (count != 0)
	{
		return head;
	}
	return NULL;
}

Node * LinkedList::getTail()
{
	if (count != 0)
	{
		return tail;
	}
	return NULL;
}

void LinkedList::Print()
{
	Node *ptr = head;
	if (ptr==NULL)
	{
		return;
	}
	while (ptr)
	{
		cout<<"________________________________________________________"<<endl;
		cout<<"ready details"<<endl;
		cout<<"Count: "<<count<<" Process "<<ptr->process.uniqueNumber <<" with arrival time "<< ptr->process.arrivalTime << endl;
		ptr = ptr->next;
	}
	cout<<"________________________________________________________"<<endl;
}

int LinkedList::getCount()
{
	return count;
}

Node* LinkedList::deleteFromBeginning()
{
	if (head == NULL && tail==NULL && count==0)
	{
		count=0;
		return NULL;
	}
	else if (head->next != NULL)
	{
		Node* temp = head;
		head = head->next;
		count--;
		return temp;
	}
	else if(head->next ==NULL && head==tail) //may
	{
		Node* temp = head;
		head=NULL;
		tail=NULL;
		count=0;
		return temp;
	}
}

void LinkedList::swapProcesses(Process & A, Process & B)
{
	Process temp = A;
	A = B;
	B = temp;
}
