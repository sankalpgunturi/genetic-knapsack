#include "immintrin.h"

// #define NUMBER_OF_ITEMS 12
// #define MAX_KNAPSACK_WEIGHT 15
#define SIZE_OF_INITIAL_POPULATION 512
#define ITEM_SIZE 128


double* convertColMajor(double * matrix, int rowNum, int colNum){
    // [256][12]
    double* res;
    posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    
    for(int j=0; j < colNum; j++){
        for(int i=0;i < rowNum; i++){
            res[rowNum*j+i] = matrix[i*colNum +j];
        }
    }
   
   return res;
}

// specially for colNum = 12 (3 SIMD registers)
double* convertColMajorSIMD(double* matrix, int rowNum, int colNum){
    int SIZE_OF_INITIAL_POPULATION=256;
    double* res;
    posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    
    int index = 0;
    for(int j=0; j < colNum; j++){
        for(int i=0; i < rowNum; i+=4){
            int offset1 = i*SIZE_OF_INITIAL_POPULATION;
            int offset2 = (i+1)*SIZE_OF_INITIAL_POPULATION;
            int offset3 = (i+2)*SIZE_OF_INITIAL_POPULATION;
            int offset4 = (i+3)*SIZE_OF_INITIAL_POPULATION;
            __m256d ymm0 = _mm256_set_pd(matrix[offset4 + j], matrix[offset3 + j], matrix[offset2 + j], matrix[offset1 + j]);
            
            _mm256_storeu_pd(&res[index], ymm0);
            index += 4;
        }
    }
   
   return res;
}

//  TODO : Convert row major to column major
double *fitness(double *weights, double *values_d, double *representation)
{   
    // int SIZE_OF_INITIAL_POPULATION = 256;
    double* fitnessArray = (double *)malloc((SIZE_OF_INITIAL_POPULATION) *sizeof(double));
    posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        fitnessArray[i]=0;
    }

    representation = convertColMajor(representation, SIZE_OF_INITIAL_POPULATION, ITEM_SIZE);
    
    double val = MAX_KNAPSACK_WEIGHT; 
    int offset = SIZE_OF_INITIAL_POPULATION;

    __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);

    __m256d weight_1, weight_2, weight_3, weight_4;

    __m256d values_1, values_2, values_3, values_4;

    __m256d i_1, i_2, i_3, i_4;

    __m256d total_weights, total_values;

    for(int k = 0; k < SIZE_OF_INITIAL_POPULATION; k += 4){

        total_weights = _mm256_set_pd(0, 0, 0, 0);
        total_values = _mm256_set_pd(0, 0, 0, 0);

        for(int id = 0; id < ITEM_SIZE; id += 4){

            i_1 = _mm256_loadu_pd(&representation[(id*offset)+k]);
            i_2 = _mm256_loadu_pd(&representation[(id+1)*offset+k]);
            i_3 = _mm256_loadu_pd(&representation[(id+2)*offset+k]);
            i_4 = _mm256_loadu_pd(&representation[(id+3)*offset+k]);

            // reload values (since too many items, 
            // in order to reuse register, we need to reload)
            weight_1 = _mm256_broadcast_sd(&weights[id]);
            weight_2 = _mm256_broadcast_sd(&weights[id+1]);
            weight_3 = _mm256_broadcast_sd(&weights[id+2]);
            weight_4 = _mm256_broadcast_sd(&weights[id+3]);

            values_1 = _mm256_broadcast_sd(&values_d[id]);
            values_2 = _mm256_broadcast_sd(&values_d[id+1]);
            values_3 = _mm256_broadcast_sd(&values_d[id+2]);
            values_4 = _mm256_broadcast_sd(&values_d[id+3]);

            total_weights = _mm256_fmadd_pd(i_1, weight_1, total_weights);
            total_weights = _mm256_fmadd_pd(i_2, weight_2, total_weights);
            total_weights = _mm256_fmadd_pd(i_3, weight_3, total_weights);
            total_weights = _mm256_fmadd_pd(i_4, weight_4, total_weights);
        
            total_values = _mm256_fmadd_pd(i_1, values_1, total_values);
            total_values = _mm256_fmadd_pd(i_2, values_2, total_values);
            total_values = _mm256_fmadd_pd(i_3, values_3, total_values);
            total_values = _mm256_fmadd_pd(i_4, values_4, total_values);
         }

        //__m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
        // 2 is OP Less Than Equal to
        // TODO
        __m256d ret_cmp = _mm256_cmp_pd(total_weights, max_knapsack_weight, 2);
        // ret_cmp & total_values
        total_values = _mm256_and_pd(ret_cmp, total_values);
        _mm256_storeu_pd(&fitnessArray[k], total_values);
    }
    return fitnessArray;
}

