#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "GeneticProgramming.cpp"

const int max_g = 10000;
const int pop_size = 1000;
const int max_depth = 5;

const double m_rate = 0.5f;
const double c_rate = 0.90f;
const double cross_prop = 0.7f;

const int fill_opt = RAND_FILL;
const int m_opt = BIT_MUT;
const int c_opt = RAND_CROSS;
const int s_opt = TOURN;

const int TER_SIZ =9;
const int OP_SIZ  =4;
const int VAR_SIZ =2;

enum nodeTypes{_var,_op,_term};
double _values[20][VAR_SIZ];
double func(double x){return 9*x*x*x + 2*x*x + x + 4;}
//*/

char terRep[TER_SIZ]= {'1','2','3','4','5','6','7','8','9'};
char opRep[OP_SIZ] = {'+','-','*','/'};
char varRep[VAR_SIZ]= {'x','y'};//the last element is the F(x)

typedef struct METADATA{ 
	int data; int type;
	METADATA(){ data = 0; type = 0; }
} mD;

mD GETL()
{
	mD tempMD;
	int coin = rand()%4;
	if(coin>=2)
	{ tempMD.data = rand()%sizeof(terRep)/sizeof(terRep[0]); tempMD.type=_term; }
	else
	{ tempMD.data = rand()%((sizeof(varRep)/sizeof(varRep[0]))-1); tempMD.type=_var; }
	return tempMD;
}

mD GETO(){ 
	mD tempMD;
	tempMD.data = rand()%sizeof(opRep)/sizeof(opRep[0]); tempMD.type = _op;
	return tempMD;
}

char GCR(mD _metadata){
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
bool IN(mD _metadata){ return _metadata.type==_op; }
bool IV(mD _metadata){ return _metadata.type==_var; }

int   GVI(mD _metadata){return _metadata.data;}
int   GEI(mD _metadata){return _metadata.data;}
double GT(mD _metadata){return (double)_metadata.data+1;}

// para cuando la funcion es un arbol de desiciones | clasificador
int INTERPRETER(mD _metadata){return _metadata.data;}

/* conjunto de funciones que evaluan la expresion */
double ADD(double a,double b){return a+b;}
double SUB(double a,double b){return a-b;}
double MUL(double a,double b){return a*b;}
double DIV(double a,double b){return (b==0)?(a/0.000000000001f):(a/b);}

double EvEx(int selection,double a,double b)
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
	srand(time(NULL));
	for(int index=0;index<20;index++)
	{
		_values[index][0]=(double)(index/(double)1);
		_values[index][1]=func(_values[index][0]);
	}
	GenAlg<mD,double>* mygenalg = newGA(fill_opt,m_opt,c_opt,s_opt,pop_size,max_depth,max_g,m_rate,c_rate,cross_prop,EvEx,GT,GETO,GETL,IN,IV,GVI,GEI,GCR);
	
	printf("INIT POBLATION \n\n");
	initPobRec(mygenalg,_values,(size_t)sizeof(_values)/sizeof(_values[0]),0);

	printf("#############################################\n####################POB#########################\n");
	printBestIndiv(mygenalg);

	GENETICPROSSES(mygenalg,_values,(size_t)sizeof(_values)/sizeof(_values[0]));

	printf("#############################################\n####################POB#########################\n");
	printBestIndiv(mygenalg);
	//printPobStatus(mygenalg);

	std::cout<< "holamundo";
	return 0;
}
