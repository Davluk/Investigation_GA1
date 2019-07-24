#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "GeneticProgramming.cpp"

const int max_g = 50;
const int pop_size = 1000;
const int max_depth = 2;

const double m_rate = 1.0f;//0.1f
const double c_rate = 1.0f;
const double cross_prop = 0.2f;
	
const int fill_opt = HALF_FILL;
const int m_opt = SUBT_MUT;//oper_mut		
const int c_opt = PROP_CROSS;
const int s_opt = TOURN;

const int VAR_SIZ =2;
const int samples =100;

enum nodeTypes{_unop,_binop,_var,_term};													
std::vector<double*> _values;
//std::vector<std::string> terRep 	{"1","2","3","4","5","6","7","8","9"};
std::vector<std::string> terRep 	{"1","2","3","4","5","6","7","8","9","sqrt(2)","sqrt(3)","pi","e"};
std::vector<std::string> binOpRep	{"+","-","*","/"};
//std::vector<std::string> binOpRep {"+","-","*","/","^"};
std::vector<std::string> unOpRep	{"exp","ln","sin","cos"};
std::vector<std::string> varRep		{ "x","y"};//the last element is the F(x)											


double f(double x){ return (x-4)*(x+5)*(x-6)*(x+7); }
//double f(double x){ return 67.0f*x*x*x*x+100*x*x*x+49.0f*x*x + 80.7f*x + 100; }
//double f(double x){ return x*x*x*x*x*x + 0.0345f*x*x + x; }

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
	if(coin>=0){//coin>=0
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
		temp_vect[0] = (double)index/10.0f+2.0f;
		temp_vect[1] = f(temp_vect[0])+(double)(rand()%1000000/10000000.0f);
		_values.push_back(temp_vect);
	}

	GenAlg<metaD,double>* mygenalg = newGA(fill_opt,m_opt,c_opt,s_opt,pop_size,max_depth,max_g,m_rate,c_rate,cross_prop,EVUEXP,EVBEXP,GT,GETO,GETL,IND,IBN,IVR,GVI,GEI,GCHR);
	
	printf("INIT POBLATION \n\n");
	initPobRec(mygenalg,VAR_SIZ,_values,(int)_values.size(),0);

	//printPobStatus(mygenalg);
	//printf("\n");
	printf("\n#############################################\n####################POB#########################\n");
	printBestIndiv(mygenalg);

	GENETICPROSSES(mygenalg,VAR_SIZ,_values,(int)_values.size());

	printf("#############################################\n####################POB#########################\n");
	printBestIndiv(mygenalg);	

	std::cout<< "holamundo\n";
	return 0;
}