double *selection(double *weights, double *values, double *initial_population)
{   
    int SIZE_OF_INITIAL_POPULATION = 256;
    double *contenders, *winners;
    posix_memalign((void*) &contenders, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double)); 
    posix_memalign((void*) &winners, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

    contenders = fitness(weights, values, initial_population);

    __m256d contenders_set_0 , contenders_set_1; // contenders_set_2, contenders_set_3, contenders_set_4, contenders_set_5;
    __m256d rep_0_1, rep_0_2, rep_0_3, rep_1_1, rep_1_2, rep_1_3;
    __m256d winner_0, winner_1, winner_2;
    __m256d compare;
    int f = 0;
    for (int i = 0; i < 256; i+=4*2)
    {   printf("\ni: %d\n", i);
        contenders_set_0 = _mm256_loadu_pd(&contenders[i]);     
        contenders_set_1 = _mm256_loadu_pd(&contenders[i + 4]);
        compare = _mm256_cmp_pd(contenders_set_0, contenders_set_1, 14); 

        printf("contender_set_0_index: i : %d\n",i);
        printf("contender_set_1_index: i+4: %d\n",i+4);
        printf("winner f %d\n", f);

        printf("i = 0: k = 0; rep_0: 0 to 3, 4 to 7, 8 to 11\n");
        printf("i = 0: k = 0; rep_1: 48 to 51, 52 to 55, 56 to 59\n");
        printf("i = 0: k = 1; rep_0: 12 to 15, 16 to 19, 20 to 23\n");
        printf("i = 0: k = 1; rep_1: 60 to 63, 64 to 67, 68 to 71\n");
        printf("i = 0: k = 2; rep_0: 24 to 27, 28 to 31, 32 t0 35\n");
        printf("i = 0: k = 2; rep_1: 72 to 75, 76 to 79, 80 to 83\n");

        for (int k = 0; k < 4; k++) { 
            //For each winner
            printf("k: %d\n", k);
            printf("rep_0_1: (i+k) %d to %d\n",(i*12+k*12), (i*12+k*12+3));
            printf("rep_0_2: (i+k+4) %d to %d\n",(i*12+k*12+4), (i*12+k*12+7));
            printf("rep_0_3: (i+k+8) %d to %d\n",(i*12+k*12+8), (i*12+k*12+11));

            printf("rep_1_1: (i+(k+4)*12) %d to %d\n",(i*12+(k+4)*12), (i*12+(k+4)*12+3));
            printf("rep_1_2: (i+(k+4)*12+4) %d to %d\n",(i*12+(k+4)*12+4), (i*12+(k+4)*12+7));
            printf("rep_1_3: (i+(k+4)*12+8) %d to %d\n",(i*12+(k+4)*12+8), (i*12+(k+4)*12+11));

            printf("winners_0:  %d to %d\n",(f*12+k*12+0), (f*12+k*12+3));
            printf("winners_1:  %d to %d\n",(f*12+k*12+4), (f*12+k*12+7));
            printf("winners_2:  %d to %d\n",(f*12+k*12+8), (f*12+k*12+11));

            rep_0_1 = _mm256_loadu_pd(&initial_population[(i*12+k*12)]); 
            rep_0_2 = _mm256_loadu_pd(&initial_population[(i*12+k*12+4)]);
            rep_0_3 = _mm256_loadu_pd(&initial_population[(i*12+k*12+8)]);

            rep_1_1 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12)]);
            rep_1_2 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12+4)]);
            rep_1_3 = _mm256_loadu_pd(&initial_population[(i*12+(k+4)*12+8)]);

            winner_0 = _mm256_blendv_pd(rep_1_1, rep_0_1, compare); 
            winner_1 = _mm256_blendv_pd(rep_1_2, rep_0_2, compare);
            winner_2 = _mm256_blendv_pd(rep_1_3, rep_0_3, compare);

            _mm256_storeu_pd(&winners[(f*12+k*12+0)], winner_0);  
            _mm256_storeu_pd(&winners[(f*12+k*12+4)], winner_1); 
            _mm256_storeu_pd(&winners[(f*12+k*12+8)], winner_2);

        }
        f+=4;
    }
    }


