#include "Node.cpp"
#include <math.h>
#include <vector>
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
#define BIT_MUT 0
#define OPER_MUT 1
#define SUBT_MUT 2
#define NO_MUT 3

/*
*
*/
#define TOURN 0
#define ROULT 1


   

/*********************************************
*   structure for tratment with individuals  *
*********************************************/
template<typename T,typename U>
struct Indiv
{
    Node<T>* chrom;
    U fitness; 
    Indiv(){}   
};

/********************************************
*   structure for the genetic algorithm     *
********************************************/  
template<typename T,typename U>
struct GenAlg{
    Indiv<T,U>* INDIVIDUALS;
    Indiv<T,U>* NEWINDIVIDUALS;
    int   max_generations;
    int   poblation_size;
    int   indiv_init_size;
    int   selection_opt;
    int   fill_selection;
    int   m_selection;
    int   c_selection;
    int   s_selection;
    U mutation_rate;
    U crossover_rate;
    U cross_proportion;
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
GenAlg<T,U>* newGA(int fillS,int ms,int cs,int ss,int pobSiz,int indvSiz,int max_g,float mutrt,float crossrt,float cross_prop,
 U (*eve)(int,U,U), U (*gtr)(T), T (*gop)(),T (*glf)(),bool (*ind)(T),bool (*ivr)(T),int (*gvi)(T),int (*gei)(T),char (*gchr)(T))
{   
    if(ss==TOURN && pobSiz%2!=0){printf("SI SELECCIONAS >>SELECCION POR TORNEO<< DEBES TENER UN NUMERO PAR DE INDIVIDUOS\n");exit(EXIT_FAILURE);}
    GenAlg<T,U>* tmpGA = new GenAlg<T,U>();
    tmpGA->INDIVIDUALS=new Indiv<T,U>[pobSiz];
    tmpGA->NEWINDIVIDUALS=new Indiv<T,U>[pobSiz];
    tmpGA->max_generations = max_g;
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
    if(counter==GA->poblation_size){return;}
    else{
        //printf("{%d,",counter);
        GA->INDIVIDUALS[counter]=*newIndiv(GA,_vals,size_vals);
        //printf("%0.4f}\n",GA->INDIVIDUALS[counter].fitness);
        //if(GA->INDIVIDUALS[counter].fitness==0){ PrintPosOrder(GA->INDIVIDUALS[counter].chrom,GA->IND,GA->GCHR); }
        initPobRec(GA,_vals,size_vals,counter+1);
    }
    return;
}

/***************************************************************************************************************
*   FS-> fill_selection; D -> depth; GO-> getOperand; GT -> getTerminal; EE-> evalExpresion; IN-> isNode       *
*   IV-> isVar; GVI-> getVarIndex; GEI-> getExpresionIndex; val-> values to model                              *
***************************************************************************************************************/
template<typename T,typename U,std::size_t  SIZE>
Indiv<T,U>* newIndiv(GenAlg<T,U>* GA,U (*Vals)[SIZE],size_t sizeVal)
{
    Indiv<T,U>* temp_indiv = new Indiv<T,U>();
    temp_indiv->chrom = newNode(GA->fill_selection,GA->indiv_init_size,GA->GOP(),GA->GLF,GA->GOP);
    temp_indiv->fitness = cuadraticError(temp_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,Vals,sizeVal);
    return temp_indiv;
}

template<typename T,typename U,std::size_t SIZE>
void evalFitness(GenAlg<T,U>* GA,Indiv<T,U>* t_indiv,U (*values)[SIZE],size_t size_vals){
    t_indiv->fitness = cuadraticError(t_indiv->chrom,GA->EVE,GA->IND,GA->IVR,GA->GVI,GA->GTR,GA->GEI,values,size_vals);
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
        if(index,GA->INDIVIDUALS[index].fitness<(U)10*GA->poblation_size)PrintPosOrder(GA->INDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

template<typename T,typename U>
void printNewPobStatus(GenAlg<T,U>* GA)
{
    for(int index = 0;index<GA->poblation_size;index++)
    { 
        printf("{\t%d\t,\t%6.4f\t}",index,GA->NEWINDIVIDUALS[index].fitness);
        if(index,GA->NEWINDIVIDUALS[index].fitness<(U)10*GA->poblation_size)PrintPosOrder(GA->NEWINDIVIDUALS[index].chrom,GA->IND,GA->GCHR); 
        printf("\n");
    }
}

/*############################################################################
##############################################################################
###############     Funciones para el algoritmo genético    ##################
##############################################################################
############################################################################*/

int tournament(int ind1, float fit1, int ind2, float fit2)
{ return (fit1<fit2)?(ind1):(ind2); }

template<typename T,typename U>
void SELECTION(GenAlg<T,U>* GA)
{
    float pob_fitness[GA->poblation_size]; 
    float total_fit = 0.0f;
    float max_fit = 0.0f;

    for(int index =0;index<GA->poblation_size;index++)
    { pob_fitness[index] = GA->INDIVIDUALS[index].fitness; }

    switch(GA->s_selection)
    {
        case ROULT://problemas
            for(int index=0;index<GA->poblation_size;index++)
            { if(pob_fitness[index]>max_fit)max_fit=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]= max_fit + 1 - pob_fitness[index]; total_fit+=pob_fitness[index]; }
            for(int index=0;index<GA->poblation_size;index++)
            { pob_fitness[index]=pob_fitness[index]*GA->poblation_size/total_fit; if(index>0){ pob_fitness[index]=pob_fitness[index]+pob_fitness[index-1]; } }
            for(int ext_index=0;ext_index<GA->poblation_size;ext_index++)
            {
                float temp_random = ((float)(rand()%1000000)/(float)1000000)*GA->poblation_size;
                for(int index=0;index<GA->poblation_size;index++)
                { 
                    GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index];
                    if(pob_fitness[index]>temp_random && index>0){ GA->NEWINDIVIDUALS[ext_index]=GA->INDIVIDUALS[index-1]; break; }  
                }
            }
        break;
        case TOURN:
            int   ind_1 = 0;
            int   ind_2 = 0; 
            int counter = 0;
            std::vector<int> indexes; 

            for(int index=0;index<GA->poblation_size;index++)indexes.push_back(index);
            while(indexes.size()>0){
                ind_1= rand()%indexes.size(); int temp_ind1=indexes[ind_1]; indexes.erase(indexes.begin()+ind_1);
                ind_2= rand()%indexes.size(); int temp_ind2=indexes[ind_2]; indexes.erase(indexes.begin()+ind_2);
                GA->NEWINDIVIDUALS[counter++]=GA->INDIVIDUALS[tournament(temp_ind1,GA->INDIVIDUALS[temp_ind1].fitness,temp_ind2,GA->INDIVIDUALS[temp_ind2].fitness)];

            }
            for(int index=0;index<GA->poblation_size;index++)indexes.push_back(index);
            while(indexes.size()>0){
                ind_1= rand()%indexes.size(); int temp_ind1=indexes[ind_1]; indexes.erase(indexes.begin()+ind_1);
                ind_2= rand()%indexes.size(); int temp_ind2=indexes[ind_2]; indexes.erase(indexes.begin()+ind_2);
                GA->NEWINDIVIDUALS[counter++]=GA->INDIVIDUALS[tournament(temp_ind1,GA->INDIVIDUALS[temp_ind1].fitness,temp_ind2,GA->INDIVIDUALS[temp_ind2].fitness)];

            }
        break;
    }
    GA->INDIVIDUALS=GA->NEWINDIVIDUALS;
    GA->NEWINDIVIDUALS = new Indiv<T,U>[GA->poblation_size];
}

template<typename T, typename U,std::size_t SIZE>
void CROSSOVER(GenAlg<T,U>* GA,U (*_vals)[SIZE],size_t val_size)
{
    std::vector<int> parent_indexes;
    int indexA=0;
    int indexB=0;
    int countOperandsA = 0;//nodeCounter(parentA->chrom,GA->IND);
    int countOperandsB = 0;//nodeCounter(parentB->chrom,GA->IND);
    for(int index = 0;index<GA->poblation_size;index++)
    {
        float temp_random = (float)(rand()%1000000)/((float)1000000);
        if(temp_random<=GA->crossover_rate){ parent_indexes.push_back(index); }
    }
    if(parent_indexes.size()%2!=0){ parent_indexes.pop_back(); }
    for(int index = 0;index<parent_indexes.size();index+=2)
    {
        Node<T>* subtreeA=new Node<T>;
        Node<T>* subtreeB=new Node<T>;
        int parent_A = parent_indexes[index];
        int parent_B = parent_indexes[index+1];
        if(GA->INDIVIDUALS[parent_A].fitness>(U)1000*GA->poblation_size && GA->INDIVIDUALS[parent_B].fitness>(U)1000*GA->poblation_size) 
        {
            float parent_A_fit = GA->INDIVIDUALS[parent_A].fitness;
            float parent_B_fit = GA->INDIVIDUALS[parent_B].fitness;
            int countOperandsA = nodeCounter(GA->INDIVIDUALS[parent_A].chrom,GA->IND);
            int countOperandsB = nodeCounter(GA->INDIVIDUALS[parent_B].chrom,GA->IND);

            switch (GA->c_selection){
            case HALF_CROSS:// 50% of individual information will be interchanged 
                indexA = (int)floor(countOperandsA/2); indexB = (int)floor(countOperandsB/2); break;
            case PROP_CROSS://porcentage proportional cross, the value is from 0 to 1
                indexA = (int)floor(countOperandsA*(GA->cross_proportion)); indexB = (int)floor(countOperandsB*(GA->cross_proportion)); break;    
            case RAND_CROSS://aleatory index
                float rand_n = (float)(rand()%1000000)/(float)1000000;
                indexA = (int)floor(countOperandsA*rand_n); indexB = (int)floor(countOperandsB*rand_n); break;
            }

            /*gets the indexed operand over the trees*/
            getIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeA,GA->IND);
            getIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeB,GA->IND);
            /* interchange the indexed operands*/
            setIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeB,GA->IND);
            setIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeA,GA->IND);
            evalFitness(GA,&GA->INDIVIDUALS[parent_A],_vals,val_size);
            evalFitness(GA,&GA->INDIVIDUALS[parent_B],_vals,val_size);
            if(GA->INDIVIDUALS[parent_A].fitness>parent_A_fit && GA->INDIVIDUALS[parent_B].fitness>parent_B_fit)
            {   /*UNDID THE ACTION*/
                setIndexedSubTree(indexA,GA->INDIVIDUALS[parent_A].chrom,subtreeA,GA->IND);
                setIndexedSubTree(indexB,GA->INDIVIDUALS[parent_B].chrom,subtreeB,GA->IND);
            }
        }
        delete subtreeA;
        delete subtreeB;
    }
}

