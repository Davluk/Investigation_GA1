#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<exception>
#include<iostream>
#include"Node.cpp"

const int TERMINAL_SIZE =9;
const int OPERANDS_SIZE =4;
const int VARIABLE_SIZE =2;

enum nodeTypes{_op,_var,_term};
float _values[][VARIABLE_SIZE]={{0.0f,-6.0f},{0.1f,-1.0f},{2.0f,4.0f},{3.0f,9.0f},{4.0f,14.0f}};
char terRep[TERMINAL_SIZE]= {'1','2','3','4','5','6','7','8','9'};
char opRep[OPERANDS_SIZE] = {'+','-','*','/'};
char varRep[VARIABLE_SIZE]= {'x','y'};

typedef struct METADATA{ 
	int data; int type;
	METADATA(){ data = 0; type = 0; }
} metaD;

template<typename T>
int countElements(T elements[]) {return elements.count();} 

metaD GETLEAF()
{
	metaD tempMD;
	int coin = rand()%4;
	if(coin>2)
	{ tempMD.data = rand()%TERMINAL_SIZE; tempMD.type=_term; }
	else
	{ tempMD.data = rand()%VARIABLE_SIZE; tempMD.type=_var; }
	return tempMD;
}

metaD GETOP(){ 
	metaD tempMD;
	tempMD.data = rand()%OPERANDS_SIZE; tempMD.type = _op;
	return tempMD;
}

char GETCHARREP(metaD _metadata){
	switch(_metadata.type)
	{
		case _op: 	return opRep [_metadata.data]; break; 
		case _term: return terRep[_metadata.data]; break; 
		case _var:  return varRep[_metadata.data]; break; 
		default:	return '.';
	}
	return '.';
}
//para cuando sea una funcion
bool ISNODE(metaD _metadata){ return _metadata.type==_op; }
bool ISVAR (metaD _metadata){ return _metadata.type==_var; }

int   GETVARINDEX(metaD _metadata){return _metadata.data;}
int   GETEXPINDEX(metaD _metadata){return _metadata.data;}
float GETTERMINAL(metaD _metadata){return (float)_metadata.data+1;}

// para cuando la funcion es un arbol de desiciones | clasificador
int INTERPRETER(metaD _metadata){return _metadata.data;}

/* conjunto de funciones que evaluan la expresion */
float ADD(float a,float b){return a+b;}
float SUB(float a,float b){return a-b;}
float MUL(float a,float b){return a*b;}
float DIV(float a,float b){return (b==0)?(a/0.000001f):(a/b);}

float EVEXP(int selection,float a,float b)
{
	switch(selection){
		case 0: return ADD(a,b);
		case 1: return SUB(a,b);
		case 2: return MUL(a,b);
		case 3: return DIV(a,b);
		default : return 1.0f;
	}
	return 1.0f;
}


int main(int argc, char const *argv[])
{
	srand((int)time(NULL));
	Node<metaD> *myNode;
	Node<metaD> *subnode=new Node<metaD>();
	int nodeCount=0;
	int half_nodes=0;
	myNode=newNode(HALF_HALF,3,GETOP(),&GETLEAF,&GETOP);
	PrintPosOrder( myNode, ISNODE, GETCHARREP );
	printf("\n evaluation_result: %f\n",evalFunction(myNode,EVEXP,ISNODE,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values[0]));
	nodeCount = nodeCounter(myNode,ISNODE);
	half_nodes = (int)ceil(nodeCount/2);
	printf("number of nodes : %d, half_ceiled: %d\n",nodeCount,half_nodes);
	findIndexedSubTree(half_nodes,myNode,subnode,ISNODE,GETCHARREP);
	//printf("selected subtree: \n");
	PrintPosOrder( myNode ,ISNODE,GETCHARREP);printf("\n");
	PrintPosOrder( subnode,ISNODE,GETCHARREP);
	return 0;
}