/*!
 *  @file  Tools.h
 *
 *  @date    2.13.2020
 *  @author  mylibh
 */

#ifndef __TOOLS_H_INCLUDED__
#define __TOOLS_H_INCLUDED__

#include <stddef.h>

/*!
*  @brief  Reads string from stdin before '\n'
*
*  @return  Read line
*/
char* get_str(void);

/*!
*  @brief  Removes space characters from the string
*
*  @param[in]  str  String for trimming
*
*  @return  Pointer to the new string
*
*  @note  Deallocates 'str'
*/
char* remove_spaces(char* str);

/*!
*  @brief  Returns substring starting from 'first' and ending with 'last'(do not include). Starts search from 'position'
*
*  @param[in]   first     Left border of substring
*  @param[in]   last      Right border of substring
*  @param[out]  position  Position to start search
*  @param[in]   str       String for search
*
*  @return  Pointer to the pattern(NULL otherwise)
*
*  @note  Stores the position of 'last' in 'position'
*/
char* get_pattern(const char first, const char last, size_t* position, const char* str);

#endif /* !__TOOLS_H_INCLUDED__ */