void crossover(double* representation, int popSize, double crossover_rate)
{

    __m256d CROSS_RATE =  _mm256_broadcast_sd(&crossover_rate);

    double random0, random1, random2, random3;

    double* cmp;
    posix_memalign((void*) &cmp, 64, 4 * sizeof(double));

// parents:
// 0 1 0 1 1 0 | 1 0 0 0 0 0 
// 1 1 1 1 1 1 | 0 0 0 1 1 1

// children:
// 0 1 0 1 1 0 | 0 0 0 1 1 1 
// 1 1 1 1 1 1 | 1 0 0 0 0 0 

for(int i=0; i<popSize; i+=8){


        random0 = (rand() % (100 - 0))/100.00;
        random1 = (rand() % (100 - 0))/100.00;
        random2 = (rand() % (100 - 0))/100.00;
        random3 = (rand() % (100 - 0))/100.00;
        // generate random numbers
        __m256d RANDOM = _mm256_set_pd(random0, random1, random2, random3);
        __m256d compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
        _mm256_storeu_pd(&cmp[0], compare);

    // if rate < cross_rate, 1111, else 0000
    // printf("%f ", cmp[(i/2)%4]);


    __m256d tmp_1, tmp_2;

    if(cmp[0] != 0){
    __m256d p_11 = _mm256_loadu_pd(&representation[i*12+4]);
    __m256d p_21 = _mm256_loadu_pd(&representation[(i+1)*12+4]);

    tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
    tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[i*12+4], tmp_1);
    // second child
    _mm256_storeu_pd(&representation[(i+1)*12+4], tmp_2);
    }

    if(cmp[1] != 0){
    __m256d p_31 = _mm256_loadu_pd(&representation[(i+2)*12+4]);
    __m256d p_41 = _mm256_loadu_pd(&representation[(i+3)*12+4]);

    tmp_1 = _mm256_permute2f128_pd(p_31, p_41, 0|(3<<4));
    tmp_2 = _mm256_permute2f128_pd(p_41, p_31, 0|(3<<4));
    
    // first child
    _mm256_storeu_pd(&representation[(i+2)*12+4], tmp_1);
    // second child
    _mm256_storeu_pd(&representation[(i+3)*12+4], tmp_2);
    }

    __m256d tmp_3, tmp_4;

    if(cmp[2] != 0){
    __m256d p_51 = _mm256_loadu_pd(&representation[(i+4)*12+4]);
    __m256d p_61 = _mm256_loadu_pd(&representation[(i+5)*12+4]);

    tmp_3 = _mm256_permute2f128_pd(p_51, p_61, 0|(3<<4));
    tmp_4 = _mm256_permute2f128_pd(p_61, p_51, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[(i+4)*12+4], tmp_3);
    // second child
    _mm256_storeu_pd(&representation[(i+5)*12+4], tmp_4);
    }

    if(cmp[3] != 0){
    __m256d p_71 = _mm256_loadu_pd(&representation[(i+6)*12+4]);
    __m256d p_81 = _mm256_loadu_pd(&representation[(i+7)*12+4]);

    tmp_3 = _mm256_permute2f128_pd(p_71, p_81, 0|(3<<4));
    tmp_4 = _mm256_permute2f128_pd(p_81, p_71, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[(i+6)*12+4], tmp_3);
    // second child
    _mm256_storeu_pd(&representation[(i+7)*12+4], tmp_4);
    }
    // }
    }

}

double *mutation(double *representation, double MUTATION_RATE)
{   
    // Row ordering of representation
    double const mr = MUTATION_RATE;
    double *random_vals;
    posix_memalign((void*) &random_vals, 64, 4 * sizeof(double));
    random_vals[0] = 1.0;
    random_vals[1] = 1.0;
    random_vals[2] = 1.0;
    random_vals[3] = 1.0;
    __m256d ONES = _mm256_loadu_pd(&random_vals[0]);
    __m256d RANDOM, compare, i1_1, i1_2, i1_3, i1_4, i1_5, i1_6, i1_7, i1_8, i1_9, i1_10, i1_11, i1_12;
    
    __m256d MUTATION_RATE_ =  _mm256_broadcast_sd(&mr);
    for (int i =0; i< 256; i+=4 ) {

        random_vals[0] = (rand() % (100 - 0))/100.00;
        random_vals[1] = (rand() % (100 - 0))/100.00;
        random_vals[2] = (rand() % (100 - 0))/100.00;
        random_vals[3] = (rand() % (100 - 0))/100.00;

        RANDOM = _mm256_loadu_pd(&random_vals[0]);
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