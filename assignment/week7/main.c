#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>


#define panic(format, ...) do { \
    printf(format, ##__VA_ARGS__); \
    assert(0); \
} while (0)

struct matrixtag {
    double **values;
    size_t rows;
    size_t columns;
};

typedef struct matrixtag matrixt;

static void trimWhitespace(char *str){
    // Remove whitespace characters from the beginning of the string
    while (isspace(*str)) {
        str++;
    }

    // Remove whitespace characters from the end of the string
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';
}

static unsigned strToUnsigned(char* str, bool *success){
    unsigned res=0;
    *success = true;
    if(str==NULL){
        *success = false;
    }
    trimWhitespace(str);
    for(size_t i=0; str[i]!='\0'; i++){
        if(str[i]<'0' || str[i]>'9'){
            *success = false;
            return res;
        }
        res *= 10;
        res += str[i]-'0';
    }
    return res;
}

static bool isBlankLine(const char* str){
    if(str == NULL){return true;}
    for(size_t i=0; str[i]!='\0'; i++){
        if(!isspace(str[i])){
            return false;
        }
    }
    return true;
}

// TODO: 转换是否成功的检查
static double strToDouble(char* str, bool *success){
    *success = true;
    double res = atof(str);
    return res;
}

matrixt* multiply(matrixt* left, matrixt* right) {
    // write this
    matrixt* mtx=NULL;
    // TODO: check: 检测两个矩阵能否相乘
    if(left==NULL || right==NULL){panic("nullptr\n");}
    if(left->columns != right->rows){panic("Mismatched rows and columns\n");}

    // 普通的计算方法
    int n = left->columns;

    mtx = malloc(sizeof(*mtx));
    mtx->columns = right->columns;
    mtx->rows = left->rows;
    mtx->values = malloc(mtx->rows * sizeof(*mtx->values));
    for(size_t i=0; i<mtx->rows; i++){
        mtx->values[i] = malloc(mtx->columns * sizeof(double));
    }

    for(size_t i=0; i<mtx->rows; i++){
        for(size_t j=0; j<mtx->columns; j++){
            mtx->values[i][j] = 0;
            for(size_t k=0; k<n; k++){
                mtx->values[i][j] += left->values[i][k] * right->values[k][j];
            }
        }
    }

    return mtx;
    // TODO: 递归的方法进行计算
}

matrixt* readMatrix(const char* filename) {
    // write this
    FILE* fp = fopen(filename,"r");
    size_t linecap = 0;
    ssize_t linelen;
    char *line = NULL;
    int colNum=0;
    int rowNum=0;
    bool success;
    matrixt* mtx=NULL;

    // get Col number
    linelen = getline(&line, &linecap, fp);
    colNum = strToUnsigned(line, &success);
    if(!success){
        panic("wrong col format\n");
    }

    // get Row number
    linelen = getline(&line, &linecap, fp);
    rowNum = strToUnsigned(line, &success);
    if(!success){
        panic("wrong row format\n");
    }

    // build a matrixt
    mtx = malloc(sizeof(*mtx));
    mtx->columns = colNum;
    mtx->rows = rowNum;
    mtx->values = malloc(rowNum * sizeof(*mtx->values));
    for(size_t i=0; i<rowNum; i++){
        mtx->values[i] = malloc(colNum * sizeof(double));
    }

    // read the data
    bool doubleSuccess;
    for(size_t i=0; i<mtx->rows; i++){
        for(size_t j=0; j<mtx->columns; j++){
            // Remove empty rows
            do{
                getline(&line, &linecap, fp);
            }while(linelen > 0 && isBlankLine(line));

            if(linelen <=0){
                free(line);
                free(mtx);
                panic("There is not enough data\n");
            }
            double n = strToDouble(line, &doubleSuccess);
            if(!doubleSuccess){
                free(line);
                free(mtx);
                panic("wrong double format\n");
            }
            mtx->values[i][j] = n;
        }
    }

    // check:Excess data
    // Remove empty rows
    do{
        linelen = getline(&line, &linecap, fp);
    }while(linelen > 0 && isBlankLine(line));
    if(linelen > 0){
        panic("Excess data\n");
    }

    free(line);
    return mtx;
}

void printMatrix(matrixt* matrix) {
    // write this
    for(size_t i=0; i<matrix->rows; i++){
        for(size_t j=0; j<matrix->columns; j++){
            printf("%lf ",matrix->values[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void freeMatrix(matrixt* matrix) {
    // write this
}

int main(int argc, char** argv) {
    // write this
    if(argc<3){
        printf("参数不足\n");
    }

    // read from multiple files
    matrixt* matrixtArr[2];
    int matrixNum = argc-1;
    for(size_t i=1; i<argc; i++){
        matrixtArr[i-1] = readMatrix(argv[i]);
        printMatrix(matrixtArr[i-1]);
    }

    matrixt* midRes=NULL;
    for(size_t i=0; i<matrixNum-1; i++){
        midRes = multiply(matrixtArr[i], matrixtArr[i+1]);
        if(midRes == NULL){
            panic("计算错误\n");
        }
        printMatrix(midRes);
    }

    return EXIT_SUCCESS;
}