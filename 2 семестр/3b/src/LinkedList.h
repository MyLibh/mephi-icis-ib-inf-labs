/*!
 *  @file  LinkedList.h
 *
 *  @date    2.14.2020
 *  @author  mylibh
 */

#ifndef __LINKEDLIST_H_INCLUDED__
#define __LINKEDLIST_H_INCLUDED__

typedef struct _tag_node
{
	char c;
	struct node_t* next;
} node_t;

/*!
*  @brief  Deallocates memory used by the list
*
*  @param[in]  head  Pointer to the head of the list
*/
void DeallocateList(node_t* head);

/*!
*  @brief  Inserts node after 'cur'
*
*  @param[in]  cur  Pointer to node
*
*  @return  Pointer to new node
*
*  @note  Works only for last node
*/
node_t* AddNextNode(node_t* cur);

/*!
*  @brief  Inserts node after 'cur'
*
*  @param[in]  cur  Pointer to current node
*  @param[in]  val  Value to store
*/
void InsertNode(node_t* cur, char val);

/*!
*  @brief  Inserts node after 'cur'
*
*  @param[in]  prev  Pointer to previous node
*  @param[in]  cur   Pointer to node for removing
*/
void RemoveNode(node_t* prev, node_t* cur);

/*!
*  @brief  Prints list values to stdout
*
*  @param[in]  head  Pointer to head of the list
*/
void PrintList(const node_t* head);

#endif /* __LINKEDLIST_H_INCLUDED__ */