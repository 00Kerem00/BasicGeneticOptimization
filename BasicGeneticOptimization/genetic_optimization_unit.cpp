#include "population.cpp"

class GeneticOptimizationUnit
{
    int variableCountOfEachIndividual;
	int individualCountOfEachPopulation;
    int survivorCountOfEachPopulation;
    const function<long(const vector<float>&, const vector<long>&)>& predictionOfIndividuals;
	const FitnessFunction& fitnessFunction;
	const ProbabilityPool& mutationRatio;
	const ProbabilityPool& crossoverRatio;
	int iterationCount;

public:
    GeneticOptimizationUnit(
        int variableCountOfEachIndividual, 
	    int individualCountOfEachPopulation,
        int survivorCountOfEachPopulation,
        const function<long(const vector<float>&, const vector<long>&)>& predictionOfIndividuals,
	    const FitnessFunction& fitnessFunction,
    	const ProbabilityPool& mutationRatio,
	    const ProbabilityPool& crossoverRatio,
    	int iterationCount
    ) : variableCountOfEachIndividual(variableCountOfEachIndividual),
        individualCountOfEachPopulation(individualCountOfEachPopulation),
        survivorCountOfEachPopulation(survivorCountOfEachPopulation),
        predictionOfIndividuals(predictionOfIndividuals),
        fitnessFunction(fitnessFunction),
        mutationRatio(mutationRatio),
        crossoverRatio(crossoverRatio),
        iterationCount(iterationCount) {}

    Individual startOptimization()
    {
        GOP_Demonstrator::getInstance().startSupply(iterationCount);
        Population currentPopulation = Population(individualCountOfEachPopulation, variableCountOfEachIndividual, predictionOfIndividuals);

        for(int i = 0; i < iterationCount; i++)
        {
            GOP_Demonstrator::getInstance().supplyByOptimizationUnit(
                currentPopulation.convertVariablesOfIndividualsToString(), survivorCountOfEachPopulation);            
            
            currentPopulation = currentPopulation.generateNextPopulation(
                survivorCountOfEachPopulation,
                fitnessFunction,
                mutationRatio,
                crossoverRatio
                );
        }
        
        GOP_Demonstrator::getInstance().endSupply(currentPopulation.convertVariablesOfIndividualsToString());

        vector<float> fitnessesOfLastIndividuals = currentPopulation.getFitnessOfIndividuals(fitnessFunction);
        int indexOfBestIndividual = 0;
        for(int i = 0; i < individualCountOfEachPopulation; i++)
        {
            if(fitnessesOfLastIndividuals[indexOfBestIndividual] < fitnessesOfLastIndividuals[i])
            {
                indexOfBestIndividual = i;
            }
        }

        return currentPopulation.individuals[indexOfBestIndividual];
    }
};