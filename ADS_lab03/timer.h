#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

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