template<typename T,typename U>
void MUTATION(GenAlg<T,U>* GA)
{
    int total_nodes = 0;//totalNodeCounter(indiv_mut->chrom);
    int subt_total_nodes=0;
    int nodes = 0;// nodeCounter(indiv_mut->chrom,GA->IND);
    int temp_rand=0;
    Node<T>* temp_node = new Node<T>();
    for(int index=0;index<GA->poblation_size;index++)
    {
        float temp_random = (float)(rand()%1000000)/((float)1000000);
        if(temp_random<=GA->mutation_rate&&GA->INDIVIDUALS[index].fitness>(U)10*GA->poblation_size){ 
            //MutateIndiv(GA,&GA->INDIVIDUALS[index]); 
            total_nodes = totalNodeCounter(GA->INDIVIDUALS[index].chrom);
            nodes = nodeCounter(GA->INDIVIDUALS[index].chrom,GA->IND);
            switch(GA->m_selection)
            {
                case BIT_MUT:
                    temp_rand= rand()%total_nodes;
                    getIndexedNode(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node);
                    if(GA->IND(temp_node->data))
                    { temp_node->data = GA->GOP(); }
                    else
                    { temp_node->data = GA->GLF(); }
                    setIndexedNode(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node);
                    
                break;
                case OPER_MUT:
                    temp_rand = rand()%nodes;
                    getIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND);
                    temp_node->data = GA->GOP();
                    setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND);
                    
                break;
                case SUBT_MUT:
                    temp_rand = rand()%nodes;
                    getIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND);
                    temp_node = newNode(GA->fill_selection,(int)log2f(totalNodeCounter(temp_node)),GA->GOP(),GA->GLF,GA->GOP);
                    setIndexedSubTree(temp_rand,GA->INDIVIDUALS[index].chrom,temp_node,GA->IND);
                    
                break;
                case NO_MUT: return ;
                break;
            } 
        }
    }

    
}
template<typename T,typename U,std::size_t SIZE>
void evalPoblation(GenAlg<T,U>* GA,U (*vals)[SIZE],size_t val_size)
{
    for(int index = 0;index<GA->poblation_size;index++)
    {
        evalFitness(GA,&GA->INDIVIDUALS[index],vals,val_size);
    }
}

