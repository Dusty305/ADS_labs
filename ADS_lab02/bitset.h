#ifndef bitset_h
#define bitset_h

#include <iostream>

using namespace std;

class BitSet
{
private:
    int bits[UnSize] = { 0 };

public:

    BitSet()
    { }

    BitSet(char arr[])
    {
        for(int i = 0; arr[i]; i++)
            bits[arr[i] - '0'] = 1;
    }

    void operator|=(BitSet& set)
    {
        for (int i = 0; i < UnSize; i++)
            bits[i] = bits[i] || set.bits[i];
    }

    BitSet operator&(BitSet& set)
    {
        BitSet result;
        for (int i = 0; i < UnSize; i++)
            result.bits[i] = bits[i] && set.bits[i];
        return move(result);
    }

    friend ostream& operator<<(ostream& os, BitSet& set)
    {
        for (int i = 9; i >= 0; i--)
            os << set.bits[i];
        return os;
    }
};

#endif