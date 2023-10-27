#include "rand_story.h"
#include <stdio.h>

int main(){

    char **lines = NULL;
    size_t lineNum = 0;
    lines = getlinesFromFile("story.txt",&lineNum);

    char **tokens = NULL;
    size_t tokenNum = 0;
    for(size_t i=0; i<lineNum; i++){
        appendTokensFromStr(lines[i], &tokens, &tokenNum);
        //TODO: 不知道最后一个要不要加上换行符
        appendTokensWithNewline(&tokens, &tokenNum);
    }

    printf("\n=========打印tokens:=========\n");
    for(size_t i=0; i<tokenNum; i++){
        printf("%s ", tokens[i]);
    }

    size_t blankNum = 0;
    Blank** blankArr = createBlankArr(tokens, tokenNum, &blankNum);

    printf("=========打印blank:=========\n");
    for(size_t i=0; i<blankNum; i++){
        printf("第%lu位上是%s\n", blankArr[i]->sourcePos, blankArr[i]->name);
    }

    

    return EXIT_SUCCESS;
}