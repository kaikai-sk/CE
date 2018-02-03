#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <queue>
#include <time.h>
#include <cstdlib>
#include <string>

#define min(a, b) ((a) < (b) ? (a) : (b))  // simple inline function

using namespace std;

// used to store the sequence read from the input file
//����ͳ�Ƶ�һƵ���¼�
typedef multimap<int, int, less<int> > sequence;

/*
λ�ñ���ṹ�壬���޷�����������������ƴ������������32λ��
ͨ������ʽָ����һ��λ�ñ���ṹ�壬����λ�ñ�������
*/
typedef struct positionCode1
{
	//position code is composed of a series of binary
	unsigned int code;
	//if the length of position code is longer than 32
	//we use a link to next 32 bits
	positionCode1 *next;
}positionCode;

// Structure of a node in the PLWAP tree:
// PLWAP-Tree �Ľڵ�ṹ��
typedef struct node1
{
	//ʱ������
	int event;	//	event name of the node 
				//�¼�����Ƶ��
	int occur;	//	occurence for the node
				//λ�ñ���ĳ���
	int pcLength;	//	the length of position code for the node
					//λ�ñ���
	positionCode *pcCode;	//	the pointer of the position code
							//���ӽڵ���ֵĴ���
	int CountSon;	//	the sum of occurence of sons
					//	the linkage to next node with same event name
					//ָ����һ��ͬ���¼���ָ��
	node1 *nextLink;
	//���ӽڵ�ָ��
	node1 *lSon;	//	the pointer to its left Son
				//���ֵܽڵ�ָ��
	node1 *rSibling;	//	the pointer to its right sibling
					//���ڵ�ָ��
	node1 *parent;	//	the poniter to its parent
}node;

//Structure of linkage header:
//header �б�ṹ��
typedef struct linkheader1
{
	//�¼�����
	int event;	//	event name of the linkage
				//�¼����ֵ�Ƶ��
	int occur;	//	occurence of the node in tree 
				//ָ�����е�һ��ͬ���ڵ�
	node *link;	//link to the first occurrence of the event in tree
				//ָ�������¼������ֵĽڵ�
	node *lastLink;	//	link to the last occurrence of the event in tree
}linkheader;



//PLWAP�㷨�����
void plwap(double minSupp = 0.00005, char* plwap_result_file = "result_PLWAP.data", char* src_data_file = "dstForPLWAP.data");

//following are functions used for the PLWAP mining.

void printtree(node*);
// For testing: print the whole PLWAP tree 
void printLinkage(list<linkheader>);
// For testing: print the linkage
void BuildTree(char*,float minSupp);
// Build the PLWAP tree
void BuildLinkage(node *);
positionCode* makeCode(int, positionCode*, bool);
// Make position code for a node
int checkPosition(positionCode*, int, positionCode*, int);
// Check the position between two nodes
void MiningProcess(list<node*>, queue<int>, int);
// Mine sequential pattern from PLWAP tree
