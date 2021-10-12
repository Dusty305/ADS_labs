#ifndef arrayset_h
#define arrayset_h

#include <iostream>

using namespace std;

class ArraySet
{
private:
    char array[UnSize+1];
public:

    const char& operator[](size_t i)const
    {
        return array[i];
    }

    char& operator[](size_t i)
    {
        return array[i];
    }

    ArraySet& operator|=(const ArraySet& arr2)
    {
        int k = 0;
        bool in_result;
        ArraySet result(*this);

        while (result[k])
            k++;

        for (int i = 0; arr2[i]; i++) {
            in_result = false;
            for (int j = 0; result[j]; j++)
                if (arr2[i] == result[j])
                {
                    in_result = true;
                    break;
                }
            if (!in_result)
                result[k++] = arr2[i];
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