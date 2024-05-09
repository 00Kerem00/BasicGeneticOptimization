class ClassA
{
private:
    static ClassA* instance;
    static bool instanceGenerated;
    ClassA() {}

public:
    int a = 0;
    static ClassA& getInstance()
    {
        if (!instanceGenerated)
        {
            instance = new ClassA();
            instanceGenerated = true;
        }
        return *instance;
    }
};

ClassA* ClassA::instance = nullptr;
bool ClassA::instanceGenerated = false;