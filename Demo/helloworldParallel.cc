#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cmath>

long unsigned num_steps{100'000};

unsigned numThreads = omp_get_num_threads();
unsigned itrrCountForEachThread = floor(num_steps / numThreads);
unsigned remainderSteps = num_steps % numThreads;

int main(int argc, char const *argv[])
{
    // parallel code
    long double pi, sum{0.0};
    double step{1.0 / num_steps};
    #pragma omp parallel num_threads(numThreads)
    {   
        long double x; // update x according to the index of the thread.
        int thisThreadNum = omp_get_thread_num();
        unsigned domainShift = thisThreadNum * itrrCountForEachThread;
        for (unsigned i = 0; i < itrrCountForEachThread; i++)
        {   
            x = (i + .5 + domainShift) * step;
            sum += 4.0 / (1.0 + pow(x, 2));
        }
    }

    // To reduce Error we do this operation in the end.
    pi = step * sum;

    std::cout << std::setprecision(20);
    std::cout << "The sum is: " << pi << std::endl;
    std::cout << "Number of threads used: " << numThreads << std::endl;
    return 0;
}
