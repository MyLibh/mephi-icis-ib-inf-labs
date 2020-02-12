/*!
 *  @file  Matrix.h
 *
 *  @date    2.13.2020
 *  @author  mylibh
 */

#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

#include "Sequence.h"

typedef struct _tag_matrix
{
    size_t n;
    seq_t* data;
} mat_t;

/*!
 *  @brief  Allocates memory for matrix
 *
 *  @param[out]  sequence  Pointer to matrix
 *
 *  @note  Allocates only for 'n' sequences(because of task)
 */
void AllocateMatrix(mat_t* matrix);

/*!
 *  @brief  Deallocates memory for matrix
 *
 *  @param[out]  matrix  Pointer to matrix
 */
void DeallocateMatrix(mat_t* matrix);

/*!
 *  @brief  Reads data from stdin
 *
 *  @param[out]  matrix  Pointer to matrix
 */
void ReadMatrix(mat_t* matrix);

/*!
 *  @brief  Prints matrix to stdout
 *
 *  @param[in]  matrix  Pointer to matrix
 */
void PrintMatrix(const mat_t* matrix);

#endif /* !__MATRIX_H_INCLUDED__ */