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

Array operator|=(Array& result, Array& arr2)
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

    return result;
}

Array operator|(Array& arr1, Array& arr2)
{
    Array result = { '\0' };
    int k = 0;
    result |= arr1;
    result |= arr2;
    return result;
}

Array operator&(Array& arr1, Array& arr2)
{
    Array result = { '\0' };
    int k = 0;

    for (int i = 0; arr1[i]; i++)
        for (int j = 0; arr2[j]; j++)
            if (arr1[i] == arr2[j])
                result[k++] = arr1[i];

    return result;
}

ostream& operator<<(ostream& os, Array& arr)
{
    for (auto it = arr.begin(); it != arr.end() && *it != '\0'; it++)
        os << *it << " ";
    return os;
}

List operator|=(List& result, List& list2)
{
    bool in_set;

    for (auto it1 = list2.begin(); it1 != list2.end() and *it1 != '\0'; it1++) {
        in_set = false;
        for (auto it2 = result.begin(); it2 != result.end(); it2++)
            if (*it1 == *it2)
            {
                in_set = true;
                break;
            }
        if (!in_set)
            result.push_back(*it1);
        
    }

    return result;
}

List operator&(List& list1, List& list2)
{
    List result;
    for (auto it1 = list1.begin(); *it1; it1++)
        for (auto it2 = list2.begin(); *it2; it2++)
            if (*it1 == *it2)
                result.push_back(*it1);
    return result;
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

    WordSet operator|=(WordSet& set)
    {
        word = word | set.word;
        return *this;
    }

    WordSet operator&(WordSet& set)
    {
        WordSet result;
        result.word = word & set.word;
        return result;
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

    BitSet operator|=(BitSet& set)
    {
        for (int i = 0; i < UnSize; i++)
            bits[i] = bits[i] || set.bits[i];
        return *this;
    }

    BitSet operator&(BitSet& set)
    {
        BitSet result;
        for (int i = 0; i < UnSize; i++)
            result.bits[i] = bits[i] && set.bits[i];
        return result;
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
    Array array_result = SetTest<Array>(Array(A), Array(B), Array(C), Array(D), 100);
    cout << "Array result: " << array_result << "\n";

    cout << "\nList test\n";
    List list_result = SetTest<List>(List(A.begin(), A.end()), List(B.begin(), B.end()), List(C.begin(), C.end()), List(D.begin(), D.end()), 100);
    cout << "List result: " << list_result << "\n";

    cout << "\nMachine word test\n";
    WordSet word_result = SetTest<WordSet>(WordSet(A), WordSet(B), WordSet(C), WordSet(D), 100);
    cout << "Machine word result: " << word_result << "\n";

    cout << "\nBit array test\n";
    BitSet bit_result = SetTest<BitSet>(BitSet(A), BitSet(B), BitSet(C), BitSet(D), 100);
    cout << "Bit array result: " << bit_result << "\n";
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