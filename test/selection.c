#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <immintrin.h>
#include <time.h>
#include <math.h>
#include"omp.h"
#define ITEM_SIZE 32768
#define SIZE_OF_INITIAL_POPULATION 32768
#define MAX_KNAPSACK_WEIGHT 50.0


//timing routine for reading the time stamp counter
static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


double* convertColMajor(double * matrix, int rowNum, int colNum){
// [256][12]
    double* res = (double *)malloc((rowNum * colNum) *sizeof(double));;
    posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    #pragma omp parallel for num_threads(20)
    for(int j=0; j < colNum; j++){
        for(int i=0; i < rowNum; i++){
            res[rowNum*j+i] = matrix[i*colNum +j];
        }
    }
   
   return res;
}

double* convertColMajorSIMD(double* matrix, int rowNum, int colNum){
    double* res = (double *)malloc((rowNum * colNum) *sizeof(double));;
    posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    // int index = 0;
    #pragma omp parallel for num_threads(8)
    for(int j=0; j < colNum; j++){
        #pragma omp parallel for num_threads(4)
        for(int i=0; i < rowNum; i+=4*8){
            int index = 0;
            __m256d ymm0 = _mm256_set_pd(matrix[(i+3)*colNum+j], matrix[(i+2)*colNum + j], matrix[(i+1)*colNum + j], matrix[i*colNum + j]);        
            __m256d ymm1 = _mm256_set_pd(matrix[(i+7)*colNum + j], matrix[(i+6)*colNum + j], matrix[(i+5)*colNum + j], matrix[(i+4)*colNum + j]);        
            __m256d ymm2 = _mm256_set_pd(matrix[(i+11)*colNum + j], matrix[(i+10)*colNum + j], matrix[(i+9)*colNum + j], matrix[(i+8)*colNum + j]);        
            __m256d ymm3 = _mm256_set_pd(matrix[(i+15)*colNum + j], matrix[(i+14)*colNum + j], matrix[(i+13)*colNum + j], matrix[(i+12)*colNum + j]);        
            __m256d ymm4 = _mm256_set_pd(matrix[(i+19)*colNum + j], matrix[(i+18)*colNum + j], matrix[(i+17)*colNum + j], matrix[(i+16)*colNum + j]);        
            __m256d ymm5 = _mm256_set_pd(matrix[(i+23)*colNum + j], matrix[(i+22)*colNum + j], matrix[(i+21)*colNum + j], matrix[(i+20)*colNum + j]);        
            __m256d ymm6 = _mm256_set_pd(matrix[(i+27)*colNum + j], matrix[(i+26)*colNum + j], matrix[(i+25)*colNum + j], matrix[(i+24)*colNum + j]);        
            __m256d ymm7 = _mm256_set_pd(matrix[(i+31)*colNum + j], matrix[(i+30)*colNum + j], matrix[(i+29)*colNum + j], matrix[(i+28)*colNum + j]);        

            _mm256_storeu_pd(&res[index], ymm0);
            _mm256_storeu_pd(&res[index+4], ymm1);
            _mm256_storeu_pd(&res[index+8], ymm2);
            _mm256_storeu_pd(&res[index+12], ymm3);
            _mm256_storeu_pd(&res[index+16], ymm4);    
            _mm256_storeu_pd(&res[index+20], ymm5);    
            _mm256_storeu_pd(&res[index+24], ymm6); 
            _mm256_storeu_pd(&res[index+28], ymm7);
            
            index += 4*8;
        }
    }
    return res;
}


void printSIMD(__m256d reg){
    double* i_12s = (double *)malloc(4 *sizeof(double));
    printf("\n");
    _mm256_storeu_pd(&i_12s[0], reg);
        printf(" ------------  \n");
       for(int o=0;o<4;o++)
       {
        printf("%lf ", i_12s[o]);
       }
       printf("\n ------------  \n");
}

