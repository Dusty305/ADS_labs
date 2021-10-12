#ifndef wordset_h
#define wordset_h

#include <iostream>

using namespace std;

class WordSet
{
private:
    int word;

public:

    WordSet()
    { }

    WordSet(char arr[])
    { 
        word = 0;
        for (int i = 0; arr[i]; i++)
            word |= (1 << (arr[i] - '0'));
    }

    void operator|=(WordSet& set)
    {
        word = word | set.word;
    }

    WordSet operator&(WordSet& set)
    {
        WordSet result;
        result.word = word & set.word;
        return move(result);
    }

    friend ostream& operator<<(ostream& os, WordSet& set)
    {
        for (int i = 9; i >= 0; i--) {
            os << ((set.word >> i) & 1);
        }
            
        return os;
    }
};

#endif