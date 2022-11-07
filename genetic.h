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

double *mutation_baseline(double *individuals)
{   
    // Row ordering of individuals
    
    double random_val = 1.5;
    double MUTATION_RATE = 2;
    
    // _mm256_broadcast_sd > 

    for (int i =0; i< 256; i+=2 ) {
        //for (int j = 0; j < 12; j+=4) {
            __m256d i1_1 = _mm256_loadu_pd(&individuals[i+0]);
            __m256d i1_2 = _mm256_loadu_pd(&individuals[i+4]);
            __m256d i1_3 = _mm256_loadu_pd(&individuals[i+8]);

            
            // if (_mm256_cmp_ps()) {
            //    _mm256_xor_epi64();
            // }
            
            _mm256_storeu_pd(&individuals[i+0], i1_1);
            _mm256_storeu_pd(&individuals[i+4], i1_2);
            _mm256_storeu_pd(&individuals[i+8], i1_3);

            i2_1 = _mm256_loadu_pd(&individuals[i+12]);
            i2_2 = _mm256_loadu_pd(&individuals[i+16]);
            i2_3 = _mm256_loadu_pd(&individuals[i+20]);


            // if (_mm256_cmp_ps()) {
            //    _mm256_xor_epi64();
            // }
            

            _mm256_storeu_pd(&individuals[i+12], i1_1);
            _mm256_storeu_pd(&individuals[i+16], i1_2);
            _mm256_storeu_pd(&individuals[i+20], i1_3);
            
            i3_1 = _mm256_loadu_pd(&individuals[i+24]);
            i3_2 = _mm256_loadu_pd(&individuals[i+28]);
            i3_3 = _mm256_loadu_pd(&individuals[i+32]);


            // if (_mm256_cmp_ps()) {
            //    _mm256_xor_epi64();
            // }
            

            _mm256_storeu_pd(&individuals[i+24], i1_1);
            _mm256_storeu_pd(&individuals[i+28], i1_2);
            _mm256_storeu_pd(&individuals[i+32], i1_3);

}