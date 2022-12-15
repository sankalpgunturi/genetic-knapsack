#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <immintrin.h>
#include<time.h>
#include"omp.h"
#define DOUBLE_MAX 0
#define ITEM_SIZE 8192
int populationSize = 16384;
//timing routine for reading the time stamp counter
static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

double t0, t1;
unsigned long long tt0;
unsigned long long tt1;
void crossover(double* representation, int popSize, int item_size, double crossover_rate)
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
 #pragma omp parallel for num_threads(16)
for(int i=0; i<popSize; i+=8){
        //printf("%d\n", i);
        // random0 = 0;
        // random1 = 0;
        // random2 = 0;
        // random3 = 0;
        // // generate random numbers
        // __m256d RANDOM = _mm256_set_pd(random0, random1, random2, random3);
        // __m256d compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
        // _mm256_storeu_pd(&cmp[0], compare);
  //#pragma omp parallel for num_threads(4)
    for(int j = 0; j < item_size; j+=4){

    __m256d tmp_1, tmp_2;

    if(cmp[0] != 0){
    __m256d p_11 = _mm256_loadu_pd(&representation[i*item_size+j]);
    __m256d p_21 = _mm256_loadu_pd(&representation[(i+1)*item_size+j]);

    tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
    tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[i*item_size+j], tmp_1);
    // second child
    _mm256_storeu_pd(&representation[(i+1)*item_size+j], tmp_2);
    }

     if(cmp[1] != 0){

    __m256d p_31 = _mm256_loadu_pd(&representation[(i+2)*item_size+j]);
    __m256d p_41 = _mm256_loadu_pd(&representation[(i+3)*item_size+j]);

    tmp_1 = _mm256_permute2f128_pd(p_31, p_41, 0|(3<<4));
    tmp_2 = _mm256_permute2f128_pd(p_41, p_31, 0|(3<<4));
    
    // first child
    _mm256_storeu_pd(&representation[(i+2)*item_size+j], tmp_1);
    // second child
    _mm256_storeu_pd(&representation[(i+3)*item_size+j], tmp_2);

    
    }

    __m256d tmp_3, tmp_4;

    if(cmp[2] != 0){
    __m256d p_51 = _mm256_loadu_pd(&representation[(i+4)*item_size+j]);
    __m256d p_61 = _mm256_loadu_pd(&representation[(i+5)*item_size+j]);

    tmp_3 = _mm256_permute2f128_pd(p_51, p_61, 0|(3<<4));
    tmp_4 = _mm256_permute2f128_pd(p_61, p_51, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[(i+4)*item_size+j], tmp_3);
    // second child
    _mm256_storeu_pd(&representation[(i+5)*item_size+j], tmp_4);
    }

    if(cmp[3] != 0){
    __m256d p_71 = _mm256_loadu_pd(&representation[(i+6)*item_size+j]);
    __m256d p_81 = _mm256_loadu_pd(&representation[(i+7)*item_size+j]);

    tmp_3 = _mm256_permute2f128_pd(p_71, p_81, 0|(3<<4));
    tmp_4 = _mm256_permute2f128_pd(p_81, p_71, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[(i+6)*item_size+j], tmp_3);
    // second child
    _mm256_storeu_pd(&representation[(i+7)*item_size+j], tmp_4);
     }
     }
    }

}

void crossoverWithoutRandom(double* representation, int popSize, double* children)
{
// parents:
// 0 1 0 1 1 0 | 1 0 0 0 0 0 
// 1 1 1 1 1 1 | 0 0 0 1 1 1

// children:
// 0 1 0 1 1 0 | 0 0 0 1 1 1 
// 1 1 1 1 1 1 | 1 0 0 0 0 0 

for(int i=0; i<popSize; i+=2){
    __m256d p_10 = _mm256_loadu_pd(&representation[i*12+0]);
    __m256d p_11 = _mm256_loadu_pd(&representation[i*12+4]);
    __m256d p_12 = _mm256_loadu_pd(&representation[i*12+8]);

    __m256d p_20 = _mm256_loadu_pd(&representation[i*12+12]);
    __m256d p_21 = _mm256_loadu_pd(&representation[i*12+16]);
    __m256d p_22 = _mm256_loadu_pd(&representation[i*12+20]);

    __m256d tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
    __m256d tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&children[i*12+0], p_10);
    _mm256_storeu_pd(&children[i*12+4], tmp_1);
    _mm256_storeu_pd(&children[i*12+8], p_12);

    // second child
    _mm256_storeu_pd(&children[i*12+12], p_20);
    _mm256_storeu_pd(&children[i*12+16], tmp_2);
    _mm256_storeu_pd(&children[i*12+20], p_22);
}

}

