/*!
 *  @file  Tools.h
 *
 *  @date    2.14.2020
 *  @author  mylibh
 */

#ifndef __TOOLS_H_INCLUDED__
#define __TOOLS_H_INCLUDED__

struct node_t;

/*!
*  @brief  Reads string from stdin into list
*
*  @param[out]  head  Pointer to pointer to head of the list
*/
void get_str(struct node_t** list);

/*!
*  @brief  Removes '\n', ' ', '\r', etc. from the list
*
*  @param[in]  head  Pointer to head of the list
*
*  @return  Value of the last node in the list
*/
char get_last_val(struct node_t* head);

/*!
*  @brief  Removes '\n', ' ', '\r', etc. from the list
*
*  @param[out]  head  Pointer to pointer to head of the list
*/
void remove_spaces(struct node_t** head);

#endif /* !__TOOLS_H_INCLUDED__ */