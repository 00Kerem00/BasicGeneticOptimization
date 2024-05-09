#include <fstream>
#include <string>
#include "dataset.cpp"

using namespace std;

class FileManager
{
public:
    static DataSet getDataSet(string fileName)
    {
        ifstream file(fileName);
        string line;

        getline(file, line);
        int countOfXColumn = stoi(line);
        getline(file, line);
        int countOfYColumn = stoi(line);
        getline(file, line);
        int countOfDataPoint = stoi(line);
        vector<vector<long>> x(countOfDataPoint);
        vector<vector<long>> y(countOfDataPoint);

        for(int i = 0; i < countOfDataPoint; i++)
        {
            x[i].resize(countOfXColumn);
            y[i].resize(countOfYColumn);

            for(int j = 0; j < countOfXColumn; j++)
            {
                getline(file, line);
                x[i][j] = stof(line);
            }
            for(int j = 0; j < countOfYColumn; j++)
            {
                getline(file, line);
                y[i][j] = stof(line);
            }
        }

        return DataSet(x, y);
    }
};