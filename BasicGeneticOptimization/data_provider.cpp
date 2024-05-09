#include <string>
#include "file_manager.cpp"
#include <functional>

class DataProvider
{
public:
    virtual DataSet provideData() const 
    {
         return DataSet(); 
    }
};

class InstantDataProvider : public DataProvider
{
    std::string fileName;
public:
    InstantDataProvider(std::string fileName) { this->fileName = fileName; }

    DataSet provideData() const override
    {
        return FileManager::getDataSet(fileName);
    }
};

class ComputationalDataProvider : public DataProvider
{
    function<DataSet()> dataSetGeneration;
public:
    ComputationalDataProvider(function<DataSet()> dataSetGeneration) { this->dataSetGeneration = dataSetGeneration; }

    DataSet provideData() const
    {
        return dataSetGeneration();
    }
};