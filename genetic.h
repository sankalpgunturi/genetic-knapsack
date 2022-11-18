#include "immintrin.h"

#define NUMBER_OF_ITEMS 12
#define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 256
#define SELECTION_QUOTIENT 4

double *convertColMajor(double *matrix, int rowNum, int colNum)
{
    // [256][12]
    double *res;
    posix_memalign((void *)&res, 64, rowNum * colNum * sizeof(double));

    for (int j = 0; j < colNum; j++)
    {
        for (int i = 0; i < rowNum; i++)
        {
            res[rowNum * j + i] = matrix[i * colNum + j];
        }
    }

    return res;
}

// specially for colNum = 12 (3 SIMD registers)
double *convertColMajorSIMD(double *matrix, int rowNum, int colNum)
{
    double *res;
    posix_memalign((void *)&res, 64, rowNum * colNum * sizeof(double));

    int index = 0;
    for (int j = 0; j < colNum; j++)
    {
        for (int i = 0; i < rowNum; i += 4)
        {
            int offset1 = i * SIZE_OF_INITIAL_POPULATION;
            int offset2 = (i + 1) * SIZE_OF_INITIAL_POPULATION;
            int offset3 = (i + 2) * SIZE_OF_INITIAL_POPULATION;
            int offset4 = (i + 3) * SIZE_OF_INITIAL_POPULATION;
            __m256d ymm0 = _mm256_set_pd(matrix[offset4 + j], matrix[offset3 + j], matrix[offset2 + j], matrix[offset1 + j]);

            _mm256_storeu_pd(&res[index], ymm0);
            index += 4;
        }
    }

    return res;
}

double *fitness(double *weights, double *values, double *representation)
{
    __m256d total_weights, total_values;

    for(int j=0;j<12;j++){
        __m256d weightsj = _mm256_broadcast_sd(&weights[j]);
        __m256d valuesj = _mm256_broadcast_sd(&values[j]);

        for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i += 4){
            __m256d r = _mm256_loadu_pd(&representation[i]);
            total_weights = _mm256_fmadd_pd(r, weightsj, total_weights);
            total_values = _mm256_fmadd_pd(r, valuesj, total_values);
        }
    }
    
    
}

//  TODO : Convert row major to column major
double *fitness(double *weights, double *values_d, double *representation)
{

    double* fitnessArray;
    posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

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
    double const val = 20.0; 
    // MAX_KNAPSACK_WEIGHT;
    int offset = SIZE_OF_INITIAL_POPULATION;

    __m256d total_weights;
    __m256d total_values;
    __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);
    
    __m256d weight_1 = _mm256_broadcast_sd(&weights[0]);
    __m256d values_1 = _mm256_broadcast_sd(&values_d[0]);

    for(int k=0; k < SIZE_OF_INITIAL_POPULATION; k+=4){
    __m256d i = _mm256_loadu_pd(&representation[offset*0]);

    total_weights = _mm256_fmadd_pd(i, weight_1, total_weights);
    total_values = _mm256_fmadd_pd(i, values_1, total_values);

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

    __m256d i_2 = _mm256_loadu_pd(&representation[offset*1]);
    __m256d i_3 = _mm256_loadu_pd(&representation[offset*2]);
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
    total_values = _mm256_and_pd(ret_cmp, total_values);
    return total_values;
}

