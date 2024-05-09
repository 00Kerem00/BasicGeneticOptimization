#include "fitness_function.cpp"
#include <string>

class Population
{
public:
    vector<Individual> individuals;

    Population(vector<Individual> individuals) { this->individuals = individuals; }
    Population(int individualCount, int variableCountOfIndividuals, const function<float(const vector<float>&, const vector<long>&)>& predictionFunction)
    {
        individuals.resize(individualCount);
        for(int i = 0; i < individualCount; i++)
        {
            individuals[i] = Individual(variableCountOfIndividuals, predictionFunction);
        }
    }

    Population generateNextPopulation(
        int survivorCountAmongAncestors, const FitnessFunction& fitnessFunction, const ProbabilityPool& mutationRatio, const ProbabilityPool& crossoverRatio)
    {
        int individualCount = individuals.size();

        // sort individuals by high fitness
        vector<int> indexesOfSortedIndividuals = sortIndividualsByHighFitness(fitnessFunction);

        // select the parents from survived individuals
        vector<float> indexesOfParents0 = RandomProducer::getInstance().produceRandomVector(individualCount, 0, survivorCountAmongAncestors);
        vector<float> indexesOfParents1 = RandomProducer::getInstance().produceRandomVector(individualCount, 0, survivorCountAmongAncestors);



        // for prevention of same parent problem shift the index of parent1 one step to right if parent0 and parent1 are same
        for(int i = 0; i < individualCount; i++)
        {
            if((int)indexesOfParents0[i] == (int)indexesOfParents1[i])
            {
                indexesOfParents1[i] += 1;
                if(indexesOfParents1[i] >= survivorCountAmongAncestors) { indexesOfParents1[i] = 0; }
            }
        }

        // supply the genetic optimization process demonstrator
        GOP_Demonstrator::getInstance().supplyByPopulation(indexesOfParents0, indexesOfParents1, indexesOfSortedIndividuals);

        // generate the children
        vector<Individual> children(individualCount);
        for(int i = 0; i < individualCount; i++)
        {
            children[i] = Individual::generateNewIndividual(individuals[indexesOfSortedIndividuals[indexesOfParents0[i]]], individuals[indexesOfSortedIndividuals[indexesOfParents1[i]]], mutationRatio, crossoverRatio);
        }

        return Population(children);
    }
    
    vector<float> getFitnessOfIndividuals(const FitnessFunction& fitnessFunction)
    {
        int individualCount = individuals.size();
        vector<float> result(individualCount);

        for(int i = 0; i < individualCount; i++)
        {
            result[i] = fitnessFunction.getFitness(individuals[i]);
        }

        return result;
    }
    // returns a vector of index of sorted by high fitness individuals that population has
    vector<int> sortIndividualsByHighFitness(const FitnessFunction& fitnessFunction)
    {
        int individualCount = individuals.size();
        vector<float> fitnessesOfIndividuals = getFitnessOfIndividuals(fitnessFunction);
        vector<int> indexes(individualCount);
        for(int i = 0; i < individualCount; i++) {indexes[i] = i;}

        for(int i = 0; i < individualCount; i++)
        {
            int indexOfMax = i;
            for(int j = i; j < individualCount; j++)
            {
                if(fitnessesOfIndividuals[indexOfMax] < fitnessesOfIndividuals[j])
                {
                    indexOfMax = j;
                }
            }

            float temp1 = fitnessesOfIndividuals[indexOfMax];
            int temp2 = indexes[indexOfMax];

            fitnessesOfIndividuals[indexOfMax] = fitnessesOfIndividuals[i];
            fitnessesOfIndividuals[i] = temp1;
            indexes[indexOfMax] = indexes[i];
            indexes[i] = temp2;
        }

        return indexes;
    }

    vector<vector<string>> convertVariablesOfIndividualsToString()
    {
        vector<vector<string>> result;

        int individualCount = individuals.size();
        int variableCount = individuals[0].variables.size();
        int maxLength = 0;
        result.resize(individualCount);
        for(int i = 0; i < individualCount; i++)
        {
            result[i].resize(variableCount);

            for(int j = 0; j < variableCount; j++)
            {
                string textVar = to_string(individuals[i].variables[j]);
                result[i][j] = textVar;
            }
        }

        return result;
    }
};