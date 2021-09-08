#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>
#include <float.h>

// #define __SAME_DATA__

int max_fn(int* array, const int size, const int threads_num)
{
    int max = array[0];
    #pragma omp parallel for num_threads(threads_num) reduction(max: max)
    for (int i = 0; i < size; ++i)
        max = array[i] > max ? array[i] : max;

    return max;
}

int main(int argc, char** argv)
{
    printf("OpenMP version: %d\n", _OPENMP);

    const int TESTS_NUM = 10;
    const int THREADS_NUM_MAX = 16;
    const int size = 1e8;
    int* array = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < TESTS_NUM; ++i)
    {
        srand(time(NULL));

#ifndef __SAME_DATA__
        for (int j = 0; j < size; ++j)
            array[j] = rand();
#else
        memset(array, 0, size * sizeof(int));
#endif /* ! __SAME_DATA__ */

        double times[THREADS_NUM_MAX];
        for (int threads_num = 1; threads_num <= THREADS_NUM_MAX; ++threads_num)
        {
            double start = omp_get_wtime(); 
            max_fn(array, size, 5);
            double end = omp_get_wtime(); 
            
            times[threads_num - 1] = end - start;

            printf ("Test #%d threads_num: %2d time: %fs\n", i + 1, threads_num, end - start);
        }

        int pos = -1;
        double min = DBL_MAX;
        for (int j = 0; j < THREADS_NUM_MAX; ++j)
            if (times[j] < min)
            {
                min = times[j];
                pos = j;
            }

        printf("Best time: %f threads_num: %d\n", min, pos + 1);
    }

    free(array);

    return 0;
}