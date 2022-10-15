import random
from typing import List
import time

class Item:
    def __init__(self, name, weight, value):
        self.name = name
        self.weight = weight
        self.value = value


class Individual:
    def __init__(self, bits: List[int]):
        self.bits = bits

    def __str__(self):
        return repr(self.bits)

    def __hash__(self):
        return hash(str(self.bits))

    def fitness(self) -> float:
        total_value = sum([
            bit * item.value
            for item, bit in zip(items, self.bits)
        ])

        total_weight = sum([
            bit * item.weight
            for item, bit in zip(items, self.bits)
        ])

        if total_weight <= MAX_KNAPSACK_WEIGHT:
            return total_value

        return 0

global time_elapsed
# MACROS
MAX_KNAPSACK_WEIGHT = 15
CROSSOVER_RATE = 0.53
MUTATION_RATE = 0.013
REPRODUCTION_RATE = 0.15

# Vary this for different results
NUMBER_OF_GENERATIONS = 100

#DON'T CHANGE THIS VALUE
MAX_INITAL_POPULATION = 128
MAX_CONTENDERS_FOR_TOURNAMENT = 4
items = [
    Item("A", 7, 5),
    Item("B", 2, 4),
    Item("C", 1, 7),
    Item("D", 9, 2),
    Item("E", 3, 6),
    Item("F", 4, 3),
    Item("G", 6, 1),
    Item("H", 8, 9),
    Item("I", 5, 9),
    Item("J", 10, 2),
    Item("K", 5, 3),
    Item("L", 3, 1),
]


def generate_initial_population(count=MAX_INITAL_POPULATION) -> List[Individual]:
    population = set()

    # generate initial population having `count` individuals
    while len(population) != count:
        # pick random bits one for each item and
        # create an individual
        bits = [
            random.choice([0, 1])
            for _ in items
        ]
        population.add(Individual(bits))

    return list(population)

def computeFitness(population: List[Individual]) -> List[Individual]:
    fitness_array = []
    for population_index in range(0, len(population) - 1):
        fitness_array.append(population[population_index].fitness())

    return fitness_array


def selection(population_before_shuffle: List[Individual]) -> List[Individual]:
    global time_elapsed
    parents = []

    # randomly shuffle the population and fitness together
    start = time.time()
    fitness_before_shuffle = computeFitness(population_before_shuffle)
    end = time.time()
    time_elapsed += (end - start)

    temp = list(zip(population_before_shuffle, fitness_before_shuffle))
    random.shuffle(temp)
    population, fitness_array = zip(*temp)
    population, fitness_array = list(population), list(fitness_array)
    
    # we use the first MAX_CONTENDERS_FOR_TOURNAMENT individuals
    # run a tournament between them and
    # get two fit parents for the next steps of evolution
    start = time.time()
    for population_index in range(0, MAX_CONTENDERS_FOR_TOURNAMENT - 1, 2):
        if fitness_array[population_index] > fitness_array[population_index + 1]:
            parents.append(population[population_index])
        else:
            parents.append(population[population_index + 1])

    end = time.time()
    time_elapsed += (end - start)
    return parents


def crossover(parents: List[Individual]) -> List[Individual]:
    N = len(items)

    child1 = parents[0].bits[:N//2] + parents[1].bits[N//2:]
    child2 = parents[0].bits[N//2:] + parents[1].bits[:N//2]

    return [Individual(child1), Individual(child2)]


def mutate(individuals: List[Individual]) -> List[Individual]:
    for individual in individuals:
        for i in range(len(individual.bits)):
            if random.random() < MUTATION_RATE:
                # Flip the bit
                individual.bits[i] = ~individual.bits[i]


def next_generation(population: List[Individual]) -> List[Individual]:
    next_gen = []
    while len(next_gen) < len(population):
        children = []

        # we run selection and get parents
        parents = selection(population)

        # reproduction
        if random.random() < REPRODUCTION_RATE:
            children = parents
        else:
            # crossover
            if random.random() < CROSSOVER_RATE:
                children = crossover(parents)

            # mutation
            if random.random() < MUTATION_RATE:
                mutate(children)

        next_gen.extend(children)

    return next_gen[:len(population)]


def print_generation(population: List[Individual]):
    for individual in population:
        print(individual.bits, individual.fitness())
    print()
    print("Average fitness", sum([x.fitness()
          for x in population])/len(population))
    print("-" * 32)


def average_fitness(population: List[Individual]) -> float:
    return sum([i.fitness() for i in population]) / len(population)


def solve_knapsack() -> Individual:
    global time_elapsed

    population = generate_initial_population()

    avg_fitnesses = []

    time_elapsed = 0
    for _ in range(NUMBER_OF_GENERATIONS):
        # avg_fitnesses.append(average_fitness(population))
        _ = selection(population)
        # population = next_generation(population)

    # population = sorted(population, key=lambda i: i.fitness(), reverse=True)

    print("Time:", time_elapsed)
    print("FLOPS/sec:", (MAX_INITAL_POPULATION * MAX_CONTENDERS_FOR_TOURNAMENT * NUMBER_OF_GENERATIONS) / time_elapsed)

    return population[0]


if __name__ == '__main__':
    solution = solve_knapsack()
    print(solution, solution.fitness())
