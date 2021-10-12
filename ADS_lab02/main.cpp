#include <iostream>
#include <string>
#include <array>
#include <list>
#include <chrono>
#include <charconv>

const int UnSize = 10; //size for all sets

#include "wordset.h"
#include "bitset.h"
#include "arrayset.h"
#include "listset.h"

/*class Timer
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
};*/

template <class Set>
Set SetTest(Set set1, Set set2, Set set3, Set set4, const int repeat = 1)
{
    //Timer timer;
    Set set_result;
    for (int i = 0; i < repeat; i++)
    {
        set_result = set1 & set2;
        set_result |= set3;
        set_result |= set4;
    }
    return set_result;
}

void test(char A[], char B[], char C[], char D[])
{
    cout << "\nArray test\n";
    ArraySet array_result = SetTest<ArraySet>(ArraySet(A), ArraySet(B), ArraySet(C), ArraySet(D), 1000000);
    cout << "Array result: " << array_result << "\n";

    cout << "\nList test\n";
    ListSet list_result = SetTest<ListSet>(ListSet(A), ListSet(B), ListSet(C), ListSet(D), 1000000);
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
    char A[] = {"01569"},
         B[] = {"024578"},
         C[] = {"69"},
         D[] = {"348"};
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
    char A[UnSize + 1] = {'\0'}, B[UnSize + 1] = {'\0'}, C[UnSize + 1] = {'\0'}, D[UnSize + 1] = {'\0'};

    to_chars(A.data(), A.data() + A.size(), rand());
    to_chars(B.data(), B.data() + B.size(), rand());
    to_chars(C.data(), C.data() + C.size(), rand());
    to_chars(D.data(), D.data() + D.size(), rand());

    A = A | A;
    B = B | B;
    C = C | C;
    D = D | D;

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