#pragma once
#include"Process.h"
#include<iostream>
using namespace std;

struct Node
{
	Process process;
	Node *next;
};

class LinkedList {
private:
	Node* head;
	Node* tail;
	int count;

public:
	LinkedList();
	void insertAtEnd(Process p);
	void swapProcesses(Process &A, Process &B);
	void bubbleSort(int criteria);
	Node* getHead();
	Node* getTail();
	void Print();
	int getCount();
	Node* deleteFromBeginning();
};
