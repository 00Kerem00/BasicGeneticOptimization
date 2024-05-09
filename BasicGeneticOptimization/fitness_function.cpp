#include "individual.cpp"

class FitnessFunction
{
public:
    virtual float getFitness(const Individual& individual) const { return -1; };
};