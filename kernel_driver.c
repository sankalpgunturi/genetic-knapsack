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
  posix_memalign((void*) &population, 64, POPULATION_SIZE * 12 * sizeof(double));
  
  srand(time(NULL));
  
  int skip_index = 15;
  FILE *fp = fopen("POPULATION_4096.txt", "r");

  if (fp == NULL){
    printf("Couldn't open file\n");
    return 0;
  }

  char buff[1]; 


  for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
    if ( i == 0) {
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      printf("\n");
    } else if ( (i )% 12 == 0) {
      //After every 12 items
      for(int k =0; k < 12*14; k++) {
        fscanf(fp, "%s", buff);
      }
      printf("\n");
    } 
      fscanf(fp, "%s", buff);
      population[i] = buff[0] - 48.0;
      printf(" %.2f ", population[i]);

   }
  mutation(population, 0.50 );
  printf("\nAFTER MUTATION\n");
           
  for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
        printf(" %.2f ",population[i]);
        if ((i+1) %12 ==0) {
          printf("\n");
        }
  } 

 return 0;
}