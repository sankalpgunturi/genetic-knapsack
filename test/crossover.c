#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <immintrin.h>
#include<time.h>
#define DOUBLE_MAX 0

//timing routine for reading the time stamp counter
static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
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

for(int i=0; i<popSize; i+=4){

    if(i%8 == 0){
        random0 = (rand() % (100 - 0))/100.00;
        random1 = (rand() % (100 - 0))/100.00;
        random2 = (rand() % (100 - 0))/100.00;
        random3 = (rand() % (100 - 0))/100.00;
        // generate random numbers
        __m256d RANDOM = _mm256_set_pd(random0, random1, random2, random3);
        __m256d compare = _mm256_cmp_pd(CROSS_RATE, RANDOM, 14);
        _mm256_storeu_pd(&cmp[0], compare);
    }

    // if rate < cross_rate, 1111, else 0000
    // printf("%f ", cmp[(i/2)%4]);
    if(cmp[(i/2)%4] != 0){

    
    __m256d p_10 = _mm256_loadu_pd(&representation[i*12+0]);
    __m256d p_11 = _mm256_loadu_pd(&representation[i*12+4]);
    __m256d p_12 = _mm256_loadu_pd(&representation[i*12+8]);

    __m256d p_20 = _mm256_loadu_pd(&representation[i*12+12]);
    __m256d p_21 = _mm256_loadu_pd(&representation[i*12+16]);
    __m256d p_22 = _mm256_loadu_pd(&representation[i*12+20]);

    __m256d tmp_1 = _mm256_permute2f128_pd(p_11, p_21, 0|(3<<4));
    __m256d tmp_2 = _mm256_permute2f128_pd(p_21, p_11, 0|(3<<4));

    // first child
    _mm256_storeu_pd(&representation[i*12+0], p_10);
    _mm256_storeu_pd(&representation[i*12+4], tmp_1);
    _mm256_storeu_pd(&representation[i*12+8], p_12);

    // second child
    _mm256_storeu_pd(&representation[i*12+12], p_20);
    _mm256_storeu_pd(&representation[i*12+16], tmp_2);
    _mm256_storeu_pd(&representation[i*12+20], p_22);
    

    // repeat one time for larger register usage
    __m256d p_30 = _mm256_loadu_pd(&representation[i*12+24]);
    __m256d p_31 = _mm256_loadu_pd(&representation[i*12+28]);
    __m256d p_32 = _mm256_loadu_pd(&representation[i*12+32]);

    __m256d p_40 = _mm256_loadu_pd(&representation[i*12+36]);
    __m256d p_41 = _mm256_loadu_pd(&representation[i*12+40]);
    __m256d p_42 = _mm256_loadu_pd(&representation[i*12+44]);

    tmp_1 = _mm256_permute2f128_pd(p_31, p_41, 0|(3<<4));
    tmp_2 = _mm256_permute2f128_pd(p_41, p_31, 0|(3<<4));
    
    // first child
    _mm256_storeu_pd(&representation[i*12+24], p_30);
    _mm256_storeu_pd(&representation[i*12+28], tmp_1);
    _mm256_storeu_pd(&representation[i*12+32], p_32);

    // second child
    _mm256_storeu_pd(&representation[i*12+36], p_40);
    _mm256_storeu_pd(&representation[i*12+40], tmp_2);
    _mm256_storeu_pd(&representation[i*12+44], p_42);

    }
    else{

    // keep the same
    __m256d p_10 = _mm256_loadu_pd(&representation[i*12+0]);
    __m256d p_11 = _mm256_loadu_pd(&representation[i*12+4]);
    __m256d p_12 = _mm256_loadu_pd(&representation[i*12+8]);

    __m256d p_20 = _mm256_loadu_pd(&representation[i*12+12]);
    __m256d p_21 = _mm256_loadu_pd(&representation[i*12+16]);
    __m256d p_22 = _mm256_loadu_pd(&representation[i*12+20]);


    // first child
    _mm256_storeu_pd(&representation[i*12+0], p_10);
    _mm256_storeu_pd(&representation[i*12+4], p_11);
    _mm256_storeu_pd(&representation[i*12+8], p_12);

    // second child
    _mm256_storeu_pd(&representation[i*12+12], p_20);
    _mm256_storeu_pd(&representation[i*12+16], p_21);
    _mm256_storeu_pd(&representation[i*12+20], p_22);

    // repeat for one more time for more usage of registers
    // keep the same
    __m256d p_30 = _mm256_loadu_pd(&representation[i*12+24]);
    __m256d p_31 = _mm256_loadu_pd(&representation[i*12+28]);
    __m256d p_32 = _mm256_loadu_pd(&representation[i*12+32]);

    __m256d p_40 = _mm256_loadu_pd(&representation[i*12+36]);
    __m256d p_41 = _mm256_loadu_pd(&representation[i*12+40]);
    __m256d p_42 = _mm256_loadu_pd(&representation[i*12+44]);


    // first child
    _mm256_storeu_pd(&representation[i*12+24], p_30);
    _mm256_storeu_pd(&representation[i*12+28], p_31);
    _mm256_storeu_pd(&representation[i*12+32], p_32);

    // second child
    _mm256_storeu_pd(&representation[i*12+36], p_40);
    _mm256_storeu_pd(&representation[i*12+40], p_41);
    _mm256_storeu_pd(&representation[i*12+44], p_42);

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
    int populationSize = 128;
    double* parents;
    posix_memalign((void*) &parents, 64, populationSize * 12 * sizeof(double));
    for(int i=0;i<128;i++){
        for(int j=0;j<12;j++){
            parents[i*12 + j] = (int)rand()%2;
        }
    }
    printPop(parents);
    crossover(parents, populationSize, cross_rate);
    printPop(parents);
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

void print(double* matrix, int rowNum, int colNum){
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            printf("%lf ", matrix[colNum*i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

unsigned long long t0, t1;
void testColMajor(){
    double* matrix;
    int rowNum = 4;
    int colNum = 12;
    posix_memalign((void*) &matrix, 64, rowNum * colNum * sizeof(double));
    int cnt = 0;
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            matrix[i*colNum + j] = cnt++;
        }
    }
    
    print(matrix, rowNum, colNum);
    t0 = rdtsc();
    double* anotherMatrix = convertColMajor(matrix, rowNum, colNum);
    t1 = rdtsc();
    print(anotherMatrix, rowNum, colNum);
    printf("time1 = %d \n", t1 - t0);

    cnt = 0;
    for(int i=0;i<rowNum;i++){
        for(int j=0;j<colNum;j++){
            matrix[i*colNum + j] = cnt++;
        }
    }

    print(matrix, rowNum, colNum);
    t0 = rdtsc();
    anotherMatrix = convertColMajorSIMD(matrix, rowNum, colNum);
    t1 = rdtsc();
    print(anotherMatrix, rowNum, colNum);
    printf("time2 = %d \n", t1 - t0);
}

int main(){
    testCrossover();
    // testColMajor();
    return 0;
}
