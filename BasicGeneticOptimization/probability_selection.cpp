#include <vector>
#include "random_producer.cpp"

class ProbabilityPool
{
public:
    vector<int> probabilities;
    ProbabilityPool(vector<int> probabilities)
    {
        this->probabilities = probabilities;
    }
};

class ProbabilitySelection
{
private:
    int totalProbability;
    ProbabilityPool probabilityPool;

public:
    ProbabilitySelection(ProbabilityPool probabilityPool) : totalProbability(0), probabilityPool({})
    {
        this->probabilityPool = probabilityPool;
        totalProbability = 0;
        int countOfProbs = probabilityPool.probabilities.size();
        for(int i = 0; i < countOfProbs; i++)
        {
            totalProbability += probabilityPool.probabilities[i];
        }
    }

    int getRandomProbability()
    {
        int randomEvent = RandomProducer::getInstance().produceRandom(0, totalProbability);
        
        int sum = 0;
        int countOfProbs = probabilityPool.probabilities.size();
        for(int i = 0; i < countOfProbs; i++)
        {
            sum += probabilityPool.probabilities[i];
            if(sum > randomEvent) { return i; }
        }

        return -1;
    }
    
    vector<int> getRandomProbabilityVector(int vectorSize)
    {
        vector<int> result(vectorSize);
        for(int i = 0; i < vectorSize; i++)
        {
            result[i] = getRandomProbability();
        }

        return result;
    }
};