#include <vector>

using namespace std;

class DataSet
{
public:
    vector<vector<long>> x;
    vector<vector<long>> y;

    DataSet() {}
    DataSet(const vector<vector<long>>& x, const vector<vector<long>>& y) // [dataPoint][column]
    {
        this->x = x;
        this->y = y;
    }

    vector<long> getXColumn(int index) const
    {
        int countOfDataPoint = x.size();
        vector<long> result(countOfDataPoint);
        for(int i = 0; i < countOfDataPoint; i++)
        {
            result[i] = x[i][index];
        }

        return result;
    }

    vector<long> getYColumn(int index) const
    {
        int countOfDataPoint = y.size();
        vector<long> result(countOfDataPoint);
        for(int i = 0; i < countOfDataPoint; i++)
        {
            result[i] = y[i][index];
        }

        return result;
    }
};