double *selection(double *weights, double *values_d, double *initial_population)
{
    double *contenders = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * sizeof(double));
    double *winners = (double *)malloc((SIZE_OF_INITIAL_POPULATION / 2) * sizeof(double));
    contenders = fitness(weights, values_d, initial_population);

    for (int i = 0; i < SIZE_OF_INITIAL_POPULATION; i += 40)
    {
        // 10 registers
        __m256 contenders_set_0 = _mm256_loadu_pd(&contenders[i]);
        __m256 contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
        __m256 contenders_set_2 = _mm256_loadu_pd(&contenders[i + 8]);
        __m256 contenders_set_3 = _mm256_loadu_pd(&contenders[i + 12]);
        __m256 contenders_set_4 = _mm256_loadu_pd(&contenders[i + 16]);
        __m256 contenders_set_5 = _mm256_loadu_pd(&contenders[i + 20]);
        __m256 contenders_set_6 = _mm256_loadu_pd(&contenders[i + 24]);
        __m256 contenders_set_7 = _mm256_loadu_pd(&contenders[i + 28]);
        __m256 contenders_set_8 = _mm256_loadu_pd(&contenders[i + 32]);
        __m256 contenders_set_9 = _mm256_loadu_pd(&contenders[i + 36]);

        // 5 registers
        __m256 result_0 = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14);
        __m256 result_1 = _mm256_cmp_pd(contenders_set_2, contenders_set_3, 14);
        __m256 result_2 = _mm256_cmp_pd(contenders_set_4, contenders_set_5, 14);
        __m256 result_3 = _mm256_cmp_pd(contenders_set_6, contenders_set_7, 14);
        __m256 result_4 = _mm256_cmp_pd(contenders_set_8, contenders_set_9, 14);

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

void crossover(double *representation, int popSize, double crossover_rate)
{

    __m256d CROSS_RATE = _mm256_broadcast_sd(&crossover_rate);

    double random0, random1, random2, random3;

    double *cmp;
    posix_memalign((void *)&cmp, 64, 4 * sizeof(double));

    // parents:
    // 0 1 0 1 1 0 | 1 0 0 0 0 0
    // 1 1 1 1 1 1 | 0 0 0 1 1 1

    // children:
    // 0 1 0 1 1 0 | 0 0 0 1 1 1
    // 1 1 1 1 1 1 | 1 0 0 0 0 0

    for (int i = 0; i < popSize; i += 2)
    {

        if (i % 8 == 0)
        {
            random0 = (rand() % (100 - 0)) / 100.00;
            random1 = (rand() % (100 - 0)) / 100.00;
            random2 = (rand() % (100 - 0)) / 100.00;
            random3 = (rand() % (100 - 0)) / 100.00;
            // generate random numbers
            __m256d RANDOM = _mm256_set_pd(random0, random1, random2, random3);
            __m256d compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
            _mm256_storeu_pd(&cmp[0], compare);
        }

        // if rate < cross_rate, 1111, else 0000
        // printf("%f ", cmp[(i/2)%4]);
        if (cmp[(i / 2) % 4] != 0)
        {

            __m256d p_10 = _mm256_loadu_pd(&representation[i * 12 + 0]);
            __m256d p_11 = _mm256_loadu_pd(&representation[i * 12 + 4]);
            __m256d p_12 = _mm256_loadu_pd(&representation[i * 12 + 8]);

            __m256d p_20 = _mm256_loadu_pd(&representation[i * 12 + 12]);
            __m256d p_21 = _mm256_loadu_pd(&representation[i * 12 + 16]);
            __m256d p_22 = _mm256_loadu_pd(&representation[i * 12 + 20]);

            __m256d tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0 | (3 << 4));
            __m256d tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0 | (3 << 4));

            // first child
            _mm256_storeu_pd(&representation[i * 12 + 0], p_10);
            _mm256_storeu_pd(&representation[i * 12 + 4], tmp_1);
            _mm256_storeu_pd(&representation[i * 12 + 8], p_12);

            // second child
            _mm256_storeu_pd(&representation[i * 12 + 12], p_20);
            _mm256_storeu_pd(&representation[i * 12 + 16], tmp_2);
            _mm256_storeu_pd(&representation[i * 12 + 20], p_22);
        }
        else
        {

            // // keep the same
            // __m256d p_10 = _mm256_loadu_pd(&representation[i*12+0]);
            // __m256d p_11 = _mm256_loadu_pd(&representation[i*12+4]);
            // __m256d p_12 = _mm256_loadu_pd(&representation[i*12+8]);

            // __m256d p_20 = _mm256_loadu_pd(&representation[i*12+12]);
            // __m256d p_21 = _mm256_loadu_pd(&representation[i*12+16]);
            // __m256d p_22 = _mm256_loadu_pd(&representation[i*12+20]);

            // // first child
            // _mm256_storeu_pd(&representation[i*12+0], p_10);
            // _mm256_storeu_pd(&representation[i*12+4], p_11);
            // _mm256_storeu_pd(&representation[i*12+8], p_12);

            // // second child
            // _mm256_storeu_pd(&representation[i*12+12], p_20);
            // _mm256_storeu_pd(&representation[i*12+16], p_21);
            // _mm256_storeu_pd(&representation[i*12+20], p_22);
        }
    }
}

