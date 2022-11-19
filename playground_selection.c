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
    double *contenders;
    
    posix_memalign((void*) &contenders, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double)); // (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *winners; //(double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double)); //
    posix_memalign((void*) &winners, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));
    contenders = fitness(weights, values_d, initial_population);
    __m256d contenders_set_0 , contenders_set_1; // contenders_set_2, contenders_set_3, contenders_set_4, contenders_set_5;
    __m256d rep_0_1, rep_0_2, rep_0_3, rep_1_1, rep_1_2, rep_1_3;
    __m256d winner_0, winner_1, winner_2;
    __m256d compare;
    int f = 0;
    for (int i = 4; i < 256; i+=4*2)
    {   printf("i: %d\n", i);
        contenders_set_0 = _mm256_loadu_pd(&contenders[i]);     
        contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
        compare = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14); 

        printf("contender_set_0_index: i : %d\n",i);
        printf("contender_set_1_index: i+4: %d\n",i+4);
        printf("winner f %d\n", f);

        // printf("i = 0: k = 0; rep_0: 0 to 3, 4 to 7, 8 to 11\n");
        // printf("i = 0: k = 0; rep_1: 48 to 51, 52 to 55, 56 to 59\n");
        // printf("i = 0: k = 1; rep_0: 12 to 15, 16 to 19, 20 to 23\n");
        // printf("i = 0: k = 1; rep_1: 60 to 63, 64 to 67, 68 to 71\n");
        // printf("i = 0: k = 2; rep_0: 24 to 27, 28 to 31, 32 t0 35\n");
        // printf("i = 0: k = 2; rep_1: 72 to 75, 76 to 79, 80 to 83\n");

        for (int k = 0; k < 4; k++) { 
            //For each winner
            printf("k: %d\n", k);
            // printf("rep_0_1: (i+k) %d to %d\n",(i*12+k*12), (i*12+k*12+3));
            // printf("rep_0_2: (i+k+4) %d to %d\n",(i*12+k*12+4), (i*12+k*12+7));
            // printf("rep_0_3: (i+k+8) %d to %d\n",(i*12+k*12+8), (i*12+k*12+11));

            // printf("rep_1_1: (i+(k+4)*12) %d to %d\n",(i*12+(k+4)*12), (i*12+(k+4)*12+3));
            // printf("rep_1_2: (i+(k+4)*12+4) %d to %d\n",(i*12+(k+4)*12+4), (i*12+(k+4)*12+7));
            // printf("rep_1_3: (i+(k+4)*12+8) %d to %d\n",(i*12+(k+4)*12+8), (i*12+(k+4)*12+11));

            printf("winners_0:  %d to %d\n",(f*12+k*12+0), (f*12+k*12+3));
            printf("winners_1:  %d to %d\n",(f*12+k*12+4), (f*12+k*12+7));
            printf("winners_2:  %d to %d\n",(f*12+k*12+8), (f*12+k*12+11));

            // rep_0_1 = _mm256_loadu_pd(&initial_population[i+k]); 
            // rep_0_2 = _mm256_loadu_pd(&initial_population[i+k+4]);
            // rep_0_3 = _mm256_loadu_pd(&initial_population[i+k+8]);

            // rep_1_1 = _mm256_loadu_pd(&initial_population[i+(k+4)*12]);
            // rep_1_2 = _mm256_loadu_pd(&initial_population[i+(k+4)*12+4]);
            // rep_1_3 = _mm256_loadu_pd(&initial_population[i+(k+4)*12+8]);

            // winner_0 = _mm256_blendv_pd(rep_1_1, rep_0_1, compare); 
            // winner_1 = _mm256_blendv_pd(rep_1_2, rep_0_2, compare);
            // winner_2 = _mm256_blendv_pd(rep_1_3, rep_0_3, compare);

            // _mm256_storeu_pd(&winners[(i/2)+k+0], winner_0); // i = 2, k = 0, winners[0] 
            // _mm256_storeu_pd(&winners[(i/2)+k+4], winner_1); //
            // _mm256_storeu_pd(&winners[(i/2)+k+8], winner_2);

        }
        f+=4;
    }
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