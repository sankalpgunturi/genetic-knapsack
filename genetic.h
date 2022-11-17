#include "immintrin.h"

// #define NUMBER_OF_ITEMS 12
// #define MAX_KNAPSACK_WEIGHT 15
// #define SIZE_OF_INITIAL_POPULATION 256

//  TODO : Convert row major to column major
__m256d fitness(double *weights, double *values_d, double *representation)
{   
    // representation X representation:
    // COLUMN major order
    // 12 bits for 1 individual. Eg: [0, 0, 1, 1, ...]
    // 12 bits for 2nd individual
    // ...
    // 
    // Currently X = 4 .
    // Algorithm
    // Broadcast weights
    // __m256d _mm256_broadcast_sd (double const * mem_addr)
    double const val = 15.0;
    __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);
    __m256d weight = _mm256_broadcast_sd(&weights[0]);
    __m256d values = _mm256_broadcast_sd(&values_d[0]);
    __m256d i = _mm256_loadu_pd(&representation[0]); 
    __m256d total_weights = _mm256_fmadd_pd(i, weight, total_weights);
    __m256d total_values = _mm256_fmadd_pd(i, values, total_values);

    __m256d weight_2 = _mm256_broadcast_sd(&weights[1]);
    __m256d weight_3 = _mm256_broadcast_sd(&weights[2]);
    __m256d weight_4 = _mm256_broadcast_sd(&weights[3]);
    __m256d weight_5 = _mm256_broadcast_sd(&weights[4]);
    __m256d weight_6 = _mm256_broadcast_sd(&weights[5]);
    __m256d weight_7 = _mm256_broadcast_sd(&weights[6]);
    __m256d weight_8 = _mm256_broadcast_sd(&weights[7]);
    __m256d weight_9 = _mm256_broadcast_sd(&weights[8]);
    __m256d weight_10 = _mm256_broadcast_sd(&weights[9]);
    __m256d weight_11 = _mm256_broadcast_sd(&weights[10]);
    __m256d weight_12 = _mm256_broadcast_sd(&weights[11]);
    
    // Broadcast values

    __m256d values_2 = _mm256_broadcast_sd(&values_d[1]);
    __m256d values_3 = _mm256_broadcast_sd(&values_d[2]);
    __m256d values_4 = _mm256_broadcast_sd(&values_d[3]);
    __m256d values_5 = _mm256_broadcast_sd(&values_d[4]);
    __m256d values_6 = _mm256_broadcast_sd(&values_d[5]);
    __m256d values_7 = _mm256_broadcast_sd(&values_d[6]);
    __m256d values_8 = _mm256_broadcast_sd(&values_d[7]);
    __m256d values_9 = _mm256_broadcast_sd(&values_d[8]);
    __m256d values_10 = _mm256_broadcast_sd(&values_d[9]);
    __m256d values_11 = _mm256_broadcast_sd(&values_d[10]);
    __m256d values_12 = _mm256_broadcast_sd(&values_d[11]);
    
    // // Broadcast MAX_KNAPSACK_WEIGHT

    // // One Independent Operation - One ouptut.
    
    // // 12 different FMAs to give 4 different outputs
    // // __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
    // // Each fma is loading the corresponding individual bit at i and the broadcasted weight at i
    // // 12 such fmas to get 4 outputs
    // // i[0][0], i[1][0], i[2][0], i[3][0]* fma(weight[0])
    __m256d i_2 = _mm256_loadu_pd(&representation[4]); 
    __m256d i_3 = _mm256_loadu_pd(&representation[8]); 
    __m256d i_4 = _mm256_loadu_pd(&representation[16]); 
    __m256d i_5 = _mm256_loadu_pd(&representation[20]); 
    __m256d i_6 = _mm256_loadu_pd(&representation[24]); 
    __m256d i_7 = _mm256_loadu_pd(&representation[28]); 
    __m256d i_8 = _mm256_loadu_pd(&representation[32]); 
    __m256d i_9 = _mm256_loadu_pd(&representation[36]); 
    __m256d i_10 = _mm256_loadu_pd(&representation[40]); 
    __m256d i_11 = _mm256_loadu_pd(&representation[44]); 
    __m256d i_12 = _mm256_loadu_pd(&representation[48]); 

    
    total_weights = _mm256_fmadd_pd(i_2, weight_2, total_weights);
    total_weights = _mm256_fmadd_pd(i_3, weight_3, total_weights);
    total_weights = _mm256_fmadd_pd(i_4, weight_4, total_weights);
    total_weights = _mm256_fmadd_pd(i_5, weight_5, total_weights);
    total_weights = _mm256_fmadd_pd(i_6, weight_6, total_weights);
    total_weights = _mm256_fmadd_pd(i_7, weight_7, total_weights);
    total_weights = _mm256_fmadd_pd(i_8, weight_8, total_weights);
    total_weights = _mm256_fmadd_pd(i_9, weight_9, total_weights);
    total_weights = _mm256_fmadd_pd(i_10, weight_10, total_weights);
    total_weights = _mm256_fmadd_pd(i_11, weight_11, total_weights);
    total_weights = _mm256_fmadd_pd(i_12, weight_12, total_weights);


    total_values = _mm256_fmadd_pd(i_2, values_2, total_values);
    total_values = _mm256_fmadd_pd(i_3, values_3, total_values);
    total_values = _mm256_fmadd_pd(i_4, values_4, total_values);
    total_values = _mm256_fmadd_pd(i_5, values_5, total_values);
    total_values = _mm256_fmadd_pd(i_6, values_6, total_values);
    total_values = _mm256_fmadd_pd(i_7, values_7, total_values);
    total_values = _mm256_fmadd_pd(i_8, values_8, total_values);
    total_values = _mm256_fmadd_pd(i_9, values_9, total_values);
    total_values = _mm256_fmadd_pd(i_10, values_10, total_values);
    total_values = _mm256_fmadd_pd(i_11, values_11, total_values);
    total_values = _mm256_fmadd_pd(i_12, values_12, total_values);
    
    //__m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
    // 2 is OP Less Than Equal to
    __m256d ret_cmp = _mm256_cmp_pd(total_weights, max_knapsack_weight, 2); 
    // ret_cmp & total_values
    total_values = _mm256_and_pd (ret_cmp, total_values);
    return total_values;

}


