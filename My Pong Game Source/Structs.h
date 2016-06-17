#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <iostream>
struct Node
{
	int value;
	std::string name;
	Node *next = NULL;
	Node *previous = NULL;
};

struct myList
{
	Node *head = NULL;
	Node *tail = NULL;
	//add element at the end of list
	void add(int &value,char name[4])
	{
		Node *node = new Node();//this does not produce memory leak, it is intended to be this way;
		node->value = value;
		node->name=name;
		if (this->empty())
		{
			head = node;
			tail = node;
		}
		else
		{
			tail->next = node;
			node->previous = tail;
			tail = node;
		}


	};
	myList()
	{

	}
	void fill(int number, int value,char name[4])
	{
		Node *newNode = new Node();
		newNode->value = value;
		newNode->name = name;
		head = newNode;
		tail = newNode;
		for (int i = 0; i < number - 1; ++i)
		{
			Node *node = new Node();
			node->value = value;
			node->name = name;
			newNode->next = node;
			node->previous = newNode;
			newNode = node;
			tail = node;
		}
	}

	void deleteFromList(Node *node)
	{
		if (node != tail && node != head)
		{
			if (node == head)
			{
				head = head->next;
				delete(head->previous);
				head->previous = NULL;
			}
			else
				if (node == tail)
				{
					tail = tail->previous;
					delete(tail->next);
					tail->next = NULL;
				}
				else
				{
					node->previous->next = node->next;
					node->next->previous = node->previous;
					delete(node);
				}
		}
	};

	bool empty()
	{
		if (head == NULL)
		{
			return true;
		}
		else return false;
	};

	~myList()
	{

		bool end = false;
		while (end == false)
		{
			if (head != NULL)
			{
				Node *node;
				node = head->next;
				delete(head);
				head = node;
			}
			else
			{
				end = true;
			}
		}
	}
	void destroy()
	{
		delete this;
	}
	void insert(Node *swapper, Node *nodeToBeSwapped)
	{
		if (nodeToBeSwapped == head)
		{
			head->previous = swapper;
			head = swapper;
			head->next = nodeToBeSwapped;
			tail = tail->previous;
			delete(tail->next);
			tail->next = NULL;
		}
		else if (nodeToBeSwapped == tail)
		{
			tail->previous->next = swapper;
			swapper->previous = tail->previous;
			delete(tail);
			tail = swapper;
			//tail->next is already null, DO NOT UNCOMMENT THIS LINE
		}
		else
		{
			Node *temp = nodeToBeSwapped->previous;
			nodeToBeSwapped->previous = swapper;
			swapper->next = nodeToBeSwapped;
			swapper->previous = temp;
			temp->next = swapper;
			tail = tail->previous;
			delete tail->next;
			tail->next = NULL;
		}

	}
	void printHighScore()
	{
		Node *node = head;
		while (node != NULL)
		{
			printf("Name: %s Score: %i\n", node->name.c_str(), node->value);
			node = node->next;
		}
	}
};

struct RotationMatrix
{
	double angle;
	double radian = (angle*M_PI) / 180.0f;
	double matrix[2][2] = { cos(radian),sin(radian),-sin(radian),cos(radian) };
	RotationMatrix(double angle) :angle(angle)
	{};
};