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
  
  // population = generate_initial_population()

  //   avg_fitnesses = []

  //   for _ in range(NUMBER_OF_GENERATIONS):
  //       avg_fitnesses.append(average_fitness(population))
  //       population = next_generation(population)

  //   population = sorted(population, key=lambda i: i.fitness(), reverse=True)
  // def average_fitness(population: List[Individual]) -> float:
  //   return sum([i.fitness() for i in population]) / len(population)

  for(int g = 0; g < NUMBER_OF_GENERATIONS; g++ ) {
    crossover(population, POPULATION_SIZE, 0.25 );
    mutation(population, 0.15 );
  }
  
  printf("\nFINAL GENERATION:\n");
  for (int i = 0; i < POPULATION_SIZE * NUMBER_OF_ITEMS; i++){
        printf(" %.2f ",population[i]);
        if ((i+1) %12 ==0) {
          printf("\n");
        }
  } 

 return 0;

 free(population);

}