#include "Node.cpp"
#ifndef GEN_ALG
#define GEN_ALG

/* ************************************************************* 
*                                                              *
*           all the methods is for tree tratment               *
*                                                              *
***************************************************************/

/********************************************
*   constants for the genetic algorithm     *    
********************************************/
#define HALF_CROSS 0
#define PROP_CROSS 1
#define RAND_CROSS 2

/*
*
*/
#define LEAF_MUT 0
#define OPER_MUT 1
#define SUBT_BUT 2

/*********************************************
*   structure for tratment with individuals  *
*********************************************/
template<typename T>
struct Indiv
{
    Node<T>* chrom;
    float fitness; 
    Indiv(){}   
};

/********************************************
*   structure for the genetic algorithm     *
********************************************/  
template<typename T,typename U>
struct GenAlg{
    Indiv<T>** INDIVIDUALS;
    Indiv<T>** NEWINDIVIDUALS;
    int   poblation_size;
    int   indiv_init_size;
    int   selection_opt;
    int   fill_selection;
    float mutation_rate;
    float crossover_rate;
    //functions releated with the treatment of the tree individuals
    U       (*EVE)(int,U,U);    // evaluate expresion
    U       (*GTR)(T);          //get terminal
    T       (*GOP)();           //get operand
    T       (*GLF)();           //get leaf
    bool    (*IND)(T);          //is a node?
    bool    (*IVR)(T);          //is a variable?
    int     (*GVI)(T);          //get variable index(the variables can be in disorder)
    int     (*GEI)(T);          //get expreison index(the expresions can be in disorder)
    GenAlg(){}
};

template<typename T,typename U,std::size_t SIZE>
GenAlg<T,U>* newGA(int fillS,int pobSiz,int indvSiz,float mutrt,float crossrt, U (*eve)(), U (*gtr)(), T (*gop)(),T (*glf)(),bool (*ind)(),bool (*ivr)(),int (*gvi)(),int (*gei)(),U (*Vals)[SIZE],size_t sizeVal)
{

    GenAlg<T,U>* tmpGA = new GenAlg<T,U>();
    tmpGA->fill_selection = fillS;
    tmpGA->poblation_size = pobSiz;
    tmpGA->indiv_init_size = indvSiz;
    tmpGA->mutation_rate = mutrt;
    tmpGA->crossover_rate = crossrt;
    tmpGA->EVE = eve;
    tmpGA->GTR = gtr;
    tmpGA->GOP = gop;
    tmpGA->GLF = glf;
    tmpGA->IND = ind;
    tmpGA->IVR = ivr;
    tmpGA->GVI = gvi;
    tmpGA->GEI = gei;
    for(int index = 0;index<pobSiz;index++)
    {
        *(tmpGA->INDIVIDUALS[index])=newIndiv(fillS,indvSiz,gop,glf,eve,ind,ivr,gvi,gtr,gei,Vals,sizeVal);
    }
    return tmpGA;
}

/***************************************************************************************************************
*   FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*   IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
***************************************************************************************************************/
template<typename T,typename U,std::size_t  SIZE>
Indiv<T>* newIndiv(int FS,int D,T (*GO)(),T (*GL)(),U (*EE)(int,U,U),bool (*IN)(T),bool (*IV)(T),int (*GVI)(T),U (*GT)(T),int (*GEI)(T),U (*Vals)[SIZE],size_t sizeVal)
{
    Indiv<T>* temp_indiv = new Indiv<T>();
    temp_indiv->chrom = newNode(FS,D,GO(),GL,GO);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,EE,IN,IV,GVI,GT,GEI,Vals,sizeVal);
    return temp_indiv;
}

/************************************************************************************
*   the function apply a kind of mutation to a individual with selection Option     *
************************************************************************************/
template<typename T>
void MutateIndiv(int option,Indiv<T>* someIndiv,bool (*IN)(),bool (*IV)(),T (*GL)(),T (*GO)())
{
    Indiv<T>* tempInd;
}

/****************************************************************************************
*   the funcitons interchange the subtree of both individuals with selection Option     *
****************************************************************************************/
template<typename T>
void CrossIndivs(int Option,float porc,Indiv<T>* parentA,Indiv<T>* parentB,bool (*IN)())
{
    Node<T>* subIndivA;//saves the direction of the B subtree
    Node<T>* subIndivB;//saves the direction of the A subtree 
    int countOperandsA = nodeCounter(parentA,IN);
    int countOperandsB = nodeCounter(parentB,IN);
    int Index_A=0;
    int Index_B=0;
    switch (Option){
        case HALF_CROSS:// 50% of individual information will be interchanged 
            Index_A = (int)countOperandsA/2; Index_B = (int)countOperandsB/2; break;
        case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
            Index_A = (int)countOperandsA*(porc); Index_B = (int)countOperandsB*(1-porc); break;    
        case RAND_CROSS://aleatory index
            Index_A = rand()%countOperandsA; Index_B = rand()%countOperandsB; break;    
    }
    /*gets the indexed operand over the trees*/
    getIndexedSubTree(Index_A,parentA->chrom,subIndivA,IN);
    getIndexedSubTree(Index_B,parentB->chrom,subIndivB,IN);
    /* interchange the indexed operands*/
    setIndexedSubTree(Index_A,parentA->chrom,subIndivB,IN);
    setIndexedSubTree(Index_B,parentB->chrom,subIndivA,IN);

}

#endif