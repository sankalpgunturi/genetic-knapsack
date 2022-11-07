// SIMD version of genetic_baseline
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <set.h>

#define NUMBER_OF_ITEMS 12
#define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 256

double fitness(double *weights, double *values, double *representation)
{
    double totalWeight = 0;
    double totalValue = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        // FMA
        totalWeight += weights[i] * representation[i];
        totalValue += values[i] * representation[i];
    }
    if (totalWeight > MAX_KNAPSACK_WEIGHT)
    {
        // penalty: negative feedback
        return 0;
    }
    return totalValue;
}

// [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 | 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 | 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 | 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 .. 255]

double *selection(double *weights, double *values, double *initial_population)
{
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));
    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i += 2)
    {
        if (fitness(weights, values, initial_population[i]) > fitness(weights, values, initial_population[i + 1]))
        {
            winners[i / 2] = initial_population[i];
        }
        else
        {
            winners[i / 2] = initial_population[i + 1];
        }
    }
    return winners;
}

int *generate_random(int l, int r, int count)
{ // this will generate random number in range l and r
    int i;
    
    int *indexes = (int *)malloc(SIZE_OF_INITIAL_POPULATION * sizeof(int));
    for (i = 0; i < count; i++)
    {
        int rand_num = (rand() % (r - l + 1)) + l;
        indexes[i] = rand_num;
    }
    return indexes;
}

int main()
{
    // [popsize][n]
    // popsize*n
    double *population;
    posix_memalign((void *)&population, 64, NUMBER_OF_ITEMS * SIZE_OF_INITIAL_POPULATION * sizeof(double));

    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i++)
    {
        int
    }
    srand(time(NULL));
    for (int i = 0; i != POPULATION_SIZE; ++i)
    {
        for (int k = i; k != i + 12; ++k)
        {
            // int t = rand()%15;
            // t = t % 5;

            int t = 0 + (int)(rand() / (double)(RAND_MAX + 1.0) * (1 - 0 + 1));
            population[k] = (double)t;
            printf("%.2f \t", population[k]);
        }
        printf("\n");
    }
    double weights[12] = {4, 5, 4, 12, 9, 0, 3, 1, 15, 7, 8, 1};
    double values[12] = {43, 89, 10, 2, 56, 78, 12, 34, 44, 9, 18, 0};
    // double **initial_population = (double **)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double *));

    // TODO: Need to convert 2D Array to 1D in selection function or vice-versa
    selection(weights, values, population);

    return 0;
}

// input n individuals(fitness array with length = n)
// 256 items
// representations: n * 256 bit (n rows, 4 columns)
// void crossover(int n, double *fitness, double *representations)
// {
//     int m = 4;
//     for (int i = 0; i < n; i += 2)
//     {
//         // load
//         __m256d P1 = _mm256_load_pd(representations[i * 4]);
//         __m256d P2 = _mm256_load_pd(representations[(i + 1) * 4]);
//         // permute
//         __m256d C1 = mm256_permute2f128_pd(P1, P2, (0 | (3 << 4)));
//         __m256d C2 = mm256_permute2f128_pd(P1, P2, (1 | (2 << 4)));
//         // load into memory
//         _mm256_storeu_pd(&representations[i * 4], C1);
//         _mm256_storeu_pd(&representations[(i + 1) * 4], C2);
//     }
// }

// void mutation()
// {
// }

// Random number generator
// Source: https://www.tutorialspoint.com/generating-random-number-in-a-range-in-c
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// main() {
//    int lower = 10, upper = 15, count = 15;
//    srand(time(0)); //current time as seed of random number generator
//    generate_random(lower, upper, count);
//    printf("
// ");
// }