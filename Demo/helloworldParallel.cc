#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cmath>
#include <array>
#include <numeric>

long unsigned num_steps{10'000'000};

int main(int argc, char const *argv[])
{
    // Serial part for setup
    unsigned numThreadsGiven = 0;
    const unsigned numThreads = 8;   //since it is literal it is constantexpr.
    omp_set_num_threads(numThreads); // Set number of threads.
    unsigned itrrCountForEachThread = floor(num_steps / numThreads);
    unsigned remainderSteps = num_steps % numThreads;

    // parallel setup
    long double pi;
    long double sum{0}; //size should be know at compile time
    double step{1.0 / num_steps};
// parallel code starts here.
#pragma omp parallel num_threads(numThreads)
    {
        long double x; // update x according to the index of the thread.
        long double threadSum{0};
        unsigned thisThreadNum = omp_get_thread_num();
        
        unsigned domainShift = thisThreadNum * itrrCountForEachThread;
        
        for (unsigned i = 0; i < itrrCountForEachThread; ++i)
        {
            x = (i + .5 + domainShift) * step;
            threadSum += 4.0 / (1.0 + pow(x, 2)); // to overcome the race condition each thread updates one part of the array.
        }

        // Update the assigned number of threads.
        if (thisThreadNum == 0)
            numThreadsGiven = omp_get_num_threads();

#pragma omp critical
        {
            sum += threadSum;
            std::cout << "thisThreadNum: " << thisThreadNum << std::endl;
            std::cout << "domainShift: " << domainShift << std::endl;
        }
    }

    // Thread team joins here -- serial execution again starts from here.

    // ToDo: Do the remaining summation of the elements since I used mod operataor above to
    // determine the number of threads.
    long double x;
    for (unsigned i = 0; i < remainderSteps; i++)
    {
        x = 1 - ((i + .5) * step);
        // put the remainders in the last element numThreads => numThreads+1 position.
        sum += 4.0 / (1.0 + pow(x, 2));
    }

    // To reduce Error we do this operation in the end.
    pi = step * sum;
    std::cout << std::setprecision(20);
    std::cout << "Number of threads requested: " << numThreads << std::endl;
    std::cout << "itrrCountForEachThread: " << itrrCountForEachThread << std::endl;
    std::cout << "numThreadsGiven: " << numThreadsGiven << std::endl;
    std::cout << "The sum is: " << pi << std::endl;
    if(numThreadsGiven!=numThreads)
    std::cerr << " Number of threads requested and assigned by OS doesn't match." << std::endl;
    return 0;
}
