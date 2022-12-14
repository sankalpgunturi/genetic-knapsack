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
  int POPULATION_SIZE = atoi(argv[1]);
  int NUMBER_OF_ITEMS = 128;
  int NUMBER_OF_GENERATIONS = atoi(argv[2]);

  int skip_index=8;
  if (argc > 3){
    skip_index = atoi(argv[3]);
  }  
  double *population; 
  double *random, *cmp;
  double *contenders, *winners;
  double *fitnessArray, *weightsArray, *weights, *values; 
  double MAX_KNAPSACK_WEIGHT=18;


  posix_memalign((void*) &population, 64, POPULATION_SIZE * NUMBER_OF_ITEMS * sizeof(double));
  posix_memalign((void*) &random, 64, POPULATION_SIZE * sizeof(double));
  posix_memalign((void*) &contenders, 64, POPULATION_SIZE * NUMBER_OF_ITEMS* sizeof(double)); 
  posix_memalign((void*) &winners, 64, POPULATION_SIZE/2 * NUMBER_OF_ITEMS *sizeof(double));
  posix_memalign((void*) &cmp, 64, 4 * sizeof(double));
  posix_memalign((void*) &fitnessArray, 64, POPULATION_SIZE * NUMBER_OF_ITEMS * sizeof(double));
  posix_memalign((void*) &weightsArray, 64, POPULATION_SIZE * NUMBER_OF_ITEMS * sizeof(double));
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
        for(int k =0; k < NUMBER_OF_ITEMS; k++) {
          fscanf(fp, "%s", &buff);
        }
        rep_fit = 0;
        printf("\n");
      } else if ( (i )% NUMBER_OF_ITEMS == 0) {
        for(int k =0; k < NUMBER_OF_ITEMS*skip_index; k++) {
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
    // selection(weights, values, population, contenders, winners, fitnessArray, weightsArray, NUMBER_OF_ITEMS, POPULATION_SIZE, MAX_KNAPSACK_WEIGHT);
    // printf("\nFIRST SELECTION: %d\n", NUMBER_OF_ITEMS * POPULATION_SIZE/2);
    // for (int i = 0; i < (POPULATION_SIZE / 2) * NUMBER_OF_ITEMS; i++){
    //       printf(" %.2f ", winners[i]);
          
    //       if (winners[i] == 1.0) {
    //         rep_fit += weights[i%NUMBER_OF_ITEMS];
            
    //       }
    //       if ((i+1) %NUMBER_OF_ITEMS ==0) {
    //         printf("\t Fitness: %.2f \n",rep_fit);
    //         rep_fit = 0;
    //       }
          
    // } 

    for (int i = 0; i < POPULATION_SIZE; i++){
      random[i] = (rand() % (128 - 0))/128.00;
    }


    unsigned long long t0, t1;
    unsigned long long selection_t0, selection_total;
    unsigned long long crossover_t0, crossover_total;
    unsigned long long mutation_t0, mutation_total;
    //KERNELS 
    selection_total = 0;
    t0 = rdtsc();
    for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {

      selection_t0 = rdtsc();
      selection(weights, values, population, contenders, winners, fitnessArray, weightsArray, NUMBER_OF_ITEMS, POPULATION_SIZE, MAX_KNAPSACK_WEIGHT);
      selection_total += rdtsc() - selection_t0 ;

      crossover_t0 = rdtsc();
      crossover(population, POPULATION_SIZE, 0.25, random, cmp );
      crossover_total += rdtsc() - crossover_t0;

      mutation_t0 = rdtsc();
      mutation(population, 0.15, random, POPULATION_SIZE );
      mutation_total += rdtsc() - mutation_t0;
    }

    t1 = rdtsc();

    printf("Total time \t\t\t\t: %.2f \n", (double) (t1-t0));
    printf("Total Selection time \t\t\t\t: %.2f \n", (double) (selection_total));
    printf("Total Crossover time \t\t\t\t: %.2f \n", (double) (crossover_total));
    printf("Total Mutation time \t\t\t\t: %.2f \n", (double) (mutation_total));


    printf("Average time per generation \t\t\t: %.2f \n", (double) (t1-t0) / NUMBER_OF_GENERATIONS);
    printf("Average Selection time per generation \t\t: %.2f \n", (double) (selection_total) / NUMBER_OF_GENERATIONS);
    printf("Average Crossover time per generation \t\t: %.2f \n", (double) (crossover_total)/ NUMBER_OF_GENERATIONS);
    printf("Average Mutation time per generation \t\t: %.2f \n", (double) (mutation_total)/ NUMBER_OF_GENERATIONS);

    // printf("\nFINAL GENERATION:\n");
    // for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
    //       printf(" %.2f ",population[i]);
    //       if ((i+1) %NUMBER_OF_ITEMS ==0) {
    //         printf("\n");
    //       }
    // } 

  
  free(population);
  free(fitnessArray);
  free(weightsArray);
  free(weights);
  free(values);
  free(contenders);
  free(random);
  free(cmp);
  free(winners);
  return 0;

}