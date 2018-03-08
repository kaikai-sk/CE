#pragma once

enum PageState
{
	prefetched,
	unprefetched,
	unknown
};

/*
	表示一个page及其相关信息
*/
class Node
{
public:
	//指向下一个节点
	Node    *next;
	//指向前一个节点
	Node    *prev;
	//page No
	unsigned key;
	
	//这是一个标记，这个page是否是被预取进来的
	//1: 这是被预取进来的
	//2: 这是正常访问得到的
	//3:不知道是不是预取进来的
	PageState pageState;
	/*
		看这个page是否被命中
		true：已经被命中
		false：没有被命中
	*/
	bool isHit;

public:
	Node() : next(0), prev(0), key(0), pageState(unknown), isHit(false) {}

	void attach(Node* prev);
	void detach();
};
