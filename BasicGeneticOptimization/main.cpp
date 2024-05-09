#include <iostream>
#include <vector>
#include "genetic_optimization_unit.cpp"
#include "data_provider.cpp"
#include "testte.cpp"

class MyFitnessFunction : public FitnessFunction
{
public:
    DataSet inOutsOfPolynomial;
    
    MyFitnessFunction(const DataProvider& dataProvider)
    {
        inOutsOfPolynomial = dataProvider.provideData();
    }

    float getFitness(const Individual& individual) const
    {
        int countOfDataPoints = inOutsOfPolynomial.x.size();
        vector<long> predictions(countOfDataPoints);
        for(int i = 0; i < countOfDataPoints; i++)
        {
            predictions[i] = individual.predict(inOutsOfPolynomial.x[i]);
        }

        return 1 / (1 + meanSquaredError(inOutsOfPolynomial.getYColumn(0), predictions));
    }

    float meanSquaredError(const vector<long>& actualValues, const vector<long>& predictions)const
    {
        float countOfDataPoint = actualValues.size();
        float sum = 0;
        for(int i = 0; i < countOfDataPoint; i++)
        {
            float difference = predictions[i] - actualValues[i];
            sum += difference * difference / countOfDataPoint;
        }

        return sum;
    }
};

int main()
{
    int mutationProbability;
    int nonMutationProbability;
    int oneCrossoverProbability;
    int twoCrossoverProbability;
    int threeCrossoverProbability;
    int individualCountOfPopulations;
    int survivorCountOfPopulations;
    int iterationCount;

    cout << "Her populasyondaki birey sayisini girin: ";
    cin >> individualCountOfPopulations;
    cout << "Her populasyondaki hayatta kalan sayisini girin: ";
    cin >> survivorCountOfPopulations;
    cout << "Iterasyon sayisini girin: ";
    cin >> iterationCount;
    cout << "Her bir genin mutasyona ugrama ihtimali katsayisini girin: ";
    cin >> mutationProbability;
    cout << "Her bir genin mutasyona ugramama ihtimali katsayisini girin: ";
    cin >> nonMutationProbability;
    cout << "Her birey olusumundaki 1 adet crossover olma ihtimali katsayisini girin: ";
    cin >> oneCrossoverProbability;
    cout << "Her birey olusumundaki 2 adet crossover olma ihtimali katsayisini girin: ";
    cin >> twoCrossoverProbability;
    cout << "Her birey olusumundaki 3 adet crossover olma ihtimali katsayisini girin: ";
    cin >> threeCrossoverProbability;
    cout << endl << endl;

    auto prediction = [](const vector<float>& variables, const vector<long>& independentVariables) 
    {
        float x = independentVariables[0];
        float a = variables[0];
        float b = variables[1];
        float c = variables[2];
        float d = variables[3];

        long result = x * x * x * a + x * x * b + x * c + d;
        return result;
    };
    auto productionOfTestingData = []()
    {
        int countOfDataPoints = 15;

        vector<vector<long>> xVec(countOfDataPoints);
        vector<vector<long>> yVec(countOfDataPoints);

        for(int i = 0; i < countOfDataPoints; i++)
        {
            xVec[i].resize(1);
            yVec[i].resize(1);

            xVec[i][0] = RandomProducer::getInstance().produceRandom(0, 100);

            long x = xVec[i][0];
            yVec[i][0] = 2 * x * x * x + 12 * x * x + 13 * x + 100;
        }

        return DataSet(xVec, yVec);
    };

    MyFitnessFunction fitnessFunction = MyFitnessFunction(ComputationalDataProvider(productionOfTestingData));
    ProbabilityPool mutationRatio = ProbabilityPool({nonMutationProbability, mutationProbability});
    ProbabilityPool crossoverRatio = ProbabilityPool({0, oneCrossoverProbability, twoCrossoverProbability, threeCrossoverProbability, 4});
    GeneticOptimizationUnit optimization = GeneticOptimizationUnit(
        4, 
        individualCountOfPopulations, 
        survivorCountOfPopulations, 
        prediction, 
        fitnessFunction, 
        mutationRatio, 
        crossoverRatio, 
        iterationCount
    );

    Individual bestIndividual = optimization.startOptimization();

    GOP_Demonstrator::getInstance().renderTheProcess();
    cout << endl << endl;
    
    cout << "EN IYI BIREYIN GENLERI:" << endl;
    int variableCount = bestIndividual.variables.size();
    for(int i = 0; i < variableCount; i++)
    {
        cout << bestIndividual.variables[i] << endl;
    }
    cin.get();
    cin.get();
    cin.get();
}


/*    ProbabilityPool mutationRatio = ProbabilityPool({4, 1});
    ProbabilityPool crossoverRatio = ProbabilityPool({0, 40, 15, 7, 4});
    GeneticOptimizationUnit optimization = GeneticOptimizationUnit(4, 10, 4, prediction, fitnessFunction, mutationRatio, crossoverRatio, 15);
*/