template<typename T,typename U,std::size_t SIZE>
void KILLWORSTINDIVS(GenAlg<T,U>* GA,U (*vals)[SIZE],size_t val_size)
{
    for(int index=0;index<GA->poblation_size;index++)
    {
        if(GA->INDIVIDUALS[index].fitness>(U)1000000*GA->poblation_size)
        {
            GA->INDIVIDUALS[index]=*newIndiv(GA,vals,val_size);
        }
    }
}

template<typename T,typename U, std::size_t SIZE>
void GENETICPROSSES(GenAlg<T,U>* GA,U (*_vals)[SIZE],size_t _val_size)
{   
    float advantage=0.0f;
    int adv_counter = 0;
    for(int index = 0;index<GA->max_generations;index++)
    {
        advantage = 100*(1.0f-((float)GA->max_generations-(float)index)/(float)GA->max_generations);
        if((advantage-(float)adv_counter)>1.0f){printf("#");adv_counter++;if(adv_counter%5==0){printf(".");}}
        KILLWORSTINDIVS(GA,_vals,_val_size);
        SELECTION(GA);
        CROSSOVER(GA,_vals,_val_size);
        MUTATION(GA);
        evalPoblation(GA,_vals,_val_size);

    }
    printf("COMPlETE!!\n");
}



template<typename T,typename U>
void printBestIndiv(GenAlg<T,U>* GA)
{
   Indiv<T,U>* best_indiv=new Indiv<T,U>(); 
   best_indiv->fitness=1/0.000000000000000000000000000000000000000000001f;
   for(int index=0;index<GA->poblation_size;index++)
   {
        if(best_indiv->fitness>GA->INDIVIDUALS[index].fitness){ *best_indiv = GA->INDIVIDUALS[index]; } 
   }
   printf("fitness: %f\n",best_indiv->fitness);PrintPosOrder(best_indiv->chrom,GA->IND,GA->GCHR);printf("\n");
   delete best_indiv;
}



#endif
