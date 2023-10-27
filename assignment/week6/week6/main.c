#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FAMILYNUM 100
#define FAMILYMEMMAX 10 // There are up to FAMILYMEMMAX people in a family
#define ALLOC 200
#define DELIM ", "

typedef struct family_t{
    char * surname;
    size_t size;
    char ** names;
} family;

typedef struct registry_t{
    family * myData;
    size_t nrecs;
} registry;

// Get the number of names in the Family
int getNameNum(char** names){
    int res = 0;
    if(names == NULL){return res;}
    // magic: suppose the end of names is NULL
    for(int i=0; names[i]!=NULL;i++){
        res ++;
    }
    return res;
}

// Insert a Family(parsed by a line of formatted string) into a registry
void addFamily(registry* myRecs, char* surname, char* sizeStr, char** names){
    if(myRecs == NULL){return;}

    int size=0;
    size = atoi(sizeStr);

    // Check: size for errors
    if(size<=0){return;}

    // Check: The declared and actual numbers do not match
    if(size != getNameNum(names)){return;}

    int pos = -1;
    for(int i=0;i<myRecs->nrecs;i++){
        if(strcmp(surname,myRecs->myData[i].surname)==0){
            pos = i;
        }
    }
    if(pos!=-1){
        int sizePre = myRecs->myData[pos].size;
        myRecs->myData[pos].size += size;
        for(int i=0; i<size; i++){
            myRecs->myData[pos].names[sizePre + i] = names[i];
        }
    }else{
        int pos = myRecs->nrecs++;
        myRecs->myData[pos].surname = surname;
        myRecs->myData[pos].size = size;
        myRecs->myData[pos].names = (char**)malloc(FAMILYMEMMAX*sizeof(char*));
        for(int i=0;i<size;i++){
            myRecs->myData[pos].names[i] = names[i];
        }
    }
}

// Swap the positions of the two Family pointers
void swap(family *x, family *y) {
    family temp = *x;
    *x = *y;
    *y = temp;
}

// Remove a Family from the Registry (and free memory)
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

    free(myRecs->myData[deletePos].names);
    swap(&myRecs->myData[deletePos], &myRecs->myData[myRecs->nrecs-1]);
    myRecs->nrecs--;

    if(myRecs->nrecs==0){
        free(myRecs->myData);
        free(myRecs);
    }
}

// Compare two Families
int cmpFamily (const void * a, const void * b){
    return ( ((family*)a)->size - ((family*)b)->size );
}

void sortFamily(registry* myRecs){
    qsort(myRecs->myData, myRecs->nrecs, sizeof(family), cmpFamily);
}

void printFamily(registry* myRecs){
    if(myRecs == NULL){
        printf("No family!\n");
        return ;
    }
    int familyNum=0;
    int memberNum=1;
    for(int i=0;i<myRecs->nrecs;i++){
        memberNum = 1;
        printf("[%c] The %s family:\n",'A'+familyNum++,myRecs->myData[i].surname);
        for(int j=0;j<myRecs->myData[i].size;j++){
            printf("    %d: %s %s\n",memberNum++,myRecs->myData[i].names[j], myRecs->myData[i].surname);
        }
        printf("\n");
    }
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

registry* initReg(){
    registry * myRecs;
    // write the code to initialize myRecs so that it holds in the heap the following families
    myRecs = (registry*)malloc(sizeof(struct registry_t));

    // TODO: Now the initialized data size is fixed, and it is recommended to change it to one that can be expanded by itself
    myRecs->myData = malloc(FAMILYNUM * sizeof(struct family_t));
    myRecs->nrecs = 0;
    return myRecs;
}

// TODO: Is this parameter reasonable?
// Read about Family from a file
int getFamilyFromFile(char*** output, const char* fileName){
    char **lines = NULL;
    char *curr = NULL;
    size_t sz;
    size_t lineNum = 0;

    // get input
    FILE* fp = fopen(fileName,"r");  // TODO:检查一下文件是否打开
    if(fp == NULL){
        printf("Opening file %s failed\n", fileName);
        return -1;
    }
    while (newgetline(&curr, &sz, fp) >= 0){
        lines = realloc(lines, (lineNum + 1) * sizeof(*lines));
        lines[lineNum] = curr;
        curr = NULL;
        lineNum++;
    }
    fclose(fp);
    *output = lines;

    return lineNum;
}

// Parse the Family information from a formatted string and add it to the Registry
void addFamilyFromLine(registry* myRecs, char* line){
    const char delim[10] = DELIM;
    char *token;
    char** temp;

    // TODO: Modified to dynamically expand, not hardcoded
    temp = malloc(FAMILYMEMMAX*sizeof(char**));
    int p=0;

    // Split strings
    token = strtok(line, delim);
    while(token != NULL){
        temp[p++] = token;
        token = strtok(NULL, delim);
    }
    temp[p] = NULL;
    addFamily(myRecs, temp[0], temp[1], &temp[2]);

}

// free the registry along with those families stored in it
void freeRegistry(registry* reg){
    for(int i=0; i<reg->nrecs; i++){
        free(reg->myData[i].names);
    }
    free(reg->myData);
    free(reg);
}

int main(int argc,char **argv){
    registry* myRecs = initReg();

    char** familyLines=NULL;
    int familyNum=0;

    if(argc == 1){
        // If no file name is specified, it is read from "Family.txt" by default
        familyNum = getFamilyFromFile(&familyLines, "Family.txt");
        for(int i=1; i<familyNum; i++){
            addFamilyFromLine(myRecs, familyLines[i]);
        }
    }else{
        // read from multiple files
        for(int i=1; i<argc; i++){
            familyNum = getFamilyFromFile(&familyLines, argv[i]);
            for(int i=1; i<familyNum; i++){
                addFamilyFromLine(myRecs, familyLines[i]);
            }
        }
    }

    // Count the total number of family number
    int sum=0;
    for(int i=0; i<myRecs->nrecs; i++){
        sum += myRecs->myData[i].size;
    }

    printf("\nThere were %d people in total\n\n", sum);

    printf("===========before sort===========\n");

    printFamily(myRecs);

    sortFamily(myRecs);

    printf("===========after sort===========\n");

    printFamily(myRecs);

    freeRegistry(myRecs);

    printf("===========result===========\n");

    for(int i=0;i<myRecs->nrecs;i++){
        printf("Family %s: %ld members\n",myRecs->myData[i].surname, myRecs->myData[i].size);
    }

    return EXIT_SUCCESS;
} 

