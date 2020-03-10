// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _MSC_VER
#error
#error	MSVC compiler needed
#error
#endif /* !_MSC_VER */

#include "LinkedList.h"
#include "Tools.h"

#include <stdio.h>
#include <stdlib.h>

void proceed(node_t** head);
int rec(const char first, const char last, node_t* prev_start, node_t* start);

signed main(void)
{
	while (1)
	{
		printf("Enter string\n> ");
		
		node_t* list = NULL;
		get_str(&list);
		if (!list)
			break;

		proceed(&list);

		printf("\nResult\n");
		PrintList(list);
		printf("\n");

		DeallocateList(list);
	}

	return 0;
}

void proceed(node_t** head)
{
	remove_spaces(head);

	char first = (*head)->c;
	char last = get_last_val(*head);

	rec(first, last, NULL, *head);

	node_t* ptr = (*head)->next;

	free(*head);

	*head = ptr;
}

int rec(const char first, const char last, node_t* prev_start, node_t* start)
{
	if (prev_start)
	{
		RemoveNode(prev_start, start);

		start = prev_start->next;
	}

	node_t* prev = start;
	node_t* ptr = prev ? prev->next : NULL;
	while (ptr && ptr->c != last)
	{
		prev = ptr;
		ptr = ptr->next;
	}
	// PrintList(start); PrintList(prev); PrintList(ptr); printf("\n");
	if (!ptr)
	{
		//DeallocateList(start);

		return 0;
	}

	while (ptr && ptr->c != first)
	{
		RemoveNode(prev, ptr);

		ptr = prev->next;
	}

	// PrintList(start); PrintList(prev); PrintList(ptr); printf("\n");
	if (ptr)
		if(rec(first, last, prev, ptr))
			InsertNode(prev, ' ');

	return 1;
}