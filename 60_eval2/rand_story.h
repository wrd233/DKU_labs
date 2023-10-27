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
};

typedef struct blank Blank;

// 从tokens建立blank数组
Blank** createBlankArr(char **tokens, size_t tokenNum, size_t* blankNum);

// 将blank位置的内容进行替换
void blankReplace(char **tokens, const Blank* blank, const char* word);

#endif
