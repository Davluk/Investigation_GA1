#ifndef GenAl
#define GenAl

/*
    genetic algorithm
        void newGA function (type of chromosomes,initPopFunction,crossoverFunction,SelectionFunciton,MutationFunction)
        newpopulation* selection(popualtion*)
        newpopulation* crossover(population*)
        newpopulaiton* mutation (population*)
*/

typedef struct GeneticAlgorithm
{
    void* Population;
    void* newPopulaiton;
    void* (*initPop)(void*);
    void* (*GASelection)(void*);
    void* (*GACrossover)(void*);
    void* (*GAMutation) (void*);
};

#endif 