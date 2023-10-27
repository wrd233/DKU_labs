#include "rand_story.h"
#include <stdio.h>
#include <string.h>

char** getlinesFromFile(const char* fileName, size_t *lineNum){
    FILE* fp = fopen(fileName, "r");

    char *line = NULL;
    char **lines = malloc(sizeof(*lines));
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&line, &len, fp)) != -1) {

        char** temp_ptr = realloc(lines, (++*lineNum)*sizeof(*lines));
        if (temp_ptr != NULL) {
            lines = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
        } else {
            // 处理内存分配失败的情况
        }

        // 去除末尾的换行符
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        lines[*lineNum-1] = line;
        line = NULL;
    }

    fclose(fp);

    return lines;
}

void appendTokensWithNewline(TokenArray* tokenArray){
    if(tokenArray->tokens == NULL){
        tokenArray->tokens = malloc(sizeof(char*));
    }

    char** temp_ptr = realloc(tokenArray->tokens, (++tokenArray->num)*sizeof(*tokenArray->tokens));
    if (temp_ptr != NULL) {
        tokenArray->tokens = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
    } else {
        // 处理内存分配失败的情况
    }

    tokenArray->tokens[tokenArray->num-1] = "\n"; 

}

TokenArray* createTokenArray(){
    TokenArray* tokenArray = malloc(sizeof(TokenArray));
    tokenArray->tokens = NULL;
    tokenArray->num = 0;
    return tokenArray;
}

BlankArray* createBlankArray(){
    BlankArray* blankArray = malloc(sizeof(BlankArray));
    blankArray->blanks = NULL;
    blankArray->num = 0;
    return blankArray;
}

void appendTokensFromStr(const char* str, TokenArray* tokenArray){
    char* strCpy = strdup(str);
    char *token = strtok(strCpy, " ");
    while (token != NULL) {
        tokenArray->num++;
        char** temp_ptr = realloc(tokenArray->tokens, (tokenArray->num)*sizeof(*tokenArray->tokens));
        if (temp_ptr != NULL) {
            tokenArray->tokens = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
        } else {
            // 处理内存分配失败的情况
            perror("错掉了");
        }

        tokenArray->tokens[tokenArray->num-1] = strdup(token); 
        token = strtok(NULL, " ");
    }

    free(strCpy);
}

BlankArray* buildBlankArr(TokenArray* tokenArray){
    BlankArray* blankArray = createBlankArray();
    for(size_t i=0; i<tokenArray->num; i++){
        char* token = tokenArray->tokens[i];
        char* firstPos = strchr(token, '_');
        char* lastPos = strrchr(token, '_');
        if(firstPos == NULL){
            continue;
        }
        if(lastPos == firstPos){
            perror("错！");
        }else{
            Blank* temp = malloc(sizeof(*temp));
            temp->name = strndup(firstPos+1, lastPos-firstPos-1);
            temp->sourcePos = i;

            blankArray->num++;
            Blank** temp_ptr = realloc(blankArray->blanks, (blankArray->num)*sizeof(*blankArray->blanks));
            if (temp_ptr != NULL) {
                blankArray->blanks = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
            } else {
                // 处理内存分配失败的情况
            }
            
            blankArray->blanks[blankArray->num-1] = temp;
        }
    }
    return blankArray;
}

void blankReplace(TokenArray* tokenArray, const Blank* blank, const char* word){

}