#include <iostream>
#include <string>
#include <array>
#include <list>
#include <chrono>
#include <charconv>
using namespace std;
using namespace chrono;

const int UnSize = 10;
using Array = array<char, UnSize + 1>;
using List = list<char>;

class Timer
{
private:
    time_point<steady_clock> start, end;
    duration<float> duration;
public:

    Timer()
    {
        start = high_resolution_clock::now();
    }

    ~Timer()
    {
        end = high_resolution_clock::now();
        duration = end - start;
        float ms = duration.count() * 1000.0f;
        cout << "Duration: " << ms << "ms\n";
    }
};

void operator|=(Array& result, Array& arr2)
{
    int k = 0;
    bool in_result;

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
}

Array operator|(Array& arr1, Array& arr2)
{
    Array result = { '\0' };
    int k = 0;
    result |= arr1;
    result |= arr2;
    return move(result);
}

Array operator&(Array& arr1, Array& arr2)
{
    Array result = { '\0' };
    int k = 0;

    for (int i = 0; arr1[i]; i++)
        for (int j = 0; arr2[j]; j++)
            if (arr1[i] == arr2[j])
                result[k++] = arr1[i];

    return move(result);
}

ostream& operator<<(ostream& os, Array& arr)
{
    for (auto it = arr.cbegin(); it != arr.cend() && *it != '\0'; it++)
        os << *it << " ";
    return os;
}

void operator|=(List& result, List& list2)
{
    bool in_set;

    for (auto it1 = list2.cbegin(); it1 != list2.cend() and *it1 != '\0'; it1++) {
        in_set = false;
        for (auto it2 = result.cbegin(); it2 != result.cend(); it2++)
            if (*it1 == *it2)
            {
                in_set = true;
                break;
            }
        if (!in_set)
            result.push_back(*it1);
    }
}

List operator&(List& list1, List& list2)
{
    List result;
    for (auto it1 = list1.cbegin(); *it1; it1++)
        for (auto it2 = list2.cbegin(); *it2; it2++)
            if (*it1 == *it2)
                result.push_back(*it1);
    return move(result);
}

ostream& operator<<(ostream& os, List l)
{
    for (char c : l)
        os << c << " ";
    return os;
}


class WordSet
{
private:
    int word;

public:

    WordSet()
    { }

    WordSet(Array arr)
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

class BitSet
{
private:
    int bits[UnSize] = { 0 };

public:

    BitSet()
    { }

    BitSet(Array arr)
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

template<class Set>
Set SetTest(Set set1, Set set2, Set set3, Set set4, const int repeat = 1)
{
    Timer timer;
    Set set_result;
    for (int i = 0; i < repeat; i++) {
        set_result = set1 & set2;
        set_result |= set3;
        set_result |= set4;
    }
    return set_result;
}

void test(Array A, Array B, Array C, Array D)
{
    cout << "\nArray test\n";
    Array array_result = SetTest<Array>(Array(A), Array(B), Array(C), Array(D), 1000000);
    cout << "Array result: " << array_result << "\n";

    cout << "\nList test\n";
    List list_result = SetTest<List>(List(A.cbegin(), A.cend()), List(B.cbegin(), B.cend()), List(C.cbegin(), C.cend()), List(D.cbegin(), D.cend()), 1000000);
    cout << "List result: " << list_result << "\n";

    cout << "\nMachine word test\n";
    WordSet word_result = SetTest<WordSet>(WordSet(A), WordSet(B), WordSet(C), WordSet(D), 1000000);
    cout << "Machine word result: " << word_result << " (from 9 to 0)\n";

    cout << "\nBit array test\n";
    BitSet bit_result = SetTest<BitSet>(BitSet(A), BitSet(B), BitSet(C), BitSet(D), 1000000);
    cout << "Bit array result: " << bit_result << " (from 9 to 0)\n";
}

void constant_test()
{
    cout << "Constant test. E = AB + C + D\n";
    Array A = { "01569" },
          B = { "024578" },
          C = { "69" },
          D = { "348" };
    cout << "Array A: " << A << "\n"
         << "Array B: " << B << "\n"
         << "Array C: " << C << "\n"
         << "Array D: " << D << "\n";

    test(A, B, C, D);
}

void generated_test()
{
    cout << "Generated test\n";
    srand(time(NULL));
    Array A = { '\0' }, B = { '\0' }, C = { '\0' }, D = { '\0' };

    to_chars(A.data(), A.data() + A.size(), rand());
    to_chars(B.data(), B.data() + B.size(), rand());
    to_chars(C.data(), C.data() + C.size(), rand());
    to_chars(D.data(), D.data() + D.size(), rand());

    A = A | A; B = B | B; C = C | C; D = D | D;

    cout << "Array A: " << A << "\n"
         << "Array B: " << B << "\n"
         << "Array C: " << C << "\n"
         << "Array D: " << D << "\n";

    test(A, B, C, D);
}

int main()
{
    constant_test();
    cout << "\n-------------------------------------------\n";
    generated_test();
    
    return 0;
}