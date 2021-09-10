#include <iostream>
#include <string>
using namespace std;

void const_test1();
void const_test2();
void generated_test1();
void generated_test2();

void test(void (*function)(), string text)
{
    cout << text << endl;
    function();
    cout << "Press ENTER to continue program execution." << endl;
    getchar();
}

int main()
{
    test(const_test1, "Test with constant values num.1");
    test(const_test2, "Test with constant values num.2");
    test(generated_test1, "Test with generated values num.1");
    test(generated_test2, "Test with generated values num.2");

    for (int i = 0; i < 10; i++)
        test(generated_test1, "Test with generated values num.1");

    return 0;
}

void print_array(string message, int arr[], const int len)
{
    cout << message;
    for (int i = 0; i < len; i++)
        cout << arr[i] << " ";
    cout << endl;
}

// �������� ����������� �������� val � ��������� set
bool in_set(const int val, const int len, int set[])
{
    for (int i = 0; i < len; i++)
        if (val == set[i])
            return true;
    return false;
}

// ����������� ���� �������� set1 � set2
void setIntersection(
    const int len1, int set1[],
    const int len2, int set2[],
    int set_res[], int& size
)
{
    for (int i = 0; i < len1; i++)
        if (in_set(set1[i], len2, set2))
            set_res[size++] = set1[i];
}

// ����������� ���� �������� set1 � set_res
void setUnion(
    const int len1, int set1[],
    int set_res[], int& size
)
{
    for (int i = 0; i < len1; i++)
        if (!in_set(set1[i], size, set_res))
            set_res[size++] = set1[i];
}

// ��������� � � � ����� ����� �������� 1 � 9
// ��������� C ����� ������� 9, ������� ������ � ��������� AB
// ��������� C ����� ������� 11, ������� ������ � ��������� AB+�
void const_test1()
{
    const int alen = 5, blen = 6, clen = 4, dlen = 9, elen = alen + blen + clen + dlen;
    int A[alen] = { 1, 7, 634, 82, 9 };
    int B[blen] = { 5, 9, 1, 43, 12, 21 };
    int C[clen] = { 9, 9000, -523, 11 };
    int D[dlen] = { 99, 88, 77, 66, 55, 44, 33, 22, 11 };
    int E[elen];
    int size = 0;

    print_array("Set A: ", A, alen);
    print_array("Set B: ", B, blen);
    print_array("Set C: ", C, clen);
    print_array("Set D: ", D, dlen);

    setIntersection(alen, A, blen, B, E, size);
    setUnion(clen, C, E, size);
    setUnion(dlen, D, E, size);


    print_array("Set E: ", E, size);
}

// ��������� A � B �� ����� ����� ���������
void const_test2()
{
    const int alen = 3, blen = 8, clen = 5, dlen = 1, elen = alen + blen + clen + dlen;
    int A[alen] = { -53, 43, 888 };
    int B[blen] = { 5, 6, 1, -11, 12, -21, 776, 9 };
    int C[clen] = { -50345, 51, -523, 3 };
    int D[dlen] = { -1 };
    int E[elen];
    int size = 0;

    print_array("Set A: ", A, alen);
    print_array("Set B: ", B, blen);
    print_array("Set C: ", C, clen);
    print_array("Set D: ", D, dlen);

    setIntersection(alen, A, blen, B, E, size);
    setUnion(clen, C, E, size);
    setUnion(dlen, D, E, size);

    print_array("Set E: ", E, size);
}

// �������� ��������� ������������ �������� �� �����������:
// - ������ 2 �������� ��������� ��������� � 2 ���������� ��������� �
// - ��������� ������� ��������� � ��������� �� ��������� ��������� ��������� �
// - ��� �������� ��������� D ��������� � ���������� ��������� C
// ��������� - ����� ��������������� ����� �� 0 �� 999
void generated_test1()
{
    const int arrlen = 8;
    int A[arrlen];
    int B[arrlen];
    int C[arrlen];
    int D[arrlen];
    int E[arrlen * 4];
    int size = 0;

    srand(time(NULL));
    for (int i = 0; i < arrlen; i++) {
        A[i] = rand() % 1000;
        B[i] = i <= 1 ? A[i] : rand() % 1000;
        C[i] = i == 7 ? A[rand() % i] : rand() % 1000;
        D[i] = i >= 5 and i <= 6 ? C[i] : rand() % 1000;
    }

    print_array("Set A: ", A, arrlen);
    print_array("Set B: ", B, arrlen);
    print_array("Set C: ", C, arrlen);
    print_array("Set D: ", D, arrlen);

    setIntersection(arrlen, A, arrlen, B, E, size);
    setUnion(arrlen, C, E, size);
    setUnion(arrlen, D, E, size);

    print_array("Set E: ", E, size);
}

// �������� ������� ��������� ������������ ��������
void generated_test2()
{
    const int arrlen = 10;
    int A[arrlen];
    int B[arrlen];
    int C[arrlen];
    int D[arrlen];
    int E[arrlen * 4];
    int size = 0;

    srand(time(NULL));
    for (int i = 0; i < arrlen; i++)
        A[i] = rand(), B[i] = rand(), C[i] = rand(), D[i] = rand();

    print_array("Set A: ", A, arrlen);
    print_array("Set B: ", B, arrlen);
    print_array("Set C: ", C, arrlen);
    print_array("Set D: ", D, arrlen);

    setIntersection(arrlen, A, arrlen, B, E, size);
    setUnion(arrlen, C, E, size);
    setUnion(arrlen, D, E, size);

    print_array("Set E: ", E, size);
}