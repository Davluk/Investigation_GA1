#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<exception>
#include<iostream>
#include<string>
#include<vector>
#include"Node.cpp"


enum nodeTypes{_unop,_binop,_var,_term};													
std::vector<double*> _values;
//std::vector<std::string> terRep 	{"1","2","3","4","5","6","7","8","9"};
std::vector<std::string> terRep 	{"1","2","3","4","5","6","7","8","9","sqrt(2)","sqrt(3)","pi","e"};
std::vector<std::string> binOpRep	{"+","-","*","/"};
//std::vector<std::string> binOpRep {"+","-","*","/","^"};
std::vector<std::string> unOpRep	{"exp","ln","sin","cos"};
std::vector<std::string> varRep		{ "x","y"};//the last element is the F(x)												

double f(double x){ return x*x; }												

const int DEPTH = 3;
const int VARIABLE_SIZE = 2;
const float PROP = 0.55;
const int samples =10;

typedef struct mD{ 
	int data; int type;
	mD(){ data = 0; type = 0; }
} metaD;

metaD GETL()
{
	metaD tempMD;
	int coin = rand()%4;
	if(coin>2)
	{ tempMD.data = rand()%terRep.size(); tempMD.type=_term; }
	else
	{ tempMD.data = rand()%(varRep.size()-1); tempMD.type=_var; }
	return tempMD;
}

metaD GETO(){ 
	metaD tempMD;
	int coin = rand()%4;
	if(coin>1){//coin>=0
		tempMD.data = rand()%binOpRep.size(); tempMD.type = _binop;
	}else{
		tempMD.data = rand()%unOpRep.size(); tempMD.type = _unop;
	}
	return tempMD;
}

std::string GCHR(metaD _metadata){
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
bool IND (metaD _metadata){ return (_metadata.type==_unop||_metadata.type==_binop); }
bool IBN (metaD _metadata){ return _metadata.type==_binop; }
bool IVR (metaD _metadata){ return _metadata.type==_var; }

int   GVI(metaD _metadata){ return _metadata.data; }
int   GEI(metaD _metadata){ return _metadata.data; }
double GT(metaD _metadata){ 
	return _metadata.data <9  ? (double)_metadata.data+1 :
		   _metadata.data==9  ? sqrt(2) :
		   _metadata.data==10 ? sqrt(3) :
		   _metadata.data==11 ? 3.14159265358979323846f : exp(1) ;
}

// para cuando la funcion es un arbol de desiciones | clasificador
int INTERPRETER(metaD _metadata){return _metadata.data;}

double EVBEXP(int selection,double a,double b)
{
	switch(selection){
		case 0: return a+b;
		case 1: return a-b;
		case 2: return a*b;
		case 3: return (b==0)?(a/0.0000000000000000000000000000000000001f):(a/b);
		case 4: return pow(a,b);
		default : return 1.0f;
	}
	return 1.0f;
}
double EVUEXP(int selection,double a)
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
	srand((int)time(NULL));
	for(int index = 0; index<samples; index++)
	{
		double* temp_vect= (double*)calloc(2,sizeof(double));
		temp_vect[0] = index+2.0f;
		temp_vect[1] = f(temp_vect[0])+(double)(rand()%1000000/10000000.0f);
		_values.push_back(temp_vect);
	}

	Node<metaD> *myNode1;
	Node<metaD> *myNode2;
	Node<metaD> *subnode1=new Node<metaD>();
	Node<metaD> *subnode2=new Node<metaD>();

	int nodeCount1=0;
	int nodeCount2=0;
	int half_nodes1=0;
	int half_nodes2=0;

	printf("...\n");
	myNode1=newNode(HALF_FILL,DEPTH,GETO(),IBN,&GETL,&GETO);
	myNode2=newNode(HALF_FILL,DEPTH,GETO(),IBN,&GETL,&GETO);

	PrintTree( myNode1, IND , IBN , GCHR );
	printf("\n\n");
	PrintTree( myNode2, IND , IBN , GCHR );
	printf("\n");

	printf("\n evaluation_result1: %f\n",evalFunction(myNode1,EVBEXP,EVUEXP,IND,IBN,IVR,GVI,GT,GEI,_values[0]));
	printf("\n cuadraticError1: %f\n",cuadraticError(myNode1,EVBEXP,EVUEXP,IND,IBN,IVR,GVI,GT,GEI,_values,(int)_values.size(),varRep.size()));

	printf("\n evaluation_result2: %f\n",evalFunction(myNode2,EVBEXP,EVUEXP,IND,IBN,IVR,GVI,GT,GEI,_values[0]));
	printf("\n cuadraticError2: %f\n",cuadraticError(myNode2,EVBEXP,EVUEXP,IND,IBN,IVR,GVI,GT,GEI,_values,(int)_values.size(),varRep.size()));

	nodeCount1 = nodeCounter(myNode1,IND,IBN);
	nodeCount2 = nodeCounter(myNode2,IND,IBN);

	half_nodes1 = (int)ceil(nodeCount1*PROP);
	half_nodes2 = (int)ceil(nodeCount2*PROP);

	printf("number of nodes 1: %d, half_ceiled: %d\n",nodeCount1,half_nodes1);
	printf("number of nodes 2: %d, half_ceiled: %d\n",nodeCount2,half_nodes2);

	getIndexedSubTree(half_nodes1,myNode1,subnode1,IND,IBN);
	getIndexedSubTree(half_nodes2,myNode2,subnode2,IND,IBN);

	printf("selected subtree1: \n");
	PrintTree( subnode1,IND,IBN,GCHR);
	printf("\n");
	printf("selected subtree2: \n");
	PrintTree( subnode2,IND,IBN,GCHR);

	printf("\nsubstitution...\n");
	setIndexedSubTree(half_nodes1,myNode1,subnode2,IND,IBN);
	setIndexedSubTree(half_nodes2,myNode2,subnode1,IND,IBN);

	PrintTree(myNode1,IND,IBN,GCHR);
	printf("\n");
	PrintTree(myNode2,IND,IBN,GCHR);
	printf("\n");
	return 0;
}
