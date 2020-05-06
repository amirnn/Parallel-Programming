#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cmath>

long unsigned num_steps{100'000};

int main(int argc, char const *argv[])
{   
    // Serial part for setup
    // unsigned numThreads = omp_get_num_threads(); 
    unsigned numThreads = omp_get_max_threads();
    std::cout << "Number of threads used: " << numThreads << std::endl;
    omp_set_num_threads(numThreads);
    unsigned itrrCountForEachThread = floor(num_steps / numThreads);
    unsigned remainderSteps = num_steps % numThreads;
    std::cout << "itrrCountForEachThread: " << itrrCountForEachThread << std::endl;
    // parallel code starts here.
    long double pi, sum{0.0};
    double step{1.0 / num_steps};
    #pragma omp parallel num_threads(numThreads)
    {   
        long double x; // update x according to the index of the thread.
        unsigned thisThreadNum = omp_get_thread_num();
        std::cout << "thisThreadNum: " << thisThreadNum << std::endl;
        unsigned domainShift = thisThreadNum * itrrCountForEachThread;
        std::cout << "domainShift: " << domainShift << std::endl;
        for (unsigned i = 0; i < itrrCountForEachThread; ++i)
        {   
            x = (i + .5 + domainShift) * step;
            sum += 4.0 / (1.0 + pow(x, 2));
        }
    }

    // Thread team joins here -- serial execution again starts from here.

    // ToDo: Do the remaining summation of the elements since I used mod operataor above to
    // determine the number of threads.
    long double x;
    for(unsigned i = 0; i < remainderSteps; i++){
        x = 1 - ((i + .5) * step);
            sum += 4.0 / (1.0 + pow(x, 2));
    }

    // To reduce Error we do this operation in the end.
    pi = step * sum;
    std::cout << std::setprecision(20);
    std::cout << "The sum is: " << pi << std::endl;
    return 0;
}
