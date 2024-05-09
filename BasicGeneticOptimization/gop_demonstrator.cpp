#include <iostream>
#include <string>
#include <vector>
#define byte myapp_byte
#include <windows.h>
#undef byte

using namespace std;

class GOPD_Iteration
{

    int getIndexOfIOITSBF(int value)
    {
        for(int i = 0; i < survivorCountOfAncestors; i++)
        {
            if(value == indexesOfAncestorsThatSortedByFitness[i]) { return getColor(i); }
        }

        return -1;
    }

    void generateTheColorsOfMatrix()
    {
        int childrenCount = childrenAsText.size();
        int variableCount = childrenAsText[0].size();

        vector<float>* indexesOfParents[2] = { &indexesOfParents0, &indexesOfParents1 };
        colorsOfMatrix.resize(childrenCount);
        for(int i = 0; i < childrenCount; i++)
        {
            colorsOfMatrix[i].resize(variableCount);

            bool indexOfInheratorParent = false;
            int indexOfCoPosition = 0;
            int crossoverCount = crossoverPositions[i].size();

            for(int j = 0; j < variableCount; j++)
            {
                bool crossOver = false;
                while(indexOfCoPosition < crossoverCount && (int)crossoverPositions[i][indexOfCoPosition] == j)
                {
                    indexOfCoPosition++;
                    crossOver = true;
                }
                if(crossOver) { indexOfInheratorParent = !indexOfInheratorParent; }

                if(mutationEvents[i][j] != 1)
                {
                    colorsOfMatrix[i][j] = (*indexesOfParents[indexOfInheratorParent])[i] + 1;
                }
                else
                {
                    colorsOfMatrix[i][j] = 11;
                }
            }
        }
    }
    void renderAncestors(HANDLE& h)
    {
        int individualCount = ancestorsAsText.size();
        int variableCount = ancestorsAsText[0].size();

        for(int i = 0; i < individualCount; i++) { renderCover(maxLengthOfTextOfAncestorVariables); cout << "      "; }
        cout << endl;
        for(int i = 0; i < variableCount; i++)
        {
            for(int j = 0; j < individualCount; j++)
            {
                renderCell(ancestorsAsText[j][i], maxLengthOfTextOfAncestorVariables, getColor(getIndexOfIOITSBF(j)), h); cout << "      ";
            }
            cout << endl;
        }
        for(int i = 0; i < individualCount; i++) { renderCover(maxLengthOfTextOfAncestorVariables); cout << "      "; }

    }
    void renderChildren(HANDLE& h)
    {
        int individualCount = childrenAsText.size();
        int variableCount = childrenAsText[0].size();

        
        for(int i = 0; i < individualCount; i++) { renderCover(maxLengthOfTextOfAncestorVariables); cout << "      "; }
        cout << endl;
        for(int i = 0; i < variableCount; i++)
        {
            for(int j = 0; j < individualCount; j++)
            {
                renderCell(childrenAsText[j][i], maxLengthOfTextOfAncestorVariables, getColor(colorsOfMatrix[j][i]), h); cout << "      ";
            }
            cout << endl;
        }
        for(int i = 0; i < individualCount; i++) { renderCover(maxLengthOfTextOfAncestorVariables); cout << "      "; }
    }
    void renderCell(string text, int maxLength, int color, HANDLE& h)
    {
        int textLength = text.length();
        int leftSpace = 2 + (maxLength - textLength) / 2;
        int rightSpace = leftSpace + (maxLength - textLength) % 2;
        
        cout << "*";
        for(int i = 0; i < leftSpace; i++) { cout << " "; };

        SetConsoleTextAttribute(h, color);
        cout << text;
        SetConsoleTextAttribute(h, 15);

        for(int i = 0; i < rightSpace; i++) { cout << " "; };
        cout << "*";
    }
    void renderCover(int maxLength)
    {
        int spaceLength = maxLength + 2;
        cout << "**";
        for(int i = 0; i < spaceLength; i++) { cout << " "; }
        cout << "**";
    }


public:
    int individualNumber;
    vector<vector<string>> ancestorsAsText;             // [anc][variable]
    int maxLengthOfTextOfAncestorVariables;
    int survivorCountOfAncestors;
    vector<vector<string>> childrenAsText;              // [chi][variable]
    int maxLengthOfTextOfChildrenVariables;

    vector<int> indexesOfAncestorsThatSortedByFitness;
    vector<float> indexesOfParents0;
    vector<float> indexesOfParents1;

    vector<vector<int>> mutationEvents;                 // [chi][variable]
    vector<vector<float>> crossoverPositions;

    vector<vector<int>> colorsOfMatrix;                 // [chi][variable]


