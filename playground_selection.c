#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "immintrin.h"
#define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 256
#define NUMBER_OF_ITEMS 12
double *generateRandomNumber(lower, upper, count)
{
    int i;
    double *res = (double *)malloc(count * sizeof(double));
    for (i = 0; i < count; i++)
    {
        int num = (rand() %
                   (upper - lower + 1)) +
                  lower;
        res[i] = num;
    }
    return res;
}
double *fitness(double *weights, double *values_d, double *initial_population)
{
    double *res = (double *)malloc(256 * sizeof(double));
    res = generateRandomNumber(0, 15, 256);
    return res;
}
double *selection(double *weights, double *values_d, double *initial_population)
{
    double *contenders = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * NUMBER_OF_ITEMS * sizeof(double));
    contenders = fitness(weights, values_d, initial_population);
    // for (int i = 0; i < 256; i++)
    // {
    //     printf("%lf ", contenders[i]);
    // }
    // printf("\n");
    int i = 0;
    __m256d contenders_set_0;
    __m256d contenders_set_1;
    __m256d contenders_set_2;
    __m256d contenders_set_3;
    __m256d contenders_set_4;
    __m256d contenders_set_5;
    __m256d contenders_set_6;
    __m256d contenders_set_7;
    __m256d contenders_set_8;
    __m256d contenders_set_9;
    __m256d result_0;
    __m256d result_1;
    __m256d result_2;
    __m256d result_3;
    __m256d result_4;
    for (int i = 0; i <= 200; i += 40)
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
        for (int j = 0; j <= (SIZE_OF_INITIAL_POPULATION / 2) - 3; j++)
        {
            if (_mm256_movemask_pd(result_0) == 15)
            {
                winners[j] = initial_population[i + 0];
            }
            else
            {
                winners[j] = initial_population[i + 4];
            }
            if (_mm256_movemask_pd(result_1) == 15)
            {
                winners[j] = initial_population[i + 8];
            }
            else
            {
                winners[j] = initial_population[i + 12];
            }
            if (_mm256_movemask_pd(result_2) == 15)
            {
                winners[j] = initial_population[i + 16];
            }
            else
            {
                winners[j] = initial_population[i + 20];
            }
            if (_mm256_movemask_pd(result_3) == 15)
            {
                winners[j] = initial_population[i + 24];
            }
            else
            {
                winners[j] = initial_population[i + 28];
            }
            if (_mm256_movemask_pd(result_4) == 15)
            {
                winners[j] = initial_population[i + 32];
            }
            else
            {
                winners[j] = initial_population[i + 36];
            }
        }
    }
    i = 240;
    contenders_set_0 = _mm256_loadu_pd(&contenders[i]);
    contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
    contenders_set_2 = _mm256_loadu_pd(&contenders[i + 8]);
    contenders_set_3 = _mm256_loadu_pd(&contenders[i + 12]);
    result_0 = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14);
    result_1 = _mm256_cmp_pd(contenders_set_2, contenders_set_3, 14);
    if (_mm256_movemask_pd(result_0) == 15)
    {
        winners[(SIZE_OF_INITIAL_POPULATION / 2) - 2] = initial_population[i];
    }
    else
    {
        winners[(SIZE_OF_INITIAL_POPULATION / 2) - 2] = initial_population[i + 4];
    }
    if (_mm256_movemask_pd(result_1) == 15)
    {
        winners[(SIZE_OF_INITIAL_POPULATION / 2) - 1] = initial_population[i + 8];
    }
    else
    {
        winners[(SIZE_OF_INITIAL_POPULATION / 2) - 1] = initial_population[i + 12];
    }
    return winners;
}
int main()
{
    double *weights = (double *)malloc(12 * sizeof(double));
    double *values_d = (double *)malloc(12 * sizeof(double));
    weights = generateRandomNumber(0, 15, 12);
    values_d = generateRandomNumber(0, 15, 12);
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));
    double *population;
    int POPULATION_SIZE = 256;
    int NUMBER_OF_GENERATIONS = 10000;
    posix_memalign((void *)&population, 64, POPULATION_SIZE * 12 * sizeof(double));
    srand(time(NULL));
    int skip_index = 15;
    FILE *fp = fopen("POPULATION_4096.txt", "r");
    if (fp == NULL)
    {
        printf("Couldn't open file\n");
        return 0;
    }
    char buff[1];
    for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++)
    {
        if (i == 0)
        {
            for (int k = 0; k < 12 * 14; k++)
            {
                fscanf(fp, "%s", buff);
            }
            // printf("\n");
        }
        else if ((i) % 12 == 0)
        {
            // After every 12 items
            for (int k = 0; k < 12 * 14; k++)
            {
                fscanf(fp, "%s", buff);
            }
            // printf("\n");
        }
        fscanf(fp, "%s", buff);
        population[i] = buff[0] - 48.0;
        //  printf(" %.2f ", population[i]);
    }
    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            printf("%.2f ", population[i * 12 + j]);
        }
        printf("\n");
    }
    printf(" ----------------- \n");
    winners = selection(weights, values_d, population);
    // 256/2
    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION / 2; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            printf("%.2f ", winners[i * 12 + j]);
        }
        printf("\n");
    }
    // for (int i = 0; i < SIZE_OF_INITIAL_POPULATION / 2; i++)
    // {
    //     printf(" %.2f ", winners[i]);
    // }
    return 0;
}