// double *selection(double *weights, double *values, double *initial_population)
// {
//     double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));
//     __m256d result;
//     for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i += 2)
//     {   
//         result = _mm256_cmp_pd(fitness(&weights, &values, &initial_population[i]), fitness(&weights, &values, &initial_population[i + 1]), winnersSIMD);
//         if (result == 1)
//         {
//             winners[i / 2] = initial_population[i];
//         }
//         else
//         {
//             winners[i / 2] = initial_population[i + 1];
//         }
//     }

//     return winners;
// }

// void crossover()
// {
// // }
// void print(const __m256d v)
// {
//     printf(_mm256_extractf128_pd(v, 0));
//     printf(_mm256_extractf128_pd(v, 1));
//     printf(_mm256_extractf128_pd(v, 2));
//     printf(_mm256_extractf128_pd(v, 3));
// }

double *mutation(double *representation, double MUTATION_RATE)
{   
    // Row ordering of representation
//   for individual in individuals:
//         for i in range(len(individual.bits)):
//             if random.random() < MUTATION_RATE:
//                 # Flip the bit
//                 individual.bits[i] = ~individual.bits[i]
    double const mr = MUTATION_RATE;
    const int le_op = 1;
    __m256d MUTATION_RATE_ =  _mm256_broadcast_sd(&mr) ;

    double random_val0;
    double random_val1;
    double random_val2;
    double random_val3;

    for (int i =0; i< 256; i+=2 ) {
        //for (int j = 0; j < 12; j+=4) {
            
            random_val0 = (rand() % (100 - 0))/100.00;
            random_val1 = (rand() % (100 - 0))/100.00;
            random_val2 = (rand() % (100 - 0))/100.00;
            random_val3 = (rand() % (100 - 0))/100.00;
            printf("\n RANDOM:");
            printf(" %.2f  %.2f  %.2f  %.2f \n", random_val0, random_val1, random_val2, random_val3);

            __m256d RANDOM = _mm256_set_pd(random_val3, random_val2, random_val1, random_val0);
            __m256d i1_1 = _mm256_loadu_pd(&representation[i+0]);
            __m256d i1_2 = _mm256_loadu_pd(&representation[i+4]);
            __m256d i1_3 = _mm256_loadu_pd(&representation[i+8]);

            // printf(" %.2f  %.2f  %.2f  %.2f ",representation[i], representation[i+1], representation[i+2], representation[i+3]);
            // printf(" %.2f  %.2f  %.2f  %.2f ",representation[i+4], representation[i+5], representation[i+6], representation[i+7]);
            // printf(" %.2f  %.2f  %.2f  %.2f ",representation[i+8], representation[i+9], representation[i+10], representation[i+11]);

            __m256d compare = _mm256_cmp_pd(RANDOM, MUTATION_RATE_, 14);
            
            _mm256_storeu_pd(&representation[i+0], RANDOM);

            _mm256_storeu_pd(&representation[i+4], MUTATION_RATE_);

            _mm256_storeu_pd(&representation[i+8], compare);
            //printf(" %.2f  %.2f  %.2f  %.2f ",compare[i], representation[i+1], representation[i+2], representation[i+3]);
            // printf(" COMPARE : %d ",i);
            // print(compare);
            i1_1 = _mm256_xor_pd(compare, i1_1); 
            i1_2 = _mm256_xor_pd(compare, i1_2);
            i1_3 = _mm256_xor_pd(compare, i1_3);
            
            // i_1  =  0 0 1 1
            // rand0 = 1.14 1.25 1.75 1.09
            // mutation_rate = 1.15 1.15 1.15 1.15
            // compare = 0xFF 0 0 0xFF 
            // 0 0 1 1
            // i_1 = 1 0 1 0
            
            // _mm256_storeu_pd(&representation[i+0], i1_1);
            // _mm256_storeu_pd(&representation[i+4], i1_2);
            // _mm256_storeu_pd(&representation[i+8], i1_3);
            
            // _mm256_storeu_pd(&representation[i+4], i1_1);
            // _mm256_storeu_pd(&representation[i+8], i1_2);
            //_mm256_storeu_pd(&representation[i+8], i1_1);

             
            break;
            // i2_1 = _mm256_loadu_pd(&representation[i+12]);
            // i2_2 = _mm256_loadu_pd(&representation[i+16]);
            // i2_3 = _mm256_loadu_pd(&representation[i+20]);


            // // if (_mm256_cmp_ps()) {
            // //    _mm256_xor_epi64();
            // // }
            

            // _mm256_storeu_pd(&representation[i+12], i1_1);
            // _mm256_storeu_pd(&representation[i+16], i1_2);
            // _mm256_storeu_pd(&representation[i+20], i1_3);
            
            // i3_1 = _mm256_loadu_pd(&representation[i+24]);
            // i3_2 = _mm256_loadu_pd(&representation[i+28]);
            // i3_3 = _mm256_loadu_pd(&representation[i+32]);


            // // if (_mm256_cmp_ps()) {
            // //    _mm256_xor_epi64();
            // // }
            

            // _mm256_storeu_pd(&representation[i+24], i1_1);
            // _mm256_storeu_pd(&representation[i+28], i1_2);
            // _mm256_storeu_pd(&representation[i+32], i1_3);

}
}