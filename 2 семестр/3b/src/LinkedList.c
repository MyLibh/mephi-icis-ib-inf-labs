// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>

void DeallocateList(node_t* head)
{
	if (!head)
		return;

	node_t* ptr = head->next;

	while (ptr)
	{
		node_t* tmp = ptr->next;

		free(ptr);
		
		ptr = tmp;
	}

	free(head);

	head = NULL;
}

node_t* AddNextNode(node_t* cur)
{
	cur->next = (node_t*)calloc(1, sizeof(node_t));

	return cur->next;
}

void InsertNode(node_t* cur, char val)
{
	node_t* next = cur->next;

	node_t* ptr = AddNextNode(cur);
	ptr->c = val;
	ptr->next = next;
}

void RemoveNode(node_t* prev, node_t* cur)
{
	node_t* ptr = cur->next;

	free(prev->next);

	prev->next = ptr;
}

void PrintList(const node_t* head)
{
	node_t* ptr = head;
	while (ptr)
	{
		printf("%c", ptr->c);

		ptr = ptr->next;
	}

	printf("\n");
}