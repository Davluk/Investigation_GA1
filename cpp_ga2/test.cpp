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
float _values[][VARIABLE_SIZE]={{0.0f,0.0f},{1.0f,1.0f},{2.0f,4.0f},{3.0f,9.0f},{4.0f,16.0f},{5.0f,25.0f}};
char terRep[TERMINAL_SIZE]= {'1','2','3','4','5','6','7','8','9'};
char opRep[OPERANDS_SIZE] = {'+','-','*','/'};
char varRep[VARIABLE_SIZE]= {'x','y'};//the last element is the F(x)

template<typename T,typename U>
float cuadraticError(Node<T> *_current_dude,U (*exExp)(int,U,U),bool (*isNode)(T),bool (*isVar)(T),int (*getVrIn)(T),U (*getTer)(T),int (*getExpInd)(T),int Function_desired_value,U (*_list_of_values)[VARIABLE_SIZE],size_t rows)
{
	float error =0.0f;
	for(int index = 0;index<rows;index++)
	{
		error+= pow(_list_of_values[index][Function_desired_value]-evalFunction(_current_dude,exExp,isNode,isVar,getVrIn,getTer,getExpInd,_list_of_values[index]),2)/rows;
	}
	return error;
}

typedef struct METADATA{ 
	int data; int type;
	METADATA(){ data = 0; type = 0; }
} metaD;

metaD GETLEAF()
{
	metaD tempMD;
	int coin = rand()%4;
	if(coin>2)
	{ tempMD.data = rand()%sizeof(terRep)/sizeof(terRep[0]); tempMD.type=_term; }
	else
	{ tempMD.data = rand()%((sizeof(varRep)/sizeof(varRep[0]))-1); tempMD.type=_var; }
	return tempMD;
}

metaD GETOP(){ 
	metaD tempMD;
	tempMD.data = rand()%sizeof(opRep)/sizeof(opRep[0]); tempMD.type = _op;
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
	Node<metaD> *myNode1;
	Node<metaD> *myNode2;
	Node<metaD> *subnode1=new Node<metaD>();
	Node<metaD> *subnode2=new Node<metaD>();

	int nodeCount1=0;
	int nodeCount2=0;
	int half_nodes1=0;
	int half_nodes2=0;

	myNode1=newNode(HALF_HALF,3,GETOP(),&GETLEAF,&GETOP);
	myNode2=newNode(HALF_HALF,3,GETOP(),&GETLEAF,&GETOP);

	PrintPosOrder( myNode1, ISNODE, GETCHARREP );
	printf("\n\n");
	PrintPosOrder( myNode2, ISNODE, GETCHARREP );
	printf("\n");

	printf("\n evaluation_result1: %f\n",evalFunction(myNode1,EVEXP,ISNODE,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values[0]));
	printf("\n cuadraticError1: %f\n",cuadraticError(myNode1,EVEXP,ISNODE,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,1,_values,(size_t)sizeof(_values)/sizeof(_values[0])));

	printf("\n evaluation_result2: %f\n",evalFunction(myNode2,EVEXP,ISNODE,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values[0]));
	printf("\n cuadraticError2: %f\n",cuadraticError(myNode2,EVEXP,ISNODE,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,1,_values,(size_t)sizeof(_values)/sizeof(_values[0])));

	nodeCount1 = nodeCounter(myNode1,ISNODE);
	nodeCount2 = nodeCounter(myNode2,ISNODE);

	half_nodes1 = (int)ceil(nodeCount1/5);
	half_nodes2 = (int)ceil(nodeCount2/2);

	printf("number of nodes 1: %d, half_ceiled: %d\n",nodeCount1,half_nodes1);
	printf("number of nodes 2: %d, half_ceiled: %d\n",nodeCount2,half_nodes2);

	getIndexedSubTree(half_nodes1,myNode1,subnode1,ISNODE);
	getIndexedSubTree(half_nodes2,myNode2,subnode2,ISNODE);

	printf("selected subtree1: \n");
	PrintPosOrder( subnode1,ISNODE,GETCHARREP);
	printf("\n");
	printf("selected subtree2: \n");
	PrintPosOrder( subnode2,ISNODE,GETCHARREP);

	printf("\nsubstitution...\n");
	setIndexedSubTree(half_nodes1,myNode1,subnode2,ISNODE);
	setIndexedSubTree(half_nodes2,myNode2,subnode1,ISNODE);

	PrintPosOrder(myNode1,ISNODE,GETCHARREP);
	printf("\n");
	PrintPosOrder(myNode2,ISNODE,GETCHARREP);
	
	return 0;
}
