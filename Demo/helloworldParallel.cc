#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cmath>

long unsigned num_steps {100'000};



int main(int argc, char const *argv[])
{
    long double x, pi, sum{0.0};
    double step {1.0/num_steps};

    for (unsigned i=0; i< num_steps; i++)
    {
        x = (i+.5) * step;
        sum += 4.0 / (1.0 + pow(x,2));
    }

    // To reduce Error we do this operation in the end.
    pi = step * sum;

    std::cout << std::setprecision(20);
    std::cout << "The sum is: " << pi << std::endl;
    return 0;
}
