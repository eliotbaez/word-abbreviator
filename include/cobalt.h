/*
 * cobalt.h
 * by Eliot Baez
 *
 * Header file for libcobalt; CObaLT; Compression, Obviously by a Lookup
 * Table. CObaLT is a library for compressing plaintext in the English
 * language (and possibly others!) on a word-by-word basis.
 *
 * This header file contains declarations of the functions and global
 * variables that form the CObaLT API.
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef COBALT_H
#define COBALT_H

/* 
 * WORDTABLE is a contiguous array of characters containing every word
 * in a predefined list, defined at compile time. The words are null-
 * separated, and the entire list is null-terminated. 
 * 
 * WORDTABLE_LEN is the length of the entire WORDTABLE array, excluding
 * the final null byte. 
 */
extern const char *WORDTABLE;
extern const size_t WORDTABLE_LEN;

/* 
 * WORDMAP is an array of 32-bit unsigned integers that store indexes
 * within WORDTABLE. For example, WORDMAP[3] stores the index of word 3
 * within WORDTABLE, such that WORDTABLE[ WORDMAP[3] ] is the address in
 * memory of the first character of the 4th word in the table. 
 * 
 * Since each of the words are null-terminated within the table, the
 * address obtained from WORDTABLE[ WORDMAP[n] ] can be treated directly
 * as a null-terminated string containing the nth word in the table.
 *
 * The number of elements in WORDMAP is defined in the macro NUMBER_OF_WORDS.
 */
extern const uint32_t WORDMAP[];

/*
 * GUIDETABLE is an array of 16-bit unsigned integers that store indexes
 * within WORDMAP.
 *
 * The underlying code of CObaLT uses a uint16_t to store the first 2
 * characters in a string when searching for a string in the table. The
 * order that these characters occur in the integer depend on the
 * endianness of the machine where CObaLT is compiled, but it does not
 * affect the function of the library.
 *
 * You should not have to ever use GUIDETABLE when using CObaLT through
 * the intended API functions. However, if you were to choose to, this
 * is how you would:
 *
 * 		const char *str = "Something";
 * 		uint16_t buf = *( (uint16_t *)str );
 * 		uint32_t word =	GUIDETABLE[buf];
 *
 * In this example, `word' stores the the ordinal number of the first
 * word in the table that shares the first 2 characters with `str'. If
 * no word in the table shares its first 2 characters with `str', then
 * `word' is the ordinal number of the last word in the list.
 */
extern const uint16_t GUIDETABLE[];

/* 
 * cblt_streq takes two null-terminated strings as arguments and returns
 * true only if the two strings are identical. The two pointers do not
 * need to be unique.
 *
 * This function is different from strcmp() in that it will ONLY tell us
 * whether str1 and str2 point to identical strings. This gives us the
 * power to simply return as soon as the first non-matching character is
 * found and save processor time. 
 *
 * It is intended only for internal use, but it may be useful, so it is
 * exposed in the API.
 */
bool cblt_streq(const char *str1, const char *str2);

/*
 * cblt_findWord searches for str in the word table and returns the
 * ordinal number of the word that matches str. Returns -1 if the string
 * is not found and -2 if passed an empty string.
 *
 * The value returned by cblt_findWord can be safely cast to uint16_t
 * without any loss of information, as long as neither of the 2 error
 * codes are returned.
 */
int32_t cblt_findWord(const char *str);

#define CBLT_WORD_NOT_FOUND	-1
#define CBLT_EMPTY_WORD_ARG	-2

/*
 * In a uint16_t array, an integer with the value CBLT_BEGIN_STRING
 * indicates that the address of the next integer is the beginning of
 * a null-terminated char array.
 *
 * There are 2 other special cases, which are the period (.) and the
 * comma (,). These characters are common enough in sentences that it
 * makes sense to give them their own hard-coded numbers.
 *
 * Spaces are even more common, and such they are interpreted as the
 * delimiter character when encoding and decoding.
 */
#define CBLT_BEGIN_STRING	0xFFFF
#define CBLT_PERIOD			0xFFFE	/*** NOT YET IMPLEMENTED ***/
#define CBLT_COMMA			0xFFFD	/*** NOT YET IMPLEMENTED ***/

/*
 * cblt_encodeSentence takes a single null-terminated sentence as an
 * argument and returns a pointer to a null-terminated array of 16-bit
 * unsigned integers containing the compressed sentence data. The memory
 * block pointed to by the return value is dynamically allocated, so it
 * is the job of the programmer to free() the pointer after doing
 * something meaningful with the data.
 *
 * This function will return a NULL pointer if any of the following
 * errors occur:
 * 	- sentence is a NULL Pointer 
 * 	- a memory allocation fails within the function
 *
 * If passed an empty string, the function will return a pointer to a
 * 16-bit unsigned integer with the value 0.
 */
uint16_t *cblt_encodeSentence(const char *sentence);

#endif /* COBALT_H */
