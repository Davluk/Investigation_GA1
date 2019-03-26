#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "GeneticProgramming.cpp"

const int pop_size = 100;
const int max_depth = 3;
const int fill_opt = HALF_HALF;
const int m_opt = LEAF_MUT;
const int c_opt = RAND_CROSS;
const int s_opt = TOURN;
const float m_rate = 0.05f;
const float c_rate = 0.90f;

const int TER_SIZ =9;
const int OP_SIZ  =4;
const int VAR_SIZ =2;

enum nodeTypes{_op,_var,_term};
float _values[][VAR_SIZ]={{0.0f,0.0f},{1.0f,1.0f},{2.0f,4.0f},{3.0f,9.0f},{4.0f,16.0f},{5.0f,25.0f},
						   {6.0f,36.0f},{7.0f,49.0f},{8.0f,64.0f},{9.0f,81.0f},{10.0f,100.0f},
						   {11.0f,121.0f},{12.0f,144.0f},{13.0f,169.0f},{14.0f,196.0f},{15.0f,225.0f},
						   {16.0f,256.0f},{17.0f,289.0f},{18.0f,324.0f},{19.0f,361.0f},{20.0f,400.0f}};
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
	if(coin>2)
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
float GT(mD _metadata){return (float)_metadata.data+1;}

// para cuando la funcion es un arbol de desiciones | clasificador
int INTERPRETER(mD _metadata){return _metadata.data;}

/* conjunto de funciones que evaluan la expresion */
float ADD(float a,float b){return a+b;}
float SUB(float a,float b){return a-b;}
float MUL(float a,float b){return a*b;}
float DIV(float a,float b){return (b==0)?(a/0.000001f):(a/b);}

float EvEx(int selection,float a,float b)
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
	GenAlg<mD,float>* mygenalg = newGA(fill_opt,m_opt,c_opt,s_opt,pop_size,max_depth,m_rate,c_rate,0.7f,EvEx,GT,GETO,GETL,IN,IV,GVI,GEI,GCR);
	printf("INIT POBLATION \n\n");
	initPobRec(mygenalg,_values,(size_t)sizeof(_values)/sizeof(_values[0]),0);

	printPobStatus(mygenalg);
	CrossIndivs(mygenalg,&mygenalg->INDIVIDUALS[0],&mygenalg->INDIVIDUALS[1],_values,(size_t)(sizeof(_values)/(sizeof(_values[0]))));
	SELECTION(mygenalg);
	printf("#############################################\n####################POB#########################\n");
	CROSSOVER(mygenalg,_values,(size_t)sizeof(_values)/sizeof(_values[0]));
	printPobStatus(mygenalg);

	std::cout<< "holamundo";
	return 0;
}
