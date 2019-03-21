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
#define NO_MUT 3

/*
*
*/
#define TOURN 0
#define ROULT 1


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
    Indiv<T>* INDIVIDUALS;
    Indiv<T>* NEWINDIVIDUALS;
    int   poblation_size;
    int   indiv_init_size;
    int   selection_opt;
    int   fill_selection;
    int   m_selection;
    int   c_selection;
    int   s_selection;
    float mutation_rate;
    float crossover_rate;
    float cross_proportion;
    //functions releated with the treatment of the tree individuals
    U       (*EVE)(int,U,U);    // evaluate expresion
    U       (*GTR)(T);          //get terminal
    T       (*GOP)();           //get operand
    T       (*GLF)();           //get leaf
    bool    (*IND)(T);          //is a node?
    bool    (*IVR)(T);          //is a variable?
    int     (*GVI)(T);          //get variable index(the variables can be in disorder)
    int     (*GEI)(T);          //get expreison index(the expresions can be in disorder)
    char    (*GCHR)(T);
    GenAlg(){}
};

template<typename T,typename U>
GenAlg<T,U>* newGA(int fillS,int ms,int cs,int ss,int pobSiz,int indvSiz,float mutrt,float crossrt,float cross_prop,
 U (*eve)(int,U,U), U (*gtr)(T), T (*gop)(),T (*glf)(),bool (*ind)(T),bool (*ivr)(T),int (*gvi)(T),int (*gei)(T),char (*gchr)(T))
{

    GenAlg<T,U>* tmpGA = new GenAlg<T,U>();
    tmpGA->INDIVIDUALS=new Indiv<T>[pobSiz];
    tmpGA->NEWINDIVIDUALS=new Indiv<T>[pobSiz];
    tmpGA->fill_selection = fillS;
    tmpGA->m_selection = ms;
    tmpGA->c_selection = cs;
    tmpGA->s_selection = ss;
    tmpGA->poblation_size = pobSiz;
    tmpGA->indiv_init_size = indvSiz;
    tmpGA->mutation_rate = mutrt;
    tmpGA->crossover_rate = crossrt;
    tmpGA->cross_proportion= cross_prop;
    tmpGA->EVE = eve;
    tmpGA->GTR = gtr;
    tmpGA->GOP = gop;
    tmpGA->GLF = glf;
    tmpGA->IND = ind;
    tmpGA->IVR = ivr;
    tmpGA->GVI = gvi;
    tmpGA->GEI = gei;
    tmpGA->GCHR = gchr;
    return tmpGA;
}

template<typename T, typename U,std::size_t SIZE>
void initPobRec(GenAlg<T,U>* GA,U (*_vals)[SIZE],size_t size_vals,int counter)
{
    //printf("%d ",counter);
    if(counter==GA->poblation_size){return;}
    else{
        GA->INDIVIDUALS[counter]=*newIndiv(GA,_vals,size_vals);
        printf("index:\t%d\t",counter);PrintPosOrder(GA->INDIVIDUALS[counter].chrom,GA->IND,GA->GCHR);printf("fitness: \t%6.4f\n",GA->INDIVIDUALS[counter].fitness);
        initPobRec(GA,_vals,size_vals,counter+1);
    }
    return;
}

/***************************************************************************************************************
*   FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*   IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
***************************************************************************************************************/
template<typename T,typename U,std::size_t  SIZE>
Indiv<T>* newIndiv(GenAlg<T,U>* GA,U (*Vals)[SIZE],size_t sizeVal)
{
    Indiv<T>* temp_indiv = new Indiv<T>();
    temp_indiv->chrom = newNode(GA->fill_selection,GA->indiv_init_size,GA->GOP(),GA->GLF,GA->GOP);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,Vals,sizeVal);
    return temp_indiv;
}

template<typename T,typename U,std::size_t SIZE>
void evalFitness(GenAlg<T,U>* GA,Indiv<T>* t_indiv,float (*values)[SIZE],size_t size_vals){
    t_indiv->fitness = cuadraticError(t_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,values,size_vals);
}

