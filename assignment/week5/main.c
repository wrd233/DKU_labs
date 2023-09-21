#include <stdio.h>
#include <stdlib.h>

enum state_t{
    IntegerPart,
    FractionPart,
    ExponentPart,
    Error,
    End
};

typedef struct{
    int isNegative;
    // for the integer part
    int integer;
    // for the fraction part
    int fraction;
    //for the exponent part
    int exponent;
    int fractionBit;
}double_t;

//function prototype
// all these three functions are used to update each part number,
// and finish the change of state for FSM.
enum state_t updateInteger(char c, int* result, int* isNegative);
enum state_t updateExponent(char c, int* result);
enum state_t updateFranction(char c, int* result, int* bits);

//finish the transform between double_t -> double
void transform(double* number, double_t userDouble);


/*
 * @ brief: this function can handle many types of input, like:
 * +3, -3, +3.2, -4.5, 23.12e12, 22.13e-13, 22E1, .01, and so on.
 * @ return 1: represent the input is valid
 * @ return -1: represnt the input is illegal
 */
int getDoubleInput(double* number){

    // get user input
    char* str[30];
    fgets(str, 30, stdin);

    //empty check
    if(str[0] == '\n'){
        printf("it seems you input do not empty anything\n");
        return -1;
    }

    //init state
    double_t userDouble = {0, 0, 0, 0, 0};

    // jump all the spaces before the number
    char* p = str;
    //empty check
    if(*p == '\n'){
        printf("it seems you do not input anything\n");
    }
    while (*p == ' ') p++;

    //create fsm （finite state machine）
    //define your init state
    enum state_t state = IntegerPart;
    while (state != Error && state != End){
        switch (state) {
            case IntegerPart:
                state = updateInteger(*p, &userDouble.integer, &userDouble.isNegative);
                break;
            case FractionPart:
                state = updateFranction(*p, &userDouble.fraction, &userDouble.fractionBit);
                break;
            case ExponentPart:
                state = updateExponent(*p, &userDouble.exponent);
                break;
        }
        p++;
    }
    //if enter error state
    if(state == Error){
        printf("it seems input is invalid\n");
        return -1;
    }else{
        //transform number to the user double
        transform(number, userDouble);
        return 1;
    }

}

//use can run this test_Case, and follow the indication to finish the test
//you can also write your own test case, because this function cannot test all situations.
void test_case();

int main() {
    //test_case();
    double result = 0;
    int isValid;
    //test template
    //input -1234 to quit this test
    while(result != -1234.0 ){
        printf("please input a number\n");
        isValid = getDoubleInput(&result);
        if(isValid == 1){
            printf("input is valid\n");
            printf("input: %lf\n", result);
        }else{
            printf("input is illegal\n");
        }
    }
    return 0;
}

enum state_t updateInteger(char c, int* result, int* isNegative){

    //check whether is first time to this function
    static int isFirst = 0;

    //check whether it is the first bit
    if(isFirst == 0){
        isFirst = 1;
        //check whether it is '+/-'
        if(c == '-' || c == '+'){
            if(c == '-') *isNegative = 1;
            return IntegerPart;
        }
    }

    //check whether it is a number
    if('0' <= c && c <= '9') {
        *result = *result * 10;
        *result = *result + c - '0';
        return IntegerPart;
    }
    else if(c == '.'){
        isFirst = 0;
        return FractionPart;
    }
    else if (c == 'e' || c == 'E'){
        isFirst = 0;
        return ExponentPart;
    }
    else if(c == '\n' || c == ' '){
        isFirst = 0;
        return End;
    }else{
        isFirst = 0;
        return Error;
    }
}

enum state_t updateExponent(char c, int* result){

    //check whether is first time to this function
    static int isFirst = 0;
    static int isPositive = 1;

    //check whether it is the first bit
    if(isFirst == 0){
        isFirst = 1;
        //check whether it is '+/-'
        if(c == '-' || c == '+'){
            if(c == '-') isPositive = -1;
            return ExponentPart;
        }
    }

    //check whether it is a number
    if('0' <= c && c <= '9') {
        //next bit
        *result = *result * 10;
        *result = *result + c - '0';
        return ExponentPart;
    }
    else if(c == '\n' || c == ' '){
        *result = *result * isPositive;
        isFirst = 0;
        isPositive = 1;
        return End;
    }else{
        isFirst = 0;
        isPositive = 1;
        return Error;
    }
}

enum state_t updateFranction(char c, int* result, int* bits){

    //check whether it is a number
    if('0' <= c && c <= '9') {
        *bits = *bits + 1;
        *result = *result * 10;
        *result = *result + c - '0';
        return FractionPart;
    }
    else if(c == 'e' || c == 'E'){
        return ExponentPart;
    }else if(c == '\n' || c == ' '){
        return End;
    }else{
        return Error;
    }
}

void transform(double* number, double_t userDouble){

    double franction = (double)userDouble.fraction;

    //get real franction value
    while (userDouble.fractionBit != 0) {
        franction = franction / 10;
        userDouble.fractionBit--;
    }


    if(userDouble.integer >= 0)
        *number = franction + userDouble.integer;
    else
        *number = -franction + userDouble.integer;


    //exponent
    while(userDouble.exponent != 0){
        if(userDouble.exponent > 0){
            userDouble.exponent--;
            *number = *number * 10;
        }
        if(userDouble.exponent < 0){
            userDouble.exponent++;
            *number = *number / 10;
        }
    }
    if(userDouble.isNegative == 1)
        *number = -*number;

}
void test_case(){
    //test
    double result;
    //please input 12.23
    printf("please input 12.23\n");
    getDoubleInput(&result);
    if(result - 12.23 < 0.000001){
        printf("Test 1 Pass: expected 12.23, actual 12.23\n");
    }else{
        printf("Test 1 Fail: expected 12.23, actual:%lf\n", result);
    }

    //please input .12
    printf("please input .12\n");
    getDoubleInput(&result);
    if(result - 0.12 < 0.000001){
        printf("Test 2 Pass: expected 0.12, actual 0.12\n");
    }else{
        printf("Test 2 Fail: expected 0.12, actual:%lf\n", result);
    }

    //please input -12
    printf("please input -12\n");
    getDoubleInput(&result);
    if(result - (-12.0) < 0.000001){
        printf("Test 3 Pass: expected -12.0, actual -12.0\n");
    }else{
        printf("Test 3 Fail: expected -12.0, actual:%lf\n", result);
    }

    //please input 1.2e2
    printf("please input 1.2e2\n");
    getDoubleInput(&result);
    if(result - 120.0 < 0.000001){
        printf("Test 4 Pass: expected 120, actual 120\n");
    }else{
        printf("Test 4 Fail: expected 120, actual:%lf\n", result);
    }

    //please input 12a.12
    printf("please input 12a.12\n");
    if(getDoubleInput(&result) == -1){
        printf("Test 5 Pass: can check error in the input\n");
    }else{
        printf("Test 5 Fail: cannot check error in the input\n");
    }

    //please input <enter>
    printf("please input <enter>\n");
    if(getDoubleInput(&result) == -1){
        printf("Test 6 Pass: can check empty input\n");
    }else{
        printf("Test 6 Fail: cannot check empty input\n");
    }
};
