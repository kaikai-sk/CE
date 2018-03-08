#pragma once

enum PageState
{
	prefetched,
	unprefetched,
	unknown
};

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
	//2: �����������ʵõ���
	//3:��֪���ǲ���Ԥȡ������
	PageState pageState;
	/*
		�����page�Ƿ�����
		true���Ѿ�������
		false��û�б�����
	*/
	bool isHit;

public:
	Node() : next(0), prev(0), key(0), pageState(unknown), isHit(false) {}

	void attach(Node* prev);
	void detach();
};