/************************************************************************************
*   the function apply a kind of mutation to a individual with selection Option     *
************************************************************************************/
template<typename T,typename U>
void MutateIndiv(GenAlg<T,U>* GA,Indiv<T>* someIndiv)
{
    Indiv<T>* tempInd;
}

/****************************************************************************************
*   the funcitons interchange the subtree of both individuals with selection Option     *
****************************************************************************************/
template<typename T,typename U,std::size_t SIZE>
void CrossIndivs(GenAlg<T,U>* GA,Indiv<T>* parentA,Indiv<T>* parentB,float (*values)[SIZE],size_t size_vals)
{
    Node<T>* subIndivA;//saves the direction of the B subtree
    Node<T>* subIndivB;//saves the direction of the A subtree 
    int countOperandsA = nodeCounter(parentA,GA->IND);
    int countOperandsB = nodeCounter(parentB,GA->IND);
    int Index_A=0;
    int Index_B=0;
    switch (GA->c_selection){
        case HALF_CROSS:// 50% of individual information will be interchanged 
            Index_A = (int)countOperandsA/2; Index_B = (int)countOperandsB/2; break;
        case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
            Index_A = (int)countOperandsA*(GA->cross_proportion); Index_B = (int)countOperandsB*(1 - GA->cross_proportion); break;    
        case RAND_CROSS://aleatory index
            Index_A = rand()%countOperandsA; Index_B = rand()%countOperandsB; break;    
    }
    /*gets the indexed operand over the trees*/
    getIndexedSubTree(Index_A,parentA->chrom,subIndivA,GA->IND);
    getIndexedSubTree(Index_B,parentB->chrom,subIndivB,GA->IND);
    /* interchange the indexed operands*/
    setIndexedSubTree(Index_A,parentA->chrom,subIndivB,GA->IND);
    setIndexedSubTree(Index_B,parentB->chrom,subIndivA,GA->IND);
    evalFitness(GA,parentA,values,size_vals);
    evalFitness(GA,parentB,values,size_vals);
}


/*##############################################################################
###############  Funciones para tomar informacion estadistica ##################
##############################################################################*/
template<typename T,typename U>
void printPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->INDIVIDUALS[index].fitness);
        if(index,GA->INDIVIDUALS[index].fitness==0)PrintPosOrder(GA->INDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

template<typename T,typename U>
void printNewPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->NEWINDIVIDUALS[index].fitness);
        if(index,GA->NEWINDIVIDUALS[index].fitness==0)PrintPosOrder(GA->NEWINDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

/*############################################################################
##############################################################################
###############     Funciones para el algoritmo genético    ##################
##############################################################################
############################################################################*/



template<typename T,typename U>
void SELECTION(GenAlg<T,U>* GA)
{
    float pob_fitness[GA->poblation_size]; 
    float temp_random=0.0f;
    float total_fit = 0.0f;
    float max_fit = 0.0f;

    for(int index =0;index<GA->poblation_size;index++)
    { pob_fitness[index] = GA->INDIVIDUALS[index].fitness; }

    switch(GA->s_selection)
    {
        case ROULT:
            for(int index=0;index<GA->poblation_size;index++)
            { if(pob_fitness[index]>max_fit)max_fit=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]= max_fit + 1 - pob_fitness[index]; total_fit+=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]=pob_fitness[index]*GA->poblation_size/total_fit; if(index>0){ pob_fitness[index]=pob_fitness[index]+pob_fitness[index-1]; } }
            for(int ext_index=0;ext_index<GA->poblation_size;ext_index++)
            {
                temp_random = ((float)rand()/(float)(RAND_MAX+1))*GA->poblation_size;
                for(int index=0;index<GA->poblation_size;index++)
                { 
                    GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index];
                    if(pob_fitness[index]>temp_random){ break; }  
                }
                printf("index:\t%d\t",ext_index);/*PrintPosOrder(GA->NEWINDIVIDUALS[ext_index].chrom,GA->IND,GA->GCHR);*/printf("fitness: \t%6.4f\n",GA->NEWINDIVIDUALS[ext_index].fitness);
            }

        break;
        case TOURN:

        break;
    }
}

#endif
