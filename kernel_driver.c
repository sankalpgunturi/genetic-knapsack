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

int main(){
  double *population;
  int POPULATION_SIZE = 256;
  int NUMBER_OF_ITEMS = 12;
  int NUMBER_OF_GENERATIONS = 1;
  posix_memalign((void*) &population, 64, POPULATION_SIZE * 12 * sizeof(double));
  
  srand(time(NULL));
  
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
  int rep_fit = 0;
  for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
    if ( i == 0) {
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      rep_fit = 0;
      printf("\n");
    } else if ( (i )% 12 == 0) {
      //After every 12 items
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      printf("\t Fitness val : %.2f \n", (rep_fit/12.0));
      rep_fit = 0;
      break;
    } 
      fscanf(fp, "%s", buff);
      population[i] = buff[0] - 48.0;
      printf(" %.2f ", population[i]);
      rep_fit += weights[i%12];
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

  selection(weights, values, population);
  // for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
  //   crossover(population, POPULATION_SIZE, 0.25 );
  //   mutation(population, 0.15 );
  // }
  
  // printf("\nFINAL GENERATION:\n");
  // for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
  //       printf(" %.2f ",population[i]);
  //       if ((i+1) %12 ==0) {
  //         printf("\n");
  //       }
  // } 

 return 0;

 free(population);

}