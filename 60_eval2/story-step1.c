#include "rand_story.h"
#include <stdio.h>

int main(){
    char **tokens = NULL;
    char **lines = NULL;
    size_t lineNum = 0;
    size_t num = 0;
    lines = getlinesFromFile("story.txt",&lineNum);
    for(size_t i=0; i<lineNum; i++){
        appendTokensFromStr(lines[i], &tokens, &num);
        //TODO: 不知道最后一个要不要加上换行符
        appendTokensWithNewline(&tokens, &num);
    }
    for(size_t i=0; i<num; i++){
        printf("%s ", tokens[i]);
    }

    return EXIT_SUCCESS;
}