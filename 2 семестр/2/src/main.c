// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _MSC_VER
#error
#error	MSVC compiler needed
#error
#endif /* !_MSC_VER */

#include "Matrix.h"

#include <stdio.h>
#include <stdlib.h>

int in(const elem_t value, const seq_t* seq);
seq_t find_duplicates(const seq_t* seq);
void proceed(const mat_t* src, mat_t* dst);

signed main(void)
{
	mat_t mat;
	mat.n	 = 0;
	mat.data = NULL;
	printf("Input n\n> ");
	scanf_s("%zu", &mat.n);

	AllocateMatrix(&mat);

	printf("\nInput matrix\n");
	ReadMatrix(&mat);

	mat_t new_mat;
	new_mat.n	 = mat.n;
	new_mat.data = NULL;
	AllocateMatrix(&new_mat);

	proceed(&mat, &new_mat);

	printf("\nResult\n");
	PrintMatrix(&mat);
	printf("\n");
	PrintMatrix(&new_mat);

	DeallocateMatrix(&new_mat);
	DeallocateMatrix(&mat);

	return 0;
}

int in(const elem_t value, const seq_t* seq)
{
	for (size_t i = 0; i < seq->n; ++i)
		if (seq->data[i] == value)
			return 1;

	return 0;
}

seq_t find_duplicates(const seq_t* seq)
{
	size_t num_dup = 0;

	seq_t res;
	res.n	 = seq->n >> 1;
	res.data = NULL;
	AllocateSequence(&res);

	for(size_t i = 0; i < seq->n - 1; ++i)
		for(size_t j = i + 1; j < seq->n; ++j)
			if (seq->data[i] == seq->data[j] && !in(seq->data[i], &res))
			{
				res.data[num_dup++] = seq->data[i];

				continue;
			}

	if (!num_dup)
	{
		printf("Cannot be zero duplicates in a row\n");

		DeallocateSequence(&res);

		exit(1);
	}

	void* tmp_ptr = realloc(res.data, num_dup * sizeof(elem_t));
	if (!tmp_ptr)
	{
		printf("Cannot reallocate(WUT???)\n");

		DeallocateSequence(&res);

		exit(1);
	}

	res.data = (elem_t*)tmp_ptr;
	res.n	 = num_dup;

	return res;
}

void proceed(const mat_t* src, mat_t* dst)
{
	for (size_t i = 0; i < src->n; ++i)
		dst->data[i] = find_duplicates(&src->data[i]);
}