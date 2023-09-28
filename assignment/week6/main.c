#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FAMILYNUM 100
#define FAMILYMEMMAX 10 // There are up to FAMILYMEMMAX people in a family
#define ALLOC 200


typedef struct family_t{
    char * surname;
    size_t size;
    char ** names;
} family;
typedef struct registry_t{
    family * myData;
    size_t nrecs;
} registry;

// int getNameNum(char** names){
    // int res = 0;
    // if(names == NULL){return res;}
    // for(int i=0; names[i]!=NULL && names[i][0] != '\0';i++){
    //     res ++;
    // }
    // return res;
// }

// TODO: 检查一下人口是否对得上
void addFamily(registry* myRecs, char* surname, char* sizeStr, char** names){
    if(myRecs == NULL){return;}

    int size=0;
    size = atoi(sizeStr);
    printf("size=%d\n", size);

    // 检查:size是否有错
    if(size<=0){return;}

    // 检查:人数是否有错
    // if(size != getNameNum(names)){return;}

    int pos = myRecs->nrecs++;

    printf("add family %s\n",surname);
    myRecs->myData[pos].surname = surname;
    myRecs->myData[pos].size = size;
    myRecs->myData[pos].names = (char**)malloc(FAMILYMEMMAX*sizeof(char*));
    for(int i=0;i<size;i++){
        myRecs->myData[pos].names[i] = names[i];
    }
    printf("-----------\n");
}

void swap(family *x, family *y) {
    family temp = *x;
    *x = *y;
    *y = temp;
}

void deleteFamily(registry* myRecs, char* surname){
    if(myRecs==NULL || myRecs->nrecs==0){return;}

    int deletePos=-1;

    for(int i=0;i<myRecs->nrecs;i++){
        if( strcmp(myRecs->myData[i].surname,surname)==0 ){
            deletePos = i;
            break;
        }
    }
    if(deletePos == -1){return;}

    printf("delete family %s\n",surname);
    free(myRecs->myData[deletePos].names);
    swap(&myRecs->myData[deletePos], &myRecs->myData[myRecs->nrecs-1]);
    myRecs->nrecs--;

    if(myRecs->nrecs==0){
        free(myRecs->myData);
        free(myRecs);
    }
    printf("-----------\n");
}

void printFamily(registry* myRecs){
    if(myRecs == NULL){
        printf("No family!\n");
        return ;
    }
    printf("print family: \n\n");
    for(int i=0;i<myRecs->nrecs;i++){
        printf("The %s family:\n",myRecs->myData[i].surname);
        for(int j=0;j<myRecs->myData[i].size;j++){
            printf("  %s %s\n",myRecs->myData[i].names[j], myRecs->myData[i].surname);
        }
        printf("\n");
    }
    printf("-----------\n");
}

ssize_t newgetline(char **line, size_t *n, FILE *fp) {
    char *buf = *line;
    ssize_t c, i = 0;
    if (buf == NULL || *n == 0) {
        *line = malloc(ALLOC); // TODO: 检查一下分配是否成功(包括之后的内存分配)
        buf = *line;
        *n = ALLOC;
    }
    while ((c = fgetc(fp)) != '\n') {
        if (c == EOF)
            return -1;
        if (i < *n - 2) {
            *(buf + i++) = c;
        }
        else {
            *n = *n + ALLOC;
            buf = realloc(buf, *n);
            *(buf + i++) = c;
        }
    }
    // *(buf + i++) = '\n';
    *(buf + i) = '\0';
    return i;
}


int main(int argc,char **argv){
    registry * myRecs;
    // write the code to initialize myRecs so that it holds in the heap the following families
    myRecs = (registry*)malloc(sizeof(struct registry_t));

    // TODO: Now the initialized data size is fixed, and it is recommended to change it to one that can be expanded by itself
    myRecs->myData = malloc(FAMILYNUM * sizeof(struct family_t));
    myRecs->nrecs = 0;

    char **lines = NULL;
    char *curr = NULL;
    size_t sz;
    size_t lineNum = 0;

    // get input
    for(int j=1;j<argc;j++){
        FILE* fp = fopen(argv[j],"r");  // TODO:检查一下文件是否打开
        while (newgetline(&curr, &sz, fp) >= 0){
            lines = realloc(lines, (lineNum + 1) * sizeof(*lines));
            lines[lineNum] = curr;
            curr = NULL;
            lineNum++;
        }
        fclose(fp);
    }


    // 分割
    const char delim[3] = ", ";
    char *token;
    char** temp;

    temp = malloc(FAMILYMEMMAX*sizeof(char**));

    for(int j=1; j<lineNum; j++){
        int p=0;
        token = strtok(lines[j], delim);
        while(token != NULL){
            // printf("%s\n", token);
            temp[p++] = token;
            token = strtok(NULL, delim);
        }
        addFamily(myRecs, temp[0], temp[1], &temp[2]);
    }

    int sum=0;

    for(int i=0; i<myRecs->nrecs; i++){
        sum += myRecs->myData[i].size;
        // printf("%s家有%ld个人\n",myRecs->myData[i].surname, myRecs->myData[i].size);    
    }

    printf("一共有%d的人\n", sum);

    printFamily(myRecs);

    return EXIT_SUCCESS;
} 

