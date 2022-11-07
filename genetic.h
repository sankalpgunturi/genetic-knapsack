#include "immintrin.h"

#define NUMBER_OF_ITEMS 12
#define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 256

double fitness(double *weights, double *values, double *representation)
{
    __m256d valuesSIMD = _mm256_loadu_pd(&values);
    __m256d weightsSIMD = _mm256_loadu_pd(&weights);
    __m256d representationSIMD = _mm256_loadu_pd(&representation);

    __m256d totalWeight = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    __m256d totalValue = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < NUMBER_OF_ITEMS; i++)
    {
        _mm256_fmadd_pd(weightsSIMD, representationSIMD, totalWeight);
        _mm256_fmadd_pd(valuesSIMD, representationSIMD, totalValue);
    }
}


double *selection(double *weights, double *values, double *initial_population)
{
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));

    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i += 2)
    {
        result = _mm256_cmp_pd(fitness(weights, values, initial_population[i]), fitness(weights, values, initial_population[i + 1]), winnersSIMD);
        if (result == 1)
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

void crossover_baseline()
{
}

void mutation_baseline()
{
}