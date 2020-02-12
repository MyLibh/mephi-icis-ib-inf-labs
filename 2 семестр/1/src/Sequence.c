// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Sequence.h"

#include <stdlib.h>
#include <stdio.h>

void AllocateSequence(seq_t* sequence)
{
	if (!sequence->n)
	{
		printf("Wrong length of sequence\n");

		return;
	}

	sequence->data = (elem_t*)calloc(sequence->n, sizeof(elem_t));
	if (!sequence->data)
	{
		printf("Cannot allocate memory for the sequence\n");

		abort(); // Can cause memory leak
	}
}

void DeallocateSequence(seq_t* sequence)
{
	free(sequence->data);

	sequence->n = 0;
	sequence->data = NULL;
}

void ReadSequence(seq_t* sequence)
{
	for (size_t i = 0; i < sequence->n; ++i)
		scanf_s("%d", &sequence->data[i]);
}

void PrintSequence(const seq_t* sequence)
{
	for (size_t i = 0; i < sequence->n; ++i)
		printf("%d ", sequence->data[i]);

	printf("\n");
}