#pragma once

/*
	��ʾһ��page���������Ϣ
*/
class Node
{
public:
	//ָ����һ���ڵ�
	Node    *next;
	//ָ��ǰһ���ڵ�
	Node    *prev;
	//page No
	unsigned key;
	//����һ����ǣ����page�Ƿ��Ǳ�Ԥȡ������
	//1: ���Ǳ�Ԥȡ������
	//0: �����������ʵõ���
	unsigned tag;
	//���page�����еĴ���
	unsigned hit_num;

public:
	Node() : next(0), prev(0), key(0), tag(0), hit_num(0) {}

	void attach(Node* prev);
	void detach();
};
