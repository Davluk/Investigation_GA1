#include <iostream>
#include "GeneticProgramming.cpp"

const int TER_SIZ =9;
const int OP_SIZ  =4;
const int VAR_SIZ =2;

enum nodeTypes{_op,_var,_term};
float _values[][VAR_SIZ]={{0.0f,0.0f},{1.0f,1.0f},{2.0f,4.0f},{3.0f,9.0f},{4.0f,16.0f},{5.0f,25.0f}};
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
	Indiv<mD>* myindiv = newIndiv(HALF_HALF,5,GETO,GETL,EvEx,IN,IV,GVI,GT,GEI,_values,(size_t)sizeof(_values)/sizeof(_values[0]));

	std::cout<< "holamundo";
	return 0;
}