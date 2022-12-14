#include "immintrin.h"
#include"omp.h"
// #define NUMBER_OF_ITEMS 12
// // #define MAX_KNAPSACK_WEIGHT 15
// #define SIZE_OF_INITIAL_POPULATION 512
// #define ITEM_SIZE 128


double* convertColMajor(double * matrix, int rowNum, int colNum, double *res){
    // // [256][12]
    // double* res;
    // posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    
    for(int j=0; j < colNum; j++){
        for(int i=0;i < rowNum; i++){
            res[rowNum*j+i] = matrix[i*colNum +j];
        }
    }
   
   return res;
}

double* convertColMajorSIMD(double* matrix, int rowNum, int colNum, double* res, int popSize){
    
    int index = 0;
    int offset1, offset2, offset3, offset4;
    for(int j=0; j < colNum; j++){
        for(int i=0; i < rowNum; i+=4){
            offset1 = i*popSize;
            offset2 = (i+1)*popSize;
            offset3 = (i+2)*popSize;
            offset4 = (i+3)*popSize;
            __m256d ymm0 = _mm256_set_pd(matrix[offset4 + j], matrix[offset3 + j], matrix[offset2 + j], matrix[offset1 + j]);
            
            _mm256_storeu_pd(&res[index], ymm0);
            index += 4;
        }
    }
   
   return res;
}

double *fitness(double *weights, double *values_d, double *representation, double* fitnessArray, double* weightsArray, int popSize, int ITEM_SIZE, double MAX_KNAPSACK_WEIGHT)
{   

    double val = MAX_KNAPSACK_WEIGHT; 
    int offset = popSize;

    __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);
    __m256d weight_1, weight_2, weight_3, weight_4;
    __m256d values_1, values_2, values_3, values_4;
    __m256d total_weights1, total_values1;

    #pragma omp parallel for num_threads(4)
    for(int id = 0; id < ITEM_SIZE; id += 4){

        weight_1 = _mm256_broadcast_sd(&weights[id]);
        weight_2 = _mm256_broadcast_sd(&weights[id+1]);
        weight_3 = _mm256_broadcast_sd(&weights[id+2]);
        weight_4 = _mm256_broadcast_sd(&weights[id+3]);

        values_1 = _mm256_broadcast_sd(&values_d[id]);
        values_2 = _mm256_broadcast_sd(&values_d[id+1]);
        values_3 = _mm256_broadcast_sd(&values_d[id+2]);
        values_4 = _mm256_broadcast_sd(&values_d[id+3]);

        for(int k = 0; k < popSize; k += 4){

            __m256d total_weights = _mm256_loadu_pd(&weightsArray[k]);
            __m256d total_values = _mm256_loadu_pd(&fitnessArray[k]);

            __m256d i_1 = _mm256_loadu_pd(&representation[(id*offset)+k]);
            __m256d i_2 = _mm256_loadu_pd(&representation[(id+1)*offset+k]);
            __m256d i_3 = _mm256_loadu_pd(&representation[(id+2)*offset+k]);
            __m256d i_4 = _mm256_loadu_pd(&representation[(id+3)*offset+k]);


            total_weights = _mm256_fmadd_pd(i_1, weight_1, total_weights);
            total_weights = _mm256_fmadd_pd(i_2, weight_2, total_weights);
            total_weights = _mm256_fmadd_pd(i_3, weight_3, total_weights);
            total_weights = _mm256_fmadd_pd(i_4, weight_4, total_weights);
        
            total_values = _mm256_fmadd_pd(i_1, values_1, total_values);
            total_values = _mm256_fmadd_pd(i_2, values_2, total_values);
            total_values = _mm256_fmadd_pd(i_3, values_3, total_values);
            total_values = _mm256_fmadd_pd(i_4, values_4, total_values);

            _mm256_storeu_pd(&fitnessArray[k], total_weights);
            _mm256_storeu_pd(&fitnessArray[k], total_values);
         }
    }

    for(int i=0; i<popSize; i+=4){
        total_weights1 = _mm256_loadu_pd(&weightsArray[i]);
        total_values1 = _mm256_loadu_pd(&fitnessArray[i]);
        __m256d ret_cmp = _mm256_cmp_pd(total_weights1, max_knapsack_weight, 2);
        total_values1 = _mm256_and_pd(ret_cmp, total_values1);
        _mm256_storeu_pd(&fitnessArray[i], total_values1);
    }
    return fitnessArray;
}


