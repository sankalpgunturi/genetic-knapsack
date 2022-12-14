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
  
  int POPULATION_SIZE = 1024;
  int NUMBER_OF_ITEMS = 128;
  int skip_index = 8;
  int NUMBER_OF_GENERATIONS = 100000;
  double *population, *selection_pop;
  double *random, *cmp;
  double *contenders, *winners;
  double *fitnessArray, *weightsArray, *weights, *values; 
  double MAX_KNAPSACK_WEIGHT=18;


  posix_memalign((void*) &population, 64, POPULATION_SIZE * NUMBER_OF_ITEMS * sizeof(double));
  posix_memalign((void*) &selection_pop, 64, (POPULATION_SIZE / 2) * NUMBER_OF_ITEMS * sizeof(double));
  posix_memalign((void*) &random, 64, POPULATION_SIZE * sizeof(double));
  posix_memalign((void*) &contenders, 64, POPULATION_SIZE * sizeof(double)); 
  posix_memalign((void*) &winners, 64, POPULATION_SIZE * sizeof(double));
  posix_memalign((void*) &cmp, 64, 4 * sizeof(double));
  posix_memalign((void*) &fitnessArray, 64, POPULATION_SIZE * sizeof(double));
  posix_memalign((void*) &weightsArray, 64, POPULATION_SIZE * sizeof(double));
  posix_memalign((void*) &values, 64, NUMBER_OF_ITEMS * sizeof(double));
  posix_memalign((void*) &weights, 64, NUMBER_OF_ITEMS * sizeof(double));

  for(int i = 0; i < NUMBER_OF_ITEMS; i++ ) {
    weights[i] = (rand() % (96-0))/96.0;
    values[i] = (rand() %(96-0))/96.0;
  }

  printf("Weights Values_set\n");
  
  FILE *fp = fopen("POPULATION_4096_12 comb - Final.csv", "r");

  if (fp == NULL){
    printf("Couldn't open file\n");
    return 0;
  }

    char buff[1]; 
    double rep_fit = 0;
    printf("POP_SIZE %d", POPULATION_SIZE * NUMBER_OF_ITEMS);
    for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
      if ( i == 0) {

        // printf("k limit %d ", NUMBER_OF_ITEMS*skip_index);
        for(int k =0; k < NUMBER_OF_ITEMS; k++) {
          // printf(" k %d", k);
          fscanf(fp, "%s", &buff);
        }
        rep_fit = 0;
        printf("\n");
      } else if ( (i )% NUMBER_OF_ITEMS == 0) {
        printf("k limit %d ", NUMBER_OF_ITEMS*skip_index);
        for(int k =0; k < NUMBER_OF_ITEMS*skip_index; k++) {
          // printf(" k %d", k);
          fscanf(fp, "%s", &buff);
        }
        printf("\t Fitness val : %.2f \n", (rep_fit));
        rep_fit = 0;
      } 
      fscanf(fp, "%s", &buff);
      population[i] = buff[0] - 48.0;
      printf(" %.2f ", population[i]);
      if (population[i] == 1.0) {
        rep_fit += weights[i%NUMBER_OF_ITEMS];
      }
   }
  
    rep_fit = 0;
    selection_pop = selection(weights, values, population, contenders, winners, fitnessArray, weightsArray, NUMBER_OF_ITEMS, POPULATION_SIZE, MAX_KNAPSACK_WEIGHT);
  // printf("\nFIRST SELECTION:\n");
  // for (int i = 0; i < NUMBER_OF_ITEMS * POPULATION_SIZE; i++){
  //       printf(" %.2f ",selection_pop[i]);
  //       if (selection_pop[i] == 1.0) {
  //         rep_fit += weights[i%NUMBER_OF_ITEMS];
          
  //       }
  //       if ((i+1) %NUMBER_OF_ITEMS ==0) {
  //         printf("\t Fitness: %.2f \n",rep_fit);
  //         rep_fit = 0;
  //       }
        
  // } 
//   // for (int i = 0; i < POPULATION_SIZE; i++){
//   //   random[i] = (rand() % (128 - 0))/128.00;
//   // }
//   // for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
//   //   crossover(population, POPULATION_SIZE, 0.25, random, cmp );
//   //   mutation(population, 0.15, random, POPULATION_SIZE );
//   // }
//   // printf("\nFINAL GENERATION:\n");
//   // for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
//   //       printf(" %.2f ",population[i]);
//   //       if ((i+1) %NUMBER_OF_ITEMS ==0) {
//   //         printf("\n");
//   //       }
//   // } 

  
  free(population);
  free(selection_pop);
  free(random);
  free(cmp);
  free(contenders);
  free(winners);
  free(fitnessArray);
  free(weightsArray);
  free(weights);
  free(values);
  return 0;

}