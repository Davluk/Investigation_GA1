#include<stdio.h>
#include<stdlib.h>
#include<time.h>



enum varCode{ x, y };
enum opCode{ add,sub,mul,divp };
int variables[]={x,y};
int operadores[]={add,sub,mul,divp};

char repOperators[] = {'+','-','*','/'};
char rePVariables[] = {'x','y'};

float add_func(float a,float b){ return a+b; }
float sub_func(float a,float b){ return a-b; }
float mul_func(float a,float b){ return a*b; }
float div_func(float a,float b){ return (b!=0)?(a/b):(a/0.001); } //divition protected

void executeFunctions(float a,float b,float (*_multi)(float,float),float (*_divi)(float,float)){
    printf("the result of multiply a,b is %f\n",_multi(a,b));
    printf("the result of divide a,b is %f\n",_divi(a,b));
}

int main(int argc, char const *argv[])
{
    srand ((int)time(NULL));
    float (*operations[4])(float,float)={};
    operations[add]=add_func;
    operations[sub]=sub_func;
    operations[mul]=mul_func;
    operations[divp]=div_func;

    executeFunctions(10,4,mul_func,div_func);



    return 0;
}
