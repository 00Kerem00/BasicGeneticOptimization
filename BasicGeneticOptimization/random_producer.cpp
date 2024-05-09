#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

class RandomProducer
{
public:

    static RandomProducer getInstance()
    {
        if(!instanceGenerated)
        {
            instance = RandomProducer();
            instanceGenerated = true;
        }

        return instance;
    }

    float produceRandom(int min, int max)
    {
        float result = min + ((float)rand() / 32767.0) * (max - min);

        return result;
    }

    vector<float> produceRandomVector(int vectorLength, int min, int max)
    {
        vector<float> result(vectorLength);
        for(int i = 0; i < vectorLength; i++)
        {
            result[i] = produceRandom(min, max);
        }
        return result;
    }

    vector<float> produceSortedRandomVector(int vectorLength, int min, int max)
    {
        vector<float> result = produceRandomVector(vectorLength, min, max);
        sort(result.begin(), result.end());

        return result;
    }
private:
    static RandomProducer instance;
    static bool instanceGenerated;
    RandomProducer() { srand(time(0)); }
};

bool RandomProducer::instanceGenerated = false;