double *selection(double *weights, double *values, double *initial_population,double* contenders, double* winners, double *fitnessArray, double *weightsArray, int ITEM_SIZE, int popSize,int MAX_KNAPSACK_WEIGHT)
{   
    __m256d contenders_set_0 , contenders_set_1;
    __m256d rep_0_1, rep_0_2, rep_0_3, rep_1_1, rep_1_2, rep_1_3;
    __m256d winner_0, winner_1, winner_2;
    __m256d compare;
    contenders = fitness(weights, values, initial_population, fitnessArray, weightsArray, popSize, ITEM_SIZE, MAX_KNAPSACK_WEIGHT);
    int f = 0;
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < popSize; i+=4*2) {
        contenders_set_0 = _mm256_loadu_pd(&contenders[i]);     
        contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
        compare = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14); 

        for (int k = 0; k < 4; k++) { 
            rep_0_1 = _mm256_loadu_pd(&initial_population[(i*12+k*12)]); 
            rep_1_1 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12)]);
            
            rep_0_2 = _mm256_loadu_pd(&initial_population[(i*12+k*12+4)]);
            rep_1_2 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12+4)]);

            rep_0_3 = _mm256_loadu_pd(&initial_population[(i*12+k*12+8)]);

            winner_0 = _mm256_blendv_pd(rep_1_1, rep_0_1, compare);
            winner_1 = _mm256_blendv_pd(rep_1_2, rep_0_2, compare);

            rep_1_3 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12+8)]);
            winner_2 = _mm256_blendv_pd(rep_1_3, rep_0_3, compare);

            _mm256_storeu_pd(&winners[(f*12+k*12+0)], winner_0);  
            _mm256_storeu_pd(&winners[(f*12+k*12+0)], winner_0);  
            _mm256_storeu_pd(&winners[(f*12+k*12+4)], winner_1); 
            _mm256_storeu_pd(&winners[(f*12+k*12+8)], winner_2);

        }
        f+=4;
    }
}

void crossover(double* representation, int popSize, double crossover_rate, double *random, double* cmp)
{

    __m256d CROSS_RATE =  _mm256_broadcast_sd(&crossover_rate);
    int j = 0;

    __m256d RANDOM = _mm256_set_pd(random[j], random[j+1], random[j+2], random[j+3]);
    __m256d compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
    _mm256_storeu_pd(&cmp[0], compare);
    __m256d tmp_1, tmp_2, p_11, p_21;
    #pragma omp parallel for num_threads(4)
    for(int i=0; i<popSize; i+=8){

        if(cmp[0] != 0){
            p_11 = _mm256_loadu_pd(&representation[i*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+1)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

            _mm256_storeu_pd(&representation[i*12+4], tmp_1);
            _mm256_storeu_pd(&representation[(i+1)*12+4], tmp_2);
        }

        if(cmp[1] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+2)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+3)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

            _mm256_storeu_pd(&representation[(i+2)*12+4], tmp_1);
            _mm256_storeu_pd(&representation[(i+3)*12+4], tmp_2);
        }

        if(cmp[2] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+4)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+5)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

            _mm256_storeu_pd(&representation[(i+4)*12+4], tmp_1);
            _mm256_storeu_pd(&representation[(i+5)*12+4], tmp_2);
        }

        if(cmp[3] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+6)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+7)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

            _mm256_storeu_pd(&representation[(i+6)*12+4], tmp_1);
            _mm256_storeu_pd(&representation[(i+7)*12+4], tmp_2);
        }
        j+=4;
    }

}

