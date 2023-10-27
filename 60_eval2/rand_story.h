#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"

//any functions you want your main to use
typedef struct{
    size_t sourcePos;
    char *name;
} Blank;

typedef struct{
    Blank **blanks;
    size_t num;
} BlankArray;

typedef struct {
    char** tokens;
    size_t num;
} TokenArray;


/**
 * @param str input string
 * @param tokens tokens
 * @param num the number of current tokens 
 * @brief Parse tokens separated by spaces in str and add them to tokens
*/

TokenArray* createTokenArray();

BlankArray* createBlankArray();

void appendTokensFromStr(const char* str, TokenArray* tokenArray);

void appendTokensWithNewline(TokenArray* tokenArray);

char** getlinesFromFile(const char* fileName, size_t *lineNum);

// 从tokens建立blank数组
BlankArray* buildBlankArr(TokenArray* tokenArray);

// 将blank位置的内容进行替换
void blankReplace(TokenArray* tokenArray, const Blank* blank, const char* word);

#endif
