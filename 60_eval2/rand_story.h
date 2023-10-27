#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"

//any functions you want your main to use

/**
 * @param str input string
 * @param tokens tokens
 * @param num the number of current tokens 
 * @brief Parse tokens separated by spaces in str and add them to tokens
*/
void appendTokensFromStr(const char* str, char ***tokens, size_t *num);

void appendTokensWithNewline(char ***tokens, size_t *num);

char** getlinesFromFile(const char* fileName, size_t *lineNum);


struct blank{
    size_t sourcePos;
    char *name;
}

#endif
