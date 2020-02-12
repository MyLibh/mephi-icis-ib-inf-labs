// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef _MSC_VER
#error
#error	MSVC compiler needed
#error
#endif /* !_MSC_VER */

#include "Sequence.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int get_number_of_digits(elem_t x);
void update(elem_t* x);
void proceed(const seq_t* src, seq_t* dst);
inline int less(const void* a, const void* b);
inline int greater(const void* a, const void* b);
inline int compare(const void* a, const void* b);

signed main(void)
{
	seq_t seq;
	seq.n	 = 0;
	seq.data = NULL;

	printf("Enter n\n> ");
	scanf_s("%zu", &seq.n);

	AllocateSequence(&seq);
	
	printf("\nEnter sequence\n> ");
	ReadSequence(&seq);
	
	seq_t new_seq;
	new_seq.n	 = seq.n << 1;
	new_seq.data = NULL;
	AllocateSequence(&new_seq);

	proceed(&seq, &new_seq);

	printf("\nResult\n");
	PrintSequence(&seq);
	PrintSequence(&new_seq);

	DeallocateSequence(&new_seq);
	DeallocateSequence(&seq);

	return 0;
}

int get_number_of_digits(elem_t x)
{
	int num = 0;
	while (x)
	{
		x /= 10;

		++num;
	}

	return num;
}

void update(elem_t* x)
{
	if (!*x)
	{
		*x = 1;

		return;
	}

	int res = 0,
		cnt = 0,
		sgn = *x < 0 ? -1 : 1;

	*x = abs(*x);
	while (*x)
	{
		res += (((*x % 10) + 1) % 10) * (int)pow(10, cnt++);

		*x /= 10;
	}

	*x = sgn * res;
}

void proceed(const seq_t* src, seq_t* dst)
{
	assert(src->n << 1 == dst->n);

	memcpy(dst->data, src->data, src->n * sizeof(elem_t));
	memcpy(dst->data + src->n, src->data, src->n * sizeof(elem_t));

	qsort(dst->data, src->n, sizeof(elem_t), &compare);

	for (size_t i = src->n; i < dst->n; ++i)
		update(&dst->data[i]);

	qsort(dst->data + src->n, src->n, sizeof(elem_t), &less);
}

inline int less(const void* a, const void* b)
{
	if (*(elem_t*)a == *(elem_t*)b)
		return 0;

	return *(elem_t*)a > * (elem_t*)b ? -1 : 1;
}

inline int greater(const void* a, const void* b)
{
	return -less(a, b);
}

inline int compare(const void* a, const void* b)
{
	int a_num = get_number_of_digits(*(elem_t*)a),
		b_num = get_number_of_digits(*(elem_t*)b);

	return greater(&a_num, &b_num);
}