// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Matrix.h"

#include <stdio.h>
#include <stdlib.h>

void AllocateMatrix(mat_t* matrix)
{
	if (!matrix->n)
	{
		printf("Wrong size\n");

		return;
	}

	matrix->data = (seq_t*)calloc(matrix->n, sizeof(seq_t));
	if (!matrix->data)
	{
		printf("Cannot allocate memory for matrix\n");

		exit(1);
	}
}

void DeallocateMatrix(mat_t* matrix)
{
	for (size_t i = 0; i < matrix->n; ++i)
		DeallocateSequence(&matrix->data[i]);

	free(matrix->data);

	matrix->n	 = 0;
	matrix->data = NULL;
}

void ReadMatrix(mat_t* matrix)
{
	for (size_t i = 0; i < matrix->n; ++i)
	{
		printf("\nInput m%zd\n> ", i);
		scanf_s("%zu", &matrix->data[i].n);

		AllocateSequence(&matrix->data[i]);

		ReadSequence(&matrix->data[i]);
	}
}

void PrintMatrix(const mat_t* matrix)
{
	for (size_t i = 0; i < matrix->n; ++i)
		PrintSequence(&matrix->data[i]);
}