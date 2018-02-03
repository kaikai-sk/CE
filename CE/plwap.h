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
//用于统计单一频繁事件
typedef multimap<int, int, less<int> > sequence;

/*
位置编码结构体，用无符号整型来代表二进制串，如果超过了32位，
通过链表方式指向下一个位置编码结构体，构成位置编码链表。
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
// PLWAP-Tree 的节点结构体
typedef struct node1
{
	//时间名称
	int event;	//	event name of the node 
				//事件出现频数
	int occur;	//	occurence for the node
				//位置编码的长度
	int pcLength;	//	the length of position code for the node
					//位置编码
	positionCode *pcCode;	//	the pointer of the position code
							//孩子节点出现的次数
	int CountSon;	//	the sum of occurence of sons
					//	the linkage to next node with same event name
					//指向下一个同名事件的指针
	node1 *nextLink;
	//左孩子节点指针
	node1 *lSon;	//	the pointer to its left Son
				//右兄弟节点指针
	node1 *rSibling;	//	the pointer to its right sibling
					//父节点指针
	node1 *parent;	//	the poniter to its parent
}node;

//Structure of linkage header:
//header 列表结构体
typedef struct linkheader1
{
	//事件名称
	int event;	//	event name of the linkage
				//事件出现的频数
	int occur;	//	occurence of the node in tree 
				//指向树中第一个同名节点
	node *link;	//link to the first occurrence of the event in tree
				//指向树中事件最后出现的节点
	node *lastLink;	//	link to the last occurrence of the event in tree
}linkheader;



//PLWAP算法的入口
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
