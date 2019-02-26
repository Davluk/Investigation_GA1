#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include"treeManager.cpp"


enum varCode{ x, y };
enum opCode{ add,sub,mul,divp };
int variables[]={x,y};
int operadores[]={add,sub,mul,divp};
int terminales[]={1,2,3,4,5};

char repOperators[] = {'+','-','*','/'};
char repVariables[] = {'x','y'};

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

    float** list_values;
    list_values[0]=new float[2];
    list_values[0][x]=0.5f;
    list_values[0][y]=0.8f;
    list_values[1]=new float[2];
    list_values[0][x]=0.9f;
    list_values[0][y]=4.9f;

    treeManager trm(operadores,terminales,variables,repOperators,repVariables,operations);
    tree* mytree = trm.newTree(5,list_values);

    //executeFunctions(10,4,mul_func,div_func);



    return 0;
}
