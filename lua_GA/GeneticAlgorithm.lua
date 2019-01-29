GenAl={}

GenAl.newGenAl=funciton(chromosomeSize, populationSize, populationInit,fitness, selection, crossover, mutation, crossChance, mutationChance)
    self.chromosomeSize = chromosomeSize
    self.populationSize = populationSize
    self.NewPop         = populationInit
    self.fitness        = fitness
    self.selection      = selection
    self.crossover      = crossover
    self.mutation       = mutation
    self.crossChance    = crossChance
    self.mutationChance = mutationChance
end

GenAl.Evolve=funciton(maxIterations)
    counter=0
    self.population = self.NewPop(self.chromosomeSize,self.populationSize,self.fitness)
    while(counter<maxIterations)
        self.newpopulation  =self.selection(self.population)
        self.newpopulation  =self.crossover(self.newpopulation,self.crossChance)
        self.population     =self.mutation(self.newpopulation,self.mutationChance)
        counter=counter+1
    end
    return self.population
end

return GenAl