#include <vector>
#include <functional>
#include "probability_selection.cpp"
#include "gop_demonstrator.cpp"

class Individual
{
private:
    function<long(const vector<float>&, const vector<long>&)> prediction;     // (variablesOfChromosome, independentVariables)


public:
    vector<float> variables;

    Individual() {}
    Individual(int variableCount, function<long(const vector<float>&, const vector<long>&)> prediction)
    {
        this->prediction = prediction;
        variables = RandomProducer::getInstance().produceRandomVector(variableCount, 0, 100);
    } 
    Individual(vector<float> variables, function<long(const vector<float>&, const vector<long>&)> prediction)
    {
        this->prediction = prediction;
        this->variables = variables;
    }

    long predict(vector<long> independentVariables)const
    {
        long result = prediction(variables, independentVariables);
        return result;
    }

    static Individual generateNewIndividual(Individual parent0, Individual parent1, ProbabilityPool mutationRatio, ProbabilityPool crossoverRatio)
    {
        int variableCount = parent0.variables.size();

        // determinate the crossover count and positions
        int crossoverCount = ProbabilitySelection(crossoverRatio).getRandomProbability();
        vector<float> crossoverPositions = RandomProducer::getInstance().produceSortedRandomVector(crossoverCount, 1, variableCount);

        // generate new individual chromosome and fill with genes of parents according to crossover positions
        vector<float> variablesOfNewIndividual(variableCount);
        Individual* parents[2] = { &parent0, &parent1 };
        bool indexOfInheratorParent = false;
        int indexOfCoPosition = 0;
        for(int i = 0; i < variableCount; i++)
        {
            bool crossOver = false;
            while(indexOfCoPosition < crossoverCount && (int)crossoverPositions[indexOfCoPosition] == i)
            {
                indexOfCoPosition++;
                crossOver = true;
            }
            if(crossOver) { indexOfInheratorParent = !indexOfInheratorParent; }

            variablesOfNewIndividual[i] = parents[indexOfInheratorParent]->variables[i];
        }

        // generate mutations
        ProbabilitySelection mutationEventDeterminator = ProbabilitySelection(mutationRatio);
        vector<int> mutationEvents = ProbabilitySelection(mutationRatio).getRandomProbabilityVector(variableCount);
        for(int i = 0; i < variableCount; i++)
        {
            if(mutationEvents[i] == true)
            {
                variablesOfNewIndividual[i] = RandomProducer::getInstance().produceRandom(0, 100);
            }
        }

        // supply the genetic optimization process demonstrator
        GOP_Demonstrator::getInstance().supplyByIndividual(mutationEvents, crossoverPositions);

        return Individual(variablesOfNewIndividual, parent0.prediction);
    }
};