void printPop(double* pop){
    for(int i=0;i<12;i++){
        for(int j=0;j<12;j++){
            printf("%lf ", pop[i*12 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void testCrossover(){
    double cross_rate = 0.8;
    srand((unsigned) time(NULL));

    double* parents;

    posix_memalign((void*) &parents, 64, populationSize * ITEM_SIZE * sizeof(double));
    for(int i=0;i<populationSize;i++){
        for(int j=0;j<ITEM_SIZE;j++){
            parents[i*ITEM_SIZE + j] = (int)rand()%2;
        }
    }

    t0 = omp_get_wtime();
    tt0 = rdtsc();
    for(int i=0;i<populationSize;i+=2){
        for(int j=0;j<ITEM_SIZE;j++){
            double random0 = (rand() % (100 - 0))/100.00;
            if(random0 < cross_rate){
                // operateXover(parents[i*]);
                double tmp = parents[i*12+j];
                parents[i*12+j] = parents[(i+1)*12 + j];
                parents[(i+1)*12 + j] = tmp;
            }
        }
    }
    tt1 = rdtsc();
    t1 = omp_get_wtime();
    printf("1: --------- %lf \n", t1-t0);
    printf("rdtsc: %llu\n", tt1-tt0);

    // printPop(parents);
    t0 = omp_get_wtime();
    tt0 = rdtsc();
    crossover(parents, populationSize, ITEM_SIZE, cross_rate);
    tt1 = rdtsc();
    t1 = omp_get_wtime();
    printf("2: --------- %lf \n", t1-t0);

    printf("performance %lf \n", (double)(populationSize*ITEM_SIZE/2)/((tt1-tt0)));


    // printPop(parents);
}

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

#define SIZE_OF_INITIAL_POPULATION 12
double* convertColMajorSIMD(double* matrix, int rowNum, int colNum){
    double* res;
    posix_memalign((void*) &res, 64, rowNum * colNum * sizeof(double));
    
    int index = 0;
    for(int j=0; j < colNum; j++){
        for(int i=0; i < rowNum; i+=16){
            int offset1 = i*SIZE_OF_INITIAL_POPULATION;
            int offset2 = (i+1)*SIZE_OF_INITIAL_POPULATION;
            int offset3 = (i+2)*SIZE_OF_INITIAL_POPULATION;
            int offset4 = (i+3)*SIZE_OF_INITIAL_POPULATION;


            int offset5 = (i+4)*SIZE_OF_INITIAL_POPULATION;
            int offset6 = (i+5)*SIZE_OF_INITIAL_POPULATION;
            int offset7 = (i+6)*SIZE_OF_INITIAL_POPULATION;
            int offset8 = (i+7)*SIZE_OF_INITIAL_POPULATION;


            int offset9 = (i+8)*SIZE_OF_INITIAL_POPULATION;
            int offset10 = (i+9)*SIZE_OF_INITIAL_POPULATION;
            int offset11 = (i+10)*SIZE_OF_INITIAL_POPULATION;
            int offset12 = (i+11)*SIZE_OF_INITIAL_POPULATION;


            int offset13 = (i+12)*SIZE_OF_INITIAL_POPULATION;
            int offset14 = (i+13)*SIZE_OF_INITIAL_POPULATION;
            int offset15 = (i+14)*SIZE_OF_INITIAL_POPULATION;
            int offset16 = (i+15)*SIZE_OF_INITIAL_POPULATION;

            __m256d ymm0 = _mm256_set_pd(matrix[offset4 + j], matrix[offset3 + j], matrix[offset2 + j], matrix[offset1 + j]);
            __m256d ymm1 = _mm256_set_pd(matrix[offset8 + j], matrix[offset7 + j], matrix[offset6 + j], matrix[offset5 + j]);
            __m256d ymm2 = _mm256_set_pd(matrix[offset12 + j], matrix[offset11 + j], matrix[offset10 + j], matrix[offset9 + j]);
            __m256d ymm3 = _mm256_set_pd(matrix[offset16 + j], matrix[offset15 + j], matrix[offset14 + j], matrix[offset13 + j]);
            
            _mm256_storeu_pd(&res[index], ymm0);
            _mm256_storeu_pd(&res[index + 4], ymm1);
            _mm256_storeu_pd(&res[index + 8], ymm2);
            _mm256_storeu_pd(&res[index + 12], ymm3);
            index += 16;
        }
    }
   
   return res;
}

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
void print(double* matrix, int rowNum, int colNum){
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            printf("%lf ", matrix[colNum*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}


void testColMajor(){
    double* matrix;
    int rowNum = 256;
    int colNum = 12;
    posix_memalign((void*) &matrix, 64, rowNum * colNum * sizeof(double));
    int cnt = 0;
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            matrix[i*colNum + j] = cnt++;
        }
    }
    
    // print(matrix, rowNum, colNum);
    t0 = omp_get_wtime();
    double* anotherMatrix = convertColMajor(matrix, rowNum, colNum);
    t1 = omp_get_wtime();
    // print(anotherMatrix, rowNum, colNum);
    printf("time1 = %lf \n", t1 - t0);

    cnt = 0;
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            matrix[i*colNum + j] = cnt++;
        }
    }

    // print(matrix, rowNum, colNum);
    t0 = omp_get_wtime();
    anotherMatrix = convertColMajorSIMD(matrix, rowNum, colNum);
    t1 = omp_get_wtime();
    // print(anotherMatrix, rowNum, colNum);
    printf("time2 = %lf \n", t1 - t0);
}

int main(){
    testCrossover();
    // testColMajor();
    return 0;
}