double *fitness(double *weights, double *values_d, double *representation, double* fitnessArray, double* weightsArray, int popSize, int item_size)
{   

    double val = MAX_KNAPSACK_WEIGHT; 
    int offset = popSize;

    // put it outside of fitness
    
    // representation = convertColMajor(representation, popSize, item_size);
    // representation = convertColMajorSIMD(representation, popSize, item_size);

    #pragma omp parallel for num_threads(20)
    for(int id = 0; id < item_size; id += 4){

        __m256d weight_1 = _mm256_broadcast_sd(&weights[id]);
        __m256d weight_2 = _mm256_broadcast_sd(&weights[id+1]);
        __m256d weight_3 = _mm256_broadcast_sd(&weights[id+2]);
        __m256d weight_4 = _mm256_broadcast_sd(&weights[id+3]);

        __m256d values_1 = _mm256_broadcast_sd(&values_d[id]);
        __m256d values_2 = _mm256_broadcast_sd(&values_d[id+1]);
        __m256d values_3 = _mm256_broadcast_sd(&values_d[id+2]);
        __m256d values_4 = _mm256_broadcast_sd(&values_d[id+3]);

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

    #pragma omp parallel for num_threads(20)
    for(int i=0; i<popSize; i+=4){
        __m256d tmp = _mm256_broadcast_sd(&val);
        __m256d total = _mm256_loadu_pd(&weightsArray[i]);
        tmp = _mm256_cmp_pd(total, tmp, 2);
        total = _mm256_loadu_pd(&fitnessArray[i]);
        total = _mm256_and_pd(tmp, total);
        _mm256_storeu_pd(&fitnessArray[i], total);
    }

    return fitnessArray;
}

// //  TODO : Convert row major to column major
// double *fitness_org(double *weights, double *values_d, double *representation)
// {

//     double* fitnessArray;
//     posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

//     // representation X representation:
//     // COLUMN major order
//     // 12 bits for 1 individual. Eg: [0, 0, 1, 1, ...]
//     // 12 bits for 2nd individual
//     // ...
//     //
//     // Currently X = 4 .
//     // Algorithm
//     // Broadcast weights
//     // __m256d _mm256_broadcast_sd (double const * mem_addr)
//     double val = MAX_KNAPSACK_WEIGHT; 
//     // MAX_KNAPSACK_WEIGHT;
//     int offset = SIZE_OF_INITIAL_POPULATION;


//     __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);
    
//     __m256d weight_1 = _mm256_broadcast_sd(&weights[0]);
//     __m256d weight_2 = _mm256_broadcast_sd(&weights[1]);
//     __m256d weight_3 = _mm256_broadcast_sd(&weights[2]);
//     __m256d weight_4 = _mm256_broadcast_sd(&weights[3]);
//     __m256d weight_5 = _mm256_broadcast_sd(&weights[4]);
//     __m256d weight_6 = _mm256_broadcast_sd(&weights[5]);
//     __m256d weight_7 = _mm256_broadcast_sd(&weights[6]);
//     __m256d weight_8 = _mm256_broadcast_sd(&weights[7]);
//     __m256d weight_9 = _mm256_broadcast_sd(&weights[8]);
//     __m256d weight_10 = _mm256_broadcast_sd(&weights[9]);
//     __m256d weight_11 = _mm256_broadcast_sd(&weights[10]);
//     __m256d weight_12 = _mm256_broadcast_sd(&weights[11]);
    
//     // Broadcast values
//     __m256d values_1 = _mm256_broadcast_sd(&values_d[0]);
//     __m256d values_2 = _mm256_broadcast_sd(&values_d[1]);
//     __m256d values_3 = _mm256_broadcast_sd(&values_d[2]);
//     __m256d values_4 = _mm256_broadcast_sd(&values_d[3]);
//     __m256d values_5 = _mm256_broadcast_sd(&values_d[4]);
//     __m256d values_6 = _mm256_broadcast_sd(&values_d[5]);
//     __m256d values_7 = _mm256_broadcast_sd(&values_d[6]);
//     __m256d values_8 = _mm256_broadcast_sd(&values_d[7]);
//     __m256d values_9 = _mm256_broadcast_sd(&values_d[8]);
//     __m256d values_10 = _mm256_broadcast_sd(&values_d[9]);
//     __m256d values_11 = _mm256_broadcast_sd(&values_d[10]);
//     __m256d values_12 = _mm256_broadcast_sd(&values_d[11]);

//     for(int k=0; k < SIZE_OF_INITIAL_POPULATION; k+=4){

//         __m256d total_weights = {0, 0, 0, 0};
//         __m256d total_values = {0, 0, 0, 0};
        
//         // // Broadcast MAX_KNAPSACK_WEIGHT

//         // // One Independent Operation - One ouptut.

//         // // 12 different FMAs to give 4 different outputs
//         // // __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
//         // // Each fma is loading the corresponding individual bit at i and the broadcasted weight at i
//         // // 12 such fmas to get 4 outputs
//         // // i[0][0], i[1][0], i[2][0], i[3][0]* fma(weight[0])
//         __m256d i_1 = _mm256_loadu_pd(&representation[offset*0 + k]);
//         __m256d i_2 = _mm256_loadu_pd(&representation[offset*1 + k]);
//         __m256d i_3 = _mm256_loadu_pd(&representation[offset*2 + k]);
//         __m256d i_4 = _mm256_loadu_pd(&representation[offset*3 + k]);
//         __m256d i_5 = _mm256_loadu_pd(&representation[offset*4 + k]);
//         __m256d i_6 = _mm256_loadu_pd(&representation[offset*5 + k]);
//         __m256d i_7 = _mm256_loadu_pd(&representation[offset*6 + k]);
//         __m256d i_8 = _mm256_loadu_pd(&representation[offset*7 + k]);
//         __m256d i_9 = _mm256_loadu_pd(&representation[offset*8 + k]);
//         __m256d i_10 = _mm256_loadu_pd(&representation[offset*9 + k]);
//         __m256d i_11 = _mm256_loadu_pd(&representation[offset*10 + k]);
//         __m256d i_12 = _mm256_loadu_pd(&representation[offset*11 + k]);

        
//         total_weights = _mm256_fmadd_pd(i_1, weight_1, total_weights);
//         total_weights = _mm256_fmadd_pd(i_2, weight_2, total_weights);
//         total_weights = _mm256_fmadd_pd(i_3, weight_3, total_weights);
//         total_weights = _mm256_fmadd_pd(i_4, weight_4, total_weights);
//         total_weights = _mm256_fmadd_pd(i_5, weight_5, total_weights);
//         total_weights = _mm256_fmadd_pd(i_6, weight_6, total_weights);
//         total_weights = _mm256_fmadd_pd(i_7, weight_7, total_weights);
//         total_weights = _mm256_fmadd_pd(i_8, weight_8, total_weights);
//         total_weights = _mm256_fmadd_pd(i_9, weight_9, total_weights);
//         total_weights = _mm256_fmadd_pd(i_10, weight_10, total_weights);
//         total_weights = _mm256_fmadd_pd(i_11, weight_11, total_weights);
//         total_weights = _mm256_fmadd_pd(i_12, weight_12, total_weights);


//         total_values = _mm256_fmadd_pd(i_1, values_1, total_values);
//         total_values = _mm256_fmadd_pd(i_2, values_2, total_values);
//         total_values = _mm256_fmadd_pd(i_3, values_3, total_values);
//         total_values = _mm256_fmadd_pd(i_4, values_4, total_values);
//         total_values = _mm256_fmadd_pd(i_5, values_5, total_values);
//         total_values = _mm256_fmadd_pd(i_6, values_6, total_values);
//         total_values = _mm256_fmadd_pd(i_7, values_7, total_values);
//         total_values = _mm256_fmadd_pd(i_8, values_8, total_values);
//         total_values = _mm256_fmadd_pd(i_9, values_9, total_values);
//         total_values = _mm256_fmadd_pd(i_10, values_10, total_values);
//         total_values = _mm256_fmadd_pd(i_11, values_11, total_values);
//         total_values = _mm256_fmadd_pd(i_12, values_12, total_values);


//         //__m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
//         // 2 is OP Less Than Equal to
//         // TODO
//         __m256d ret_cmp = _mm256_cmp_pd(total_weights, max_knapsack_weight, 2);
//         // ret_cmp & total_values
//         //printSIMD(total_weights);
//         // printSIMD(ret_cmp);
//         //total_values = _mm256_and_pd(ret_cmp, total_values);
//         //printSIMD(total_weights);
//         _mm256_storeu_pd(&fitnessArray[k], total_values);
//     }
//     return fitnessArray;
// }

void print(double* matrix, int rowNum, int colNum){
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            printf("%lf ", matrix[colNum*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int check(double * ans, double* res, int n){
    for(int i=0;i<n;i++){
        if(abs(ans[i] - res[i])>0.1){
            return 0;
        }
    }
    return 1;
}

double t0, t1;

int main(){
    srand((unsigned) time(NULL));
    double weights[ITEM_SIZE+5];
    double values[ITEM_SIZE+5];
    // double * weights = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * 12 *sizeof(double));
    for(int i=0;i<ITEM_SIZE;i++){
        weights[i] = rand()%10;
        values[i] = rand()%10;
    }
    // double weights[12] = { 8, 5, 1, 0, 4, 4, 9, 10, 15, 3, 12, 10 };
    // double values[12] = { 1, 10, 5, 15, 9, 11, 0, 13, 13, 8, 4, 2 };
    double *pop = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * ITEM_SIZE *sizeof(double));
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        for(int j=0;j<ITEM_SIZE;j++){
            pop[i*ITEM_SIZE + j] = (int)rand()%2;
        }
    }

    double* fitnessAns =  (double *)malloc((SIZE_OF_INITIAL_POPULATION) *sizeof(double));
    t0 = omp_get_wtime();
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        fitnessAns[i]=0;
        for(int j=0;j<ITEM_SIZE;j++){
            fitnessAns[i] += pop[i*ITEM_SIZE + j] * values[j];
        }
        if(fitnessAns[i] > MAX_KNAPSACK_WEIGHT){
            fitnessAns[i]=0;
        }
    }
    t1 = omp_get_wtime();
    printf(" 1  --------- %lf \n", t1-t0);
    // print(pop, SIZE_OF_INITIAL_POPULATION, 12);
    // pop = convertColMajor(pop, SIZE_OF_INITIAL_POPULATION, 12);
    // print(pop, SIZE_OF_INITIAL_POPULATION, 12);

    double* fitnessArray = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * ITEM_SIZE *sizeof(double));;
    posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

    double* weightsArray = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * ITEM_SIZE *sizeof(double));;
    posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

    // rdtsc()
    t0 = omp_get_wtime(); // seconds
    fitnessArray = fitness(weights, values, pop, fitnessArray, weightsArray, SIZE_OF_INITIAL_POPULATION, ITEM_SIZE);
    t1 = omp_get_wtime();
    printf(" 2  --------- %lf \n", t1-t0);

    // for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
    //     printf("%lf ", fitnessAns[i]);
    // }
    // printf("\n\n");
    
    // for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
    //     printf("%lf ", fitnessArray[i]);
    // }
    // printf("\n");
    // if(check(fitnessArray, fitnessAns, SIZE_OF_INITIAL_POPULATION) != 1){
    //     printf("GGGGGGGGGGGGGG\n");
    // }
    // fitnessArray =  fitness(weights, values, pop, fitnessArray, weightsArray, SIZE_OF_INITIAL_POPULATION, ITEM_SIZE);
    //     if(check(fitnessArray, fitnessAns, SIZE_OF_INITIAL_POPULATION) != 1){
    //     printf("GGGGGGGGGGGGGG");
    // }
    return 0;
}