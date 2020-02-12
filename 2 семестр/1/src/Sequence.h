/*!
 *  @file Sequence.h
 *
 *  @date    2.12.2020
 *  @author  mylibh
 */

#ifndef __SEQUENCE_H_INCLUDED__
#define __SEQUENCE_H_INCLUDED__

#include <stddef.h>

typedef int elem_t;

typedef struct _tag_sequence
{
	size_t n;
	elem_t* data;
} seq_t;

/*!
 *  @brief  Allocates memory for sequence
 *
 *  @param[out]  sequence  Pointer to sequence
 */
void AllocateSequence(seq_t* sequence);

/*!
 *  @brief  Deallocates memory for sequence
 *
 *  @param[out]  sequence  Pointer to sequence
 */
void DeallocateSequence(seq_t* sequence);

/*!
 *  @brief  Reads data from stdin
 *
 *  @param[out]  sequence  Pointer to sequence
 */
void ReadSequence(seq_t* sequence);

/*!
 *  @brief  Prints sequence to stdout
 *
 *  @param[in]  sequence  Pointer to sequence
 */
void PrintSequence(const seq_t* sequence);

#endif /* !__SEQUENCE_H_INCLUDED__ */