double *mutation(double *representation, double MUTATION_RATE)
{
    // Row ordering of representation
    double const mr = MUTATION_RATE;
    __m256d MUTATION_RATE_ = _mm256_broadcast_sd(&mr);

    double random_val0;
    double random_val1;
    double random_val2;
    double random_val3;
    __m256d ONES = _mm256_set_pd(1.0, 1.0, 1.0, 1.0);
    for (int i = 0; i < 256; i += 3)
    {

        random_val0 = (rand() % (100 - 0)) / 100.00;
        random_val1 = (rand() % (100 - 0)) / 100.00;
        random_val2 = (rand() % (100 - 0)) / 100.00;
        random_val3 = (rand() % (100 - 0)) / 100.00;
        __m256d RANDOM = _mm256_set_pd(random_val3, random_val2, random_val1, random_val0);
        __m256d compare = _mm256_cmp_pd(RANDOM, MUTATION_RATE_, 2);
        compare = _mm256_and_pd(compare, ONES);

        __m256d i1_1 = _mm256_loadu_pd(&representation[i + 0]);
        __m256d i1_2 = _mm256_loadu_pd(&representation[i + 4]);
        __m256d i1_3 = _mm256_loadu_pd(&representation[i + 8]);

        __m256d i1_4 = _mm256_loadu_pd(&representation[i + 12]);
        __m256d i1_5 = _mm256_loadu_pd(&representation[i + 16]);
        __m256d i1_6 = _mm256_loadu_pd(&representation[i + 20]);

        __m256d i1_7 = _mm256_loadu_pd(&representation[i + 24]);
        __m256d i1_8 = _mm256_loadu_pd(&representation[i + 28]);
        __m256d i1_9 = _mm256_loadu_pd(&representation[i + 32]);

        i1_1 = _mm256_xor_pd(compare, i1_1);
        i1_2 = _mm256_xor_pd(compare, i1_2);
        i1_3 = _mm256_xor_pd(compare, i1_3);
        i1_4 = _mm256_xor_pd(compare, i1_4);
        i1_5 = _mm256_xor_pd(compare, i1_5);
        i1_6 = _mm256_xor_pd(compare, i1_6);
        i1_7 = _mm256_xor_pd(compare, i1_7);
        i1_8 = _mm256_xor_pd(compare, i1_8);
        i1_9 = _mm256_xor_pd(compare, i1_9);

        // i_1  =  0 0 1 1
        // rand0 = 1.14 1.25 1.75 1.09
        // mutation_rate = 1.15 1.15 1.15 1.15
        // compare =
        // compare = 0xFF 0 0 0xFF
        // 0 0 1 1
        // i_1 = 1 0 1 0

        _mm256_storeu_pd(&representation[i], i1_1);
        _mm256_storeu_pd(&representation[i + 4], i1_2);
        _mm256_storeu_pd(&representation[i + 8], i1_3);
        _mm256_storeu_pd(&representation[i + 12], i1_4);
        _mm256_storeu_pd(&representation[i + 16], i1_5);
        _mm256_storeu_pd(&representation[i + 20], i1_6);
        _mm256_storeu_pd(&representation[i + 24], i1_7);
        _mm256_storeu_pd(&representation[i + 28], i1_8);
        _mm256_storeu_pd(&representation[i + 32], i1_9);
    }
}