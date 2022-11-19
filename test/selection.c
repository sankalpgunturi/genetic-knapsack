#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <immintrin.h>
#include<time.h>
#define SIZE_OF_INITIAL_POPULATION 12
#define MAX_KNAPSACK_WEIGHT 50.0

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
    double val = MAX_KNAPSACK_WEIGHT; 
    // MAX_KNAPSACK_WEIGHT;
    int offset = SIZE_OF_INITIAL_POPULATION;


    __m256d max_knapsack_weight = _mm256_broadcast_sd(&val);
    
    __m256d weight_1 = _mm256_broadcast_sd(&weights[0]);
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
    __m256d values_1 = _mm256_broadcast_sd(&values_d[0]);
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

    for(int k=0; k < SIZE_OF_INITIAL_POPULATION; k+=4){

    __m256d total_weights = {0, 0, 0, 0};
    __m256d total_values = {0, 0, 0, 0};
    

    // // Broadcast MAX_KNAPSACK_WEIGHT

    // // One Independent Operation - One ouptut.

    // // 12 different FMAs to give 4 different outputs
    // // __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
    // // Each fma is loading the corresponding individual bit at i and the broadcasted weight at i
    // // 12 such fmas to get 4 outputs
    // // i[0][0], i[1][0], i[2][0], i[3][0]* fma(weight[0])
    __m256d i_1 = _mm256_loadu_pd(&representation[offset*0 + k]);
    __m256d i_2 = _mm256_loadu_pd(&representation[offset*1 + k]);
    __m256d i_3 = _mm256_loadu_pd(&representation[offset*2 + k]);
    __m256d i_4 = _mm256_loadu_pd(&representation[offset*3 + k]);
    __m256d i_5 = _mm256_loadu_pd(&representation[offset*4 + k]);
    __m256d i_6 = _mm256_loadu_pd(&representation[offset*5 + k]);
    __m256d i_7 = _mm256_loadu_pd(&representation[offset*6 + k]);
    __m256d i_8 = _mm256_loadu_pd(&representation[offset*7 + k]);
    __m256d i_9 = _mm256_loadu_pd(&representation[offset*8 + k]);
    __m256d i_10 = _mm256_loadu_pd(&representation[offset*9 + k]);
    __m256d i_11 = _mm256_loadu_pd(&representation[offset*10 + k]);
    __m256d i_12 = _mm256_loadu_pd(&representation[offset*11 + k]);

    
    total_weights = _mm256_fmadd_pd(i_1, weight_1, total_weights);
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


    total_values = _mm256_fmadd_pd(i_1, values_1, total_values);
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
    // TODO
    __m256d ret_cmp = _mm256_cmp_pd(total_weights, max_knapsack_weight, 2);
    // ret_cmp & total_values
    printSIMD(total_weights);
    // printSIMD(ret_cmp);
    total_values = _mm256_and_pd(ret_cmp, total_values);
    printSIMD(total_weights);
    _mm256_storeu_pd(&fitnessArray[k], total_values);
    }
    return fitnessArray;
}

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

int main(){
    srand((unsigned) time(NULL));
    double weights[12] = { 8, 5, 1, 0, 4, 4, 9, 10, 15, 3, 12, 10 };
    double values[12] = { 1, 10, 5, 15, 9, 11, 0, 13, 13, 8, 4, 2 };
    double *pop = (double *)malloc((SIZE_OF_INITIAL_POPULATION) * 12 *sizeof(double));
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        for(int j=0;j<12;j++){
            pop[i*12 + j] = (int)rand()%2;
        }
    }



    
    double* fitnessAns =  (double *)malloc((SIZE_OF_INITIAL_POPULATION) *sizeof(double));;
    
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        for(int j=0;j<12;j++){
            fitnessAns[i] += pop[i*12 + j] * values[j];
        }
        if(fitnessAns[i] > MAX_KNAPSACK_WEIGHT){
            fitnessAns[i]=0;
        }
    }

    // print(pop, SIZE_OF_INITIAL_POPULATION, 12);
    pop = convertColMajor(pop, SIZE_OF_INITIAL_POPULATION, 12);
    // print(pop, SIZE_OF_INITIAL_POPULATION, 12);

    double* fitnessArray = fitness(weights, values, pop);
    for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        printf("%lf ", fitnessAns[i]);
    }
    printf("\n");
        for(int i=0;i<SIZE_OF_INITIAL_POPULATION;i++){
        printf("%lf ", fitnessArray[i]);
    }
    printf("\n");
    if(check(fitnessArray, fitnessAns, SIZE_OF_INITIAL_POPULATION) != 1){
        printf("GGGGGGGGGGGGGG");
    }
    return 0;
}