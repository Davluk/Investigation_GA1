#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<exception>
#include<iostream>
#include<string>
#include"Node.cpp"

const int VARIABLE_SIZE = 2;
const int DEPTH = 3;

enum nodeTypes{_unop,_binop,_var,_term};													
float** _values;
std::basic_string<char> terRep[] = {"1","2","3","4","5","6","7","8","9","sqt(2)","sqt(3)","pi","e"};
std::basic_string<char> binOpRep[] = {"+","-","*","/","^"};
std::basic_string<char> unOpRep[]  = {"exp","ln","sin","cos"};
std::basic_string<char> varRep[] = { "x","y"};//the last element is the F(x)												

float f(float x){ return 9*x*x*x + 4*x*x; }

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
	int coin = rand()%4;
	if(coin>2){
		tempMD.data = rand()%sizeof(unOpRep)/sizeof(unOpRep[0]); tempMD.type = _unop;
	}else{
		tempMD.data = rand()%sizeof(binOpRep)/sizeof(binOpRep[0]); tempMD.type = _binop;
	}
	return tempMD;
}

std::basic_string<char> GETCHARREP(metaD _metadata){
	switch(_metadata.type)
	{
		case _binop:return binOpRep[_metadata.data]; break;
		case _unop: return unOpRep [_metadata.data]; break; 
		case _term: return terRep[_metadata.data]; break; 
		case _var:  return varRep[_metadata.data]; break; 
		default:	return ".";
	}
	return ".";
}
//para cuando sea una funcion
bool ISNODE(metaD _metadata){ return (_metadata.type==_unop||_metadata.type==_binop); }
bool ISBIN (metaD _metadata){ return _metadata.type==_binop; }
bool ISVAR (metaD _metadata){ return _metadata.type==_var; }

int   GETVARINDEX(metaD _metadata){ return _metadata.data; }
int   GETEXPINDEX(metaD _metadata){ return _metadata.data; }
float GETTERMINAL(metaD _metadata){ 
	return _metadata.data <9  ? (float)_metadata.data+1 :
		   _metadata.data==9  ? sqrt(2) :
		   _metadata.data==10 ? sqrt(3) :
		   _metadata.data==11 ? 3.14159265358979323846f : exp(1) ;
}

// para cuando la funcion es un arbol de desiciones | clasificador
int INTERPRETER(metaD _metadata){return _metadata.data;}

float EVBEXP(int selection,float a,float b)
{
	switch(selection){
		case 0: return a+b;
		case 1: return a-b;
		case 2: return a*b;
		case 3: return (b==0)?(a/0.000001f):(a/b);
		case 4: return pow(a,b);
		default : return 1.0f;
	}
	return 1.0f;
}
float EVUEXP(int selection,float a)
{
	switch(selection){
		case 0: return exp(a);
		case 1: return log(a);
		case 2: return sin(a);
		case 3: return cos(a);
		default : return 1.0f;
	}
	return 1.0f;
}


int main(int argc, char const *argv[])
{
	_values =(float**)calloc(40,sizeof(int*));
	for(int index = 0; index<40; index++)
	{
		_values[index] = (float*)calloc(2,sizeof(float));
		_values[index][0] = index/20.0f+1.0f;
		_values[index][1] = f(_values[index][0]);
	}

	srand((int)time(NULL));
	Node<metaD> *myNode1;
	Node<metaD> *myNode2;
	Node<metaD> *subnode1=new Node<metaD>();
	Node<metaD> *subnode2=new Node<metaD>();

	int nodeCount1=0;
	int nodeCount2=0;
	int half_nodes1=0;
	int half_nodes2=0;

	printf("...\n");
	myNode1=newNode(HALF_HALF,DEPTH,GETOP(),ISBIN,&GETLEAF,&GETOP);
	myNode2=newNode(HALF_HALF,DEPTH,GETOP(),ISBIN,&GETLEAF,&GETOP);

	PrintPosOrder( myNode1, ISNODE , ISBIN , GETCHARREP );
	printf("\n\n");
	PrintPosOrder( myNode2, ISNODE , ISBIN , GETCHARREP );
	printf("\n");

	printf("\n evaluation_result1: %f\n",evalFunction(myNode1,EVBEXP,EVUEXP,ISNODE,ISBIN,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values[0]));
	printf("\n cuadraticError1: %f\n",cuadraticError(myNode1,EVBEXP,EVUEXP,ISNODE,ISBIN,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values,VARIABLE_SIZE,(size_t)sizeof(_values)/sizeof(_values[0])));

	printf("\n evaluation_result2: %f\n",evalFunction(myNode2,EVBEXP,EVUEXP,ISNODE,ISBIN,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values[0]));
	printf("\n cuadraticError2: %f\n",cuadraticError(myNode2,EVBEXP,EVUEXP,ISNODE,ISBIN,ISVAR,GETVARINDEX,GETTERMINAL,GETEXPINDEX,_values,VARIABLE_SIZE,(size_t)sizeof(_values)/sizeof(_values[0])));

	nodeCount1 = nodeCounter(myNode1,ISNODE,ISBIN);
	nodeCount2 = nodeCounter(myNode2,ISNODE,ISBIN);

	half_nodes1 = (int)ceil(nodeCount1/2);
	half_nodes2 = (int)ceil(nodeCount2/2);

	printf("number of nodes 1: %d, half_ceiled: %d\n",nodeCount1,half_nodes1);
	printf("number of nodes 2: %d, half_ceiled: %d\n",nodeCount2,half_nodes2);

	getIndexedSubTree(half_nodes1,myNode1,subnode1,ISNODE,ISBIN);
	getIndexedSubTree(half_nodes2,myNode2,subnode2,ISNODE,ISBIN);

	printf("selected subtree1: \n");
	PrintPosOrder( subnode1,ISNODE,ISBIN,GETCHARREP);
	printf("\n");
	printf("selected subtree2: \n");
	PrintPosOrder( subnode2,ISNODE,ISBIN,GETCHARREP);

	printf("\nsubstitution...\n");
	setIndexedSubTree(half_nodes1,myNode1,subnode2,ISNODE,ISBIN);
	setIndexedSubTree(half_nodes2,myNode2,subnode1,ISNODE,ISBIN);

	PrintPosOrder(myNode1,ISNODE,ISBIN,GETCHARREP);
	printf("\n");
	PrintPosOrder(myNode2,ISNODE,ISBIN,GETCHARREP);
	
	return 0;
}