double *mutation(double *representation, double MUTATION_RATE, double *random_vals, int popSize)
{   
    // Row ordering of representation
    double const mr = MUTATION_RATE;
    double const one = 1;
    __m256d ONES = _mm256_broadcast_sd(&one);
    __m256d RANDOM, compare, i1_1, i1_2, i1_3, i1_4, i1_5, i1_6, i1_7, i1_8, i1_9, i1_10, i1_11, i1_12;
    
    __m256d MUTATION_RATE_ =  _mm256_broadcast_sd(&mr);
    #pragma omp parallel for num_threads(4)
    for (int i =0; i< popSize; i+=4 ) {

        RANDOM = _mm256_loadu_pd(&random_vals[i]); 
        compare = _mm256_cmp_pd(RANDOM, MUTATION_RATE_, 2);
        compare = _mm256_and_pd(compare, ONES);
        
        i1_1 = _mm256_loadu_pd(&representation[i+0]);
        i1_2 = _mm256_loadu_pd(&representation[i+4]);
        i1_3 = _mm256_loadu_pd(&representation[i+8]);
        i1_4 = _mm256_loadu_pd(&representation[i+12]);
        i1_5 = _mm256_loadu_pd(&representation[i+16]);
        i1_1 = _mm256_xor_pd(compare, i1_1); 
        i1_6 = _mm256_loadu_pd(&representation[i+20]);
        _mm256_storeu_pd(&representation[i], i1_1);

        i1_7 = _mm256_loadu_pd(&representation[i+24]);
        i1_2 = _mm256_xor_pd(compare, i1_2);
        _mm256_storeu_pd(&representation[i+4], i1_2);

        i1_8 = _mm256_loadu_pd(&representation[i+28]);
        i1_3 = _mm256_xor_pd(compare, i1_3);
        _mm256_storeu_pd(&representation[i+8], i1_3);

        i1_9 = _mm256_loadu_pd(&representation[i+32]);
        i1_4 = _mm256_xor_pd(compare, i1_4); 
        _mm256_storeu_pd(&representation[i+12], i1_4);

        i1_10 = _mm256_loadu_pd(&representation[i+36]);
        i1_5 = _mm256_xor_pd(compare, i1_5);
        _mm256_storeu_pd(&representation[i+16], i1_5);

        i1_11 = _mm256_loadu_pd(&representation[i+40]);
        i1_6 = _mm256_xor_pd(compare, i1_6);
        _mm256_storeu_pd(&representation[i+20], i1_6);

        i1_12 = _mm256_loadu_pd(&representation[i+44]);
        i1_7 = _mm256_xor_pd(compare, i1_7); 
        _mm256_storeu_pd(&representation[i+24], i1_7);

        i1_8 = _mm256_xor_pd(compare, i1_8);
        _mm256_storeu_pd(&representation[i+28], i1_8);

        i1_9 = _mm256_xor_pd(compare, i1_9);
        _mm256_storeu_pd(&representation[i+32], i1_9);

        i1_10 = _mm256_xor_pd(compare, i1_10);
        _mm256_storeu_pd(&representation[i+36], i1_10);

        i1_11 = _mm256_xor_pd(compare, i1_11);
        _mm256_storeu_pd(&representation[i+40], i1_11);

        i1_12 = _mm256_xor_pd(compare, i1_12);
        _mm256_storeu_pd(&representation[i+44], i1_12);


    }
}

//FOLLOWING IS INCOMPLETE
void crossover_and_mutation(double* representation, int popSize, double crossover_rate, double *random, double *cmp, double MUTATION_RATE)
{
    double const mr = MUTATION_RATE;
    double const one = 1;
    __m256d CROSS_RATE =  _mm256_broadcast_sd(&crossover_rate);
    __m256d MUTATION_RATE_ = _mm256_broadcast_sd(&mr);
    __m256d ONES = _mm256_broadcast_sd(&one);
    __m256d RANDOM, compare, compare_mutation, tmp_1, tmp_2, p_11, p_21; 
    for(int i=0; i<popSize; i+=4){

        RANDOM = _mm256_loadu_pd(&random[i]); 

        compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
        _mm256_storeu_pd(&cmp[0], compare);

        compare_mutation = _mm256_cmp_pd(RANDOM, MUTATION_RATE_, 2);
        compare_mutation = _mm256_and_pd(compare, ONES);
        _mm256_storeu_pd(&cmp[0], compare);

        if(cmp[0] != 0){
            p_11 = _mm256_loadu_pd(&representation[i*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+1)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4)); //crossover
            tmp_1 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation

            // first child
            _mm256_storeu_pd(&representation[i*12+4], tmp_1);
            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));
            tmp_2 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // second child
            _mm256_storeu_pd(&representation[(i+1)*12+4], tmp_2);
            
        }

        if(cmp[1] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+2)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+3)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4)); //crossover
            tmp_1 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // first child
            _mm256_storeu_pd(&representation[(i+2)*12+4], tmp_1);

            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));
            tmp_2 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // second child
            _mm256_storeu_pd(&representation[(i+3)*12+4], tmp_2);
        }

        if(cmp[2] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+4)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+5)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4)); //crossover
            tmp_1 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // first child
            _mm256_storeu_pd(&representation[(i+4)*12+4], tmp_1);

            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));
            tmp_2 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // second child
            _mm256_storeu_pd(&representation[(i+5)*12+4], tmp_2);
        }

        if(cmp[3] != 0){
            p_11 = _mm256_loadu_pd(&representation[(i+6)*12+4]);
            p_21 = _mm256_loadu_pd(&representation[(i+7)*12+4]);

            tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4)); //crossover
            tmp_1 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // first child
            _mm256_storeu_pd(&representation[(i+6)*12+4], tmp_1);

            tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));
            tmp_2 = _mm256_xor_pd(compare_mutation, tmp_1);  //mutation
            // second child
            _mm256_storeu_pd(&representation[(i+7)*12+4], tmp_2);
        }
    // }
    }


}