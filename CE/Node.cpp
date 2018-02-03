#include "Node.h"

/*
链表中插入一个节点
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
链表中删除一个节点
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