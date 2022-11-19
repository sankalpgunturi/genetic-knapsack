#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "immintrin.h"

#include "genetic.h"
//#include "genetic_baseline.h" //NOT INCLUDED FOR SUBMISSION


static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

int main(int argc, char** argv){
  double *population, *selection_pop;
  int POPULATION_SIZE = 256;
  int NUMBER_OF_ITEMS = 12;
  int NUMBER_OF_GENERATIONS = 1;
  posix_memalign((void*) &population, 64, POPULATION_SIZE * 12 * sizeof(double));
  posix_memalign((void*) &selection_pop, 64, (POPULATION_SIZE / 2) * 12 * sizeof(double));
  srand(time(NULL));
  if (argc < 2){
    printf("kernel_d.c <<Number of Generations>>\n");
    exit(0);
  }  
  else{
    NUMBER_OF_GENERATIONS = atoi(argv[1]);
  }
  
  int skip_index = 15;
  FILE *fp = fopen("POPULATION_4096.txt", "r");

  if (fp == NULL){
    printf("Couldn't open file\n");
    return 0;
  }

  char buff[1]; 
  double *weights;
  posix_memalign((void*) &weights, 64, 12 * sizeof(double));

  weights[0] = 0;
  weights[1] = 2;
  weights[2] = 3;
  weights[3] = 0;
  weights[4] = 2;
  weights[5] = 3;
  weights[6] = 0;
  weights[7] = 2;
  weights[8] = 3;
  weights[9] = 0;
  weights[10] = 2;
  weights[11] = 3;
  double rep_fit = 0;

  for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
    if ( i == 0) {
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      rep_fit = 0;
      //printf("\n");
    } else if ( (i )% 12 == 0) {
      //After every 12 items
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      //printf("\t Fitness val : %.2f \n", (rep_fit));
      rep_fit = 0;
    } 
      fscanf(fp, "%s", buff);
      population[i] = buff[0] - 48.0;
      //printf(" %.2f ", population[i]);
      if (population[i] == 1.0) {
        rep_fit += weights[i%12];
      }
   }
  

  double *values;
  posix_memalign((void*) &values, 64, 12 * sizeof(double));

  values[0] = 0;
  values[1] = 2;
  values[2] = 3;
  values[3] = 0;
  values[4] = 2;
  values[5] = 3;
  values[6] = 0;
  values[7] = 2;
  values[8] = 3;
  values[9] = 0;
  values[10] = 2;
  values[11] = 3;

  // rep_fit = 0;
  // selection_pop = selection(weights, values, population);
  // printf("\nFIRST SELECTION:\n");
  // for (int i = 0; i < 128 * NUMBER_OF_ITEMS; i++){
  //       printf(" %.2f ",selection_pop[i]);
  //       if (selection_pop[i] == 1.0) {
  //         rep_fit += weights[i%12];
          
  //       }
  //       if ((i+1) %12 ==0) {
  //         printf("\t Fitness: %.2f \n",rep_fit);
  //         rep_fit = 0;
  //       }
        
  // } 
  int SIZE_OF_INITIAL_POPULATION = 256;
  double* fitnessArray;
  posix_memalign((void*) &fitnessArray, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

  double *contenders, *winners;
  posix_memalign((void*) &contenders, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double)); 
  posix_memalign((void*) &winners, 64, SIZE_OF_INITIAL_POPULATION * sizeof(double));

  int sum =0;
  int t1, t0;
  for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
    t0 = rdtsc();
    selection(weights, values, population, contenders, winners, fitnessArray);
    t1 = rdtsc();
    sum += (t1-t0);
  }
  // int sum1 =0;
  // for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
  //   t0 = rdtsc();
  //   crossover(population, POPULATION_SIZE, 0.25 );
  //   t1 = rdtsc();
  //   sum1 += (t1-t0);
  // }
  // int sum2 =0;
  // for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
  //   t0 = rdtsc();
  //   mutation(population, 0.15 );
  //   t1 = rdtsc();
  //   sum2 += (t1-t0);
  // }
  
  // printf("\nFINAL GENERATION:\n");
  // for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
  //       printf(" %.2f ",population[i]);
  //       if ((i+1) %12 ==0) {
  //         printf("\n");
  //       }
  // } 

  printf("# GENERATIONS | FLOPS/sec ");
  // # No. of ops in one cycle x No. of generations / (throughput x SIMD_length) x Max. freq
 printf("%.2f, %.2f\n", NUMBER_OF_GENERATIONS, (128*NUMBER_OF_GENERATIONS ) / ((double)(t1-t0)*(3.4/2.4)));
 free(population);
 return 0;

}