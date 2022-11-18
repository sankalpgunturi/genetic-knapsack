#include <stdio.h>
#include <stdlib.h>
#include "immintrin.h"

#define NUMBER_OF_ITEMS 12
#define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 256

double *selection(double *weights, double *values_d, double *initial_population)
{
    double *contenders = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));
    contenders = fitness(weights, values_d, initial_population);

    __m256d contenders_set_0 = _mm256_loadu_pd(&contenders[i]);
    __m256d contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
    __m256d contenders_set_2 = _mm256_loadu_pd(&contenders[i + 8]);
    __m256d contenders_set_3 = _mm256_loadu_pd(&contenders[i + 12]);
    __m256d contenders_set_4 = _mm256_loadu_pd(&contenders[i + 16]);
    __m256d contenders_set_5 = _mm256_loadu_pd(&contenders[i + 20]);
    __m256d contenders_set_6 = _mm256_loadu_pd(&contenders[i + 24]);
    __m256d contenders_set_7 = _mm256_loadu_pd(&contenders[i + 28]);
    __m256d contenders_set_8 = _mm256_loadu_pd(&contenders[i + 32]);
    __m256d contenders_set_9 = _mm256_loadu_pd(&contenders[i + 36]);

    __m256d result_0 = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14);
    __m256d result_1 = _mm256_cmp_pd(contenders_set_2, contenders_set_3, 14);
    __m256d result_2 = _mm256_cmp_pd(contenders_set_4, contenders_set_5, 14);
    __m256d result_3 = _mm256_cmp_pd(contenders_set_6, contenders_set_7, 14);
    __m256d result_4 = _mm256_cmp_pd(contenders_set_8, contenders_set_9, 14);

    for (int i = 40; i < SIZE_OF_INITIAL_POPULATION; i += 40)
    {
        // 10 registers
        contenders_set_0 = _mm256_loadu_pd(&contenders[i]);
        contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
        contenders_set_2 = _mm256_loadu_pd(&contenders[i + 8]);
        contenders_set_3 = _mm256_loadu_pd(&contenders[i + 12]);
        contenders_set_4 = _mm256_loadu_pd(&contenders[i + 16]);
        contenders_set_5 = _mm256_loadu_pd(&contenders[i + 20]);
        contenders_set_6 = _mm256_loadu_pd(&contenders[i + 24]);
        contenders_set_7 = _mm256_loadu_pd(&contenders[i + 28]);
        contenders_set_8 = _mm256_loadu_pd(&contenders[i + 32]);
        contenders_set_9 = _mm256_loadu_pd(&contenders[i + 36]);

        // 5 registers
        result_0 = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14);
        result_1 = _mm256_cmp_pd(contenders_set_2, contenders_set_3, 14);
        result_2 = _mm256_cmp_pd(contenders_set_4, contenders_set_5, 14);
        result_3 = _mm256_cmp_pd(contenders_set_6, contenders_set_7, 14);
        result_4 = _mm256_cmp_pd(contenders_set_8, contenders_set_9, 14);

        for (int j = 0; j < SIZE_OF_INITIAL_POPULATION / 2; j += 2)
        {
            if (_mm256_movemask_pd(result_0) == 15)
            {
                winners[j] = _mm256_loadu_pd(&initial_population[j]);
            }
            else
            {
                winners[j] = _mm256_loadu_pd(&initial_population[j + 1]);
            }
        }
    }
    return winners;
}

int main()
{
    double *weights = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *values_d = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *initial_population = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    selection(weights, values_d, initial_population);
    return 0;
}