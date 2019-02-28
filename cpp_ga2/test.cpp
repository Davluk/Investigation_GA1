#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Node.cpp"

const int TERMINAL_SIZE =9;
const int OPERANDS_SIZE =4;
const int VARIABLE_SIZE =2;

enum nodeTypes{_op,_var,_term};
char terRep[TERMINAL_SIZE]= {'1','2','3','4','5','6','7','8','9'};
char opRep[OPERANDS_SIZE] = {'+','-','*','/'};
char varRep[VARIABLE_SIZE]= {'x','y'};

typedef struct METADATA{ 
	int data; int type;
	METADATA(){ data = 0; type = 0; }
} metaD;

template<typename T>
int countElements(T elements[]) {return (int)sizeof(elements)/sizeof(elements[0]);} 

metaD GETLEAF()
{
	metaD tempMD;
	int coin = rand()%4;
	if(coin>2)
	{ tempMD.data = rand()%countElements( terRep ); tempMD.type=_term; }
	else
	{ tempMD.data = rand()%countElements( varRep ); tempMD.type=_var; }
	return tempMD;
}

metaD GETOP(){ 
	metaD tempMD;
	tempMD.data = rand()%countElements( opRep ); tempMD.type = _op;
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

bool ISNODE(metaD _metadata){ return _metadata.data==_op; }

/*
	funciones de control :
	- interpreter	
	- getLeaft
	- getOp
	- isNode
	- getCharRep
*/





int main(int argc, char const *argv[])
{
	srand((int)time(NULL));
	Node<metaD> *myNode;
	myNode=newNode(RAND_FILL,4,GETOP(),&GETLEAF,&GETOP);
	PrintPosOrder( myNode, &ISNODE, &GETCHARREP );

	return 0;
}