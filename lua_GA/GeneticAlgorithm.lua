GenAl={}

GenAl.newGenAl=funciton(chromosomeSize, populationSize, populationInit, selection, crossover, mutation, crossChance, mutationChance)
    self.population = populationInit(chromosomeSize,populationSize)
    self.newpopulation = {}
    self.getNewPop = populationInit
    self.selection = selection
    self.mutation = mutation
    self.crossover = crossover
    self.crossChance = crossChance
    self.mutationChance = mutationChance
end

GenAl.Evolve=funciton(maxIterations)
    counter=0
    while(counter<maxIterations)
        self.newpopulation=self.selection()
        self.newpopulation=self.crossover(self.crossChance)
        self.population=self.mutation(self.mutationChance)
        counter=counter+1
    end
end

return GenAl