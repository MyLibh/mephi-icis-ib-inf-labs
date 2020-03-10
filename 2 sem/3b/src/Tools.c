// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Tools.h"
#include "LinkedList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_LENGTH 128

void get_str(node_t** list)
{
	char buf[BUFFER_LENGTH] = "";

	node_t head = { NULL };
	node_t* cur = &head;

	int scanf_ret_val = 0;
	do
	{
		scanf_ret_val = scanf_s("%127[^\n]", buf, BUFFER_LENGTH);
		if (scanf_ret_val == 1)
		{
			size_t len = strlen(buf);
			for (size_t i = 0; i < len; ++i)
			{
				cur = AddNextNode(cur);	

				cur->c = buf[i];
			}
		}
		else if (!scanf_ret_val)
			scanf_s("%*c");
		else
			DeallocateList(&head);
	} while (scanf_ret_val > 0);

	*list = head.next;
}

char get_last_val(node_t* head)
{
	node_t* ptr = head;
	while (ptr->next)
		ptr = ptr->next;

	return ptr->c;
}

void remove_spaces(node_t** head)
{
	node_t* prev = *head,
		  * ptr  = (*head)->next;
	while (ptr)
	{
		if (isspace(ptr->c))
		{
			RemoveNode(prev, ptr);

			ptr = prev->next;

			continue;
		}

		prev = ptr;
		ptr = ptr->next;
	}

	if (isspace((*head)->c))
	{
		node_t* ptr = (*head)->next;

		free(*head);

		*head = ptr;
	}
}