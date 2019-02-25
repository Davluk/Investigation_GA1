#include<stdio.h>
#include<stdlib.h>
#include<time.h>

enum varCode{ x, y };
enum opCode{ add,sub,mul,div };
int variables[]={x,y};

float add_func(float a,float b){ return a+b; }
float sub_func(float a,float b){ return a-b; }
float mul_func(float a,float b){ return a*b; }
float div_func(float a,float b){ return (b==0)?(a/b):(a/0.001); } //divition protected

int main(int argc, char const *argv[])
{
    srand ((int)time(NULL));
    float (*operations[])(float,float)={add_func,sub_func,mul_func,div_func};
    

    return 0;
}
