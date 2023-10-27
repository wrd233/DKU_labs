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

void appendTokensWithNewline(char ***tokens, size_t *num){
    if(tokens == NULL){
        *tokens = malloc(sizeof(char*));
    }

    char** temp_ptr = realloc(*tokens, (++*num)*sizeof(**tokens));
    if (temp_ptr != NULL) {
        *tokens = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
    } else {
        // 处理内存分配失败的情况
    }

    (*tokens)[*num-1] = "\n"; 

}

void appendTokensFromStr(const char* str, char ***tokens, size_t *num){
    if(tokens == NULL){
        *tokens = malloc(sizeof(char*));
    }
    char* strCpy = strdup(str);

    char *token = strtok(strCpy, " ");
    while (token != NULL) {
        
        char** temp_ptr = realloc(*tokens, (++*num)*sizeof(**tokens));
        if (temp_ptr != NULL) {
            *tokens = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
        } else {
            // 处理内存分配失败的情况
        }

        (*tokens)[*num-1] = strdup(token); 
        token = strtok(NULL, " ");
    }

    free(strCpy);
}

Blank** createBlankArr(char **tokens, size_t tokenNum, size_t* blankNum){
    Blank** res = NULL;
    for(size_t i=0; i<tokenNum; i++){
        char* token = tokens[i];
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

            if(res == NULL){
                res = malloc(sizeof(Blank*));
            }
            Blank** temp_ptr = realloc(res, (++*blankNum)*sizeof(*res));
            if (temp_ptr != NULL) {
                res = temp_ptr;  // 重新分配成功，将临时指针赋值给原始指针
            } else {
                // 处理内存分配失败的情况
            }
            
            res[*blankNum-1] = temp;
        }
    }
    return res;
}

void blankReplace(char **tokens, const Blank* blank, const char* word){
    
}