    void renderIteration(HANDLE& h, bool lastIteration)
    {
        renderAncestors(h);

        int lengthOfLine = (maxLengthOfTextOfAncestorVariables + 12) * ancestorsAsText.size() - 6;
        int offsetOfArrow = lengthOfLine / 2 - 1;

        cout << endl << endl;
        for(int i = 0; i < offsetOfArrow; i++) { cout << " "; } cout << "||" << endl;
        for(int i = 0; i < offsetOfArrow; i++) { cout << " "; } cout << "vv" << endl;
        cout << endl << endl;

        generateTheColorsOfMatrix();
        renderChildren(h);
        
        if(!lastIteration)
        {
            lengthOfLine = (maxLengthOfTextOfChildrenVariables + 12) * childrenAsText.size() - 6;
            offsetOfArrow = lengthOfLine / 2 - 1;

            cout << endl << endl << endl;
            for(int i = 0; i < offsetOfArrow; i++) { cout << " "; } cout << "||" << endl;
            cout << endl << endl << endl;
        }
    }
    static int getColor(int index)
    {
        index = (index % 14) + 1;
        switch (index)
        {
            case 0: return 8;
            case 9: return 15;
            case 8: return 14;
            default: return index;
        }
    }
};

class GOP_Demonstrator      // Genetic Optimization Process Demonstrator
{
private:
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    vector<GOPD_Iteration> iterations;
    int iterationNumber = 0;
    int individualNumber = 0;
    static GOP_Demonstrator* instance;
    static bool instanceGenerated;
    GOP_Demonstrator() {}
    
    int getMaxLengthAmongStringMatrix(const vector<vector<string>>& matrix)
    {
        int maxLength = 0;
        int currentLength;
        int fL = matrix.size(); int sL = matrix[0].size();
        for(int i = 0; i < fL; i++)
        {
            for(int j = 0; j < sL; j++)
            {
                currentLength = matrix[i][j].length();
                if(maxLength < currentLength) { maxLength = currentLength; }
            }
        }

        return maxLength;
    }

public:
    static GOP_Demonstrator& getInstance()
    {
        if(!instanceGenerated)
        {
            instance = new GOP_Demonstrator();
            instanceGenerated = true;
        }

        return *instance;
    }
    void startSupply(int iterationCount)
    {
        iterationNumber = 0;
        iterations.resize(iterationCount);
    }
    void supplyByOptimizationUnit(vector<vector<string>> ancestorsAsText, int survivorCountOfAncestors)
    {
        iterations[iterationNumber] = GOPD_Iteration();
        iterations[iterationNumber].individualNumber = 0;
        iterations[iterationNumber].ancestorsAsText = ancestorsAsText;
        iterations[iterationNumber].maxLengthOfTextOfAncestorVariables = getMaxLengthAmongStringMatrix(ancestorsAsText);
        iterations[iterationNumber].survivorCountOfAncestors = survivorCountOfAncestors;
        if(iterationNumber > 0)
        {
            iterations[iterationNumber - 1].childrenAsText = iterations[iterationNumber].ancestorsAsText;
            iterations[iterationNumber - 1].maxLengthOfTextOfChildrenVariables = iterations[iterationNumber].maxLengthOfTextOfAncestorVariables;
        }

        iterationNumber++;
    }
    void endSupply(vector<vector<string>> lastChildrenAsText)
    {
        iterations[iterationNumber - 1].childrenAsText = lastChildrenAsText;
        iterations[iterationNumber - 1].maxLengthOfTextOfChildrenVariables = getMaxLengthAmongStringMatrix(lastChildrenAsText);
    }
    void supplyByPopulation(vector<float> indexesOfParents0, vector<float> indexesOfParents1,vector<int> indexesOfIndividualsThatSortedByFitness)
    {
        if(iterationNumber <= 0) { return; /*bir sorun var*/ }

        iterations[iterationNumber - 1].indexesOfParents0 = indexesOfParents0;
        iterations[iterationNumber - 1].indexesOfParents1 = indexesOfParents1;
        iterations[iterationNumber - 1].indexesOfAncestorsThatSortedByFitness = indexesOfIndividualsThatSortedByFitness;

        iterations[iterationNumber - 1].mutationEvents.resize(indexesOfParents0.size());
        iterations[iterationNumber - 1].crossoverPositions.resize(indexesOfParents0.size());
        individualNumber = 0;
    }
    void supplyByIndividual(vector<int> mutationEvents, vector<float> crossoverPositions)
    {
        if(iterationNumber <= 0) { return; /*bir sorun var*/ }
        
        iterations[iterationNumber - 1].mutationEvents[individualNumber] = mutationEvents;
        iterations[iterationNumber - 1].crossoverPositions[individualNumber] = crossoverPositions;

        individualNumber++;
    }
    void renderTheProcess()
    {
        cout << "hierarchy of fitness colors: ";
        for(int i = 0; i < 14; i++) { SetConsoleTextAttribute(h, GOPD_Iteration::getColor(i)); cout << i + 1 << " "; }
        cout << endl << endl;

        SetConsoleTextAttribute(h, 7);

        for(int i = 0; i < iterationNumber; i++)
        {
            iterations[i].renderIteration(h, i == iterationNumber - 1);
        }
    }
};

GOP_Demonstrator* GOP_Demonstrator::instance = nullptr;
bool GOP_Demonstrator::instanceGenerated = false;