#ifndef arrayset_h
#define arrayset_h

#include <iostream>

using namespace std;

class ArraySet
{
private:
    char array[UnSize + 1] = { '\0' };
public:

    ArraySet()
    { }

    ArraySet(char arr[])
    {
        int k = 0;
        while (arr[k])
            array[k++] = arr[k];
        array[k] = '\0';
    }

    const char& operator[](size_t i)const
    {
        return array[i];
    }

    char& operator[](size_t i)
    {
        return array[i];
    }

    ArraySet& operator|=(const ArraySet& set)
    {
        for (int i = 0; set[i]; i++) {
            bool in_result = false;
            int k = 0;
            for (k = 0; (*this)[k]; k++)
                if (set[i] == (*this)[k])
                {
                    in_result = true;
                    break;
                }
            if (!in_result)
                (*this)[k++] = set[i];
        }
        return *this;
    }

    ArraySet operator|(const ArraySet& arr1)const
    {
        ArraySet result(*this);
        result |= arr1;
        return move(result);
    }

    ArraySet operator&(const ArraySet& arr1)
    {
        ArraySet result;
        ArraySet arr2(*this);
        int k = 0;

        for (int i = 0; arr1[i]; i++)
            for (int j = 0; arr2[j]; j++)
                if (arr1[i] == arr2[j])
                    result[k++] = arr1[i];

        result[k] = '\0';
        return move(result);
    }

    friend ostream& operator<<(ostream& os, ArraySet& arr)
    {
        for (int i = 0; arr[i]; i++)
            os << arr[i] << " ";
        return os;
    }

};

#endif