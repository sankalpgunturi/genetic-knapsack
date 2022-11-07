#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "immintrin.h"

#include "genetic.h"
#include "genetic_baseline.h" //NOT INCLUDED FOR SUBMISSION


static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

// def next_generation(population: List[Individual]) -> List[Individual]:
//     next_gen = []
//     while len(next_gen) < len(population):
//         children = []

//         # we run selection and get parents
//         parents = selection(population)

//         # reproduction
//         if random.random() < REPRODUCTION_RATE:
//             children = parents
//         else:
//             # crossover
//             if random.random() < CROSSOVER_RATE:
//                 children = crossover(parents)
            
//             # mutation
//             if random.random() < MUTATION_RATE:
//                 mutate(children)

//         next_gen.extend(children)

//     return next_gen[:len(population)]
//
// def print_generation(population: List[Individual]):
//     for individual in population:
//         print(individual.bits, individual.fitness())
//     print()
//     print("Average fitness", sum([x.fitness() for x in population])/len(population))
//     print("-" * 32)
//
// def solve_knapsack() -> Individual:
//     population = generate_initial_population()

//     avg_fitnesses = []

//     for _ in range(NUMBER_OF_GENERATIONS):
//         avg_fitnesses.append(average_fitness(population))
//         population = next_generation(population)

//     population = sorted(population, key=lambda i: i.fitness(), reverse=True)
//     return population[0]


// def generate_initial_population(count=MAX_INITAL_POPULATION) -> List[Individual]:
//     population = set()

//     # generate initial population having `count` individuals
//     while len(population) != count:
//         # pick random bits one for each item and 
//         # create an individual 
//         bits = [
//             random.choice([0, 1])
//             for _ in items
//         ]
//         population.add(Individual(bits))

//     return list(population)

// double* generate_initial_population(int count) {
//      double* population;
     
//      for (int k = 0; k < 12; k++) {
//         int temp = rand() % 1;
//         population[k] = (double)temp;
//         printf("%d \t %d \t %f ", count, k, population[k]);
//      }
//      printf("\n");
// }

int main(){
  double *population;
  int POPULATION_SIZE = 256;
  int NUMBER_OF_ITEMS = 12;
  posix_memalign((void**) &population, 64, POPULATION_SIZE * 12 * sizeof(double));
  
  srand(time(NULL));
  
  //CURRENT HARDCODED 256 indexes to choose from 4096 combinations
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
      }printf("\n");
    } 
      fscanf(fp, "%s", buff);
      //printf("i %d : %s\t", i, buff );
      population[i] = buff[0] - 48.0;
      printf("i %d, %f ", i, population[i]);

  }
 return 0;
}