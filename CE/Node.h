#pragma once

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
	//0: 这是正常访问得到的
	unsigned tag;
	//这个page被命中的次数
	unsigned hit_num;

public:
	Node() : next(0), prev(0), key(0), tag(0), hit_num(0) {}

	void attach(Node* prev);
	void detach();
};
