#include "rand_story.h"
#include <stdio.h>

int main(){
    char **lines = NULL;
    size_t lineNum = 0;
    lines = getlinesFromFile("story.txt",&lineNum);

    TokenArray* tokenArray = createTokenArray();
    for(size_t i=0; i<lineNum; i++){
        appendTokensFromStr(lines[i], tokenArray);
        //TODO: 不知道最后一个要不要加上换行符
        appendTokensWithNewline(tokenArray);
    }

    printf("\n=========打印tokens:=========\n");
    for(size_t i=0; i<tokenArray->num; i++){
        printf("%s ", tokenArray->tokens[i]);
    }

    BlankArray* blankArray = buildBlankArr(tokenArray);
    printf("=========打印blank:=========\n");
    for(size_t i=0; i<blankArray->num; i++){
        Blank* temp = blankArray->blanks[i];
        printf("第%lu位上是%s\n", temp->sourcePos, temp->name);
    }



    return EXIT_SUCCESS;
}