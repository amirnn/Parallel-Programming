#include <stdio.h>
#include <omp.h>
int main(int argc, char const *argv[])
{
    #pragma omp parallel
    {
        int id=0;
        printf("hello %d \n", id);
    }
    return 0;
}
