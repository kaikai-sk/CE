#include "Node.h"

/*
�����в���һ���ڵ�
*/
void Node::attach(Node *head)
{
	if (head->next)
		head->next->prev = this;
	this->next = head->next;

	head->next = this;
	this->prev = head;
}

/*
������ɾ��һ���ڵ�
*/
void Node::detach()
{
	if (this->prev)
		this->prev->next = this->next;
	if (this->next)
		this->next->prev = this->prev;

	this->next = 0;  // I don't like `nullptr`
	this->prev = 0;
}