import random
from typing import List
from hwcounter import count, Timer, count_end
import sys

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

global DURATION
global OPERATIONS
MAX_KNAPSACK_WEIGHT = 15
CROSSOVER_RATE = 0.53
MUTATION_RATE = 0.013
REPRODUCTION_RATE = 0.15
NUMBER_OF_GENERATIONS = int(sys.argv[1])
MAX_INITAL_POPULATION = 128

#DON'T CHANGE THIS VALUE
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


def selection(population: List[Individual]) -> List[Individual]:
    parents = []
    
    # randomly shuffle the population
    random.shuffle(population)

    # we use the first 4 individuals
    # run a tournament between them and
    # get two fit parents for the next steps of evolution

    # tournament between first and second
    if population[0].fitness() > population[1].fitness():
        parents.append(population[0])
    else:
        parents.append(population[1])
    
    # tournament between third and fourth
    if population[2].fitness() > population[3].fitness():
        parents.append(population[2])
    else:
        parents.append(population[3])

    return parents


def crossover(parents: List[Individual]) -> List[Individual]:
    N = len(items)

    child1 = parents[0].bits[:N//2] + parents[1].bits[N//2:]
    child2 = parents[0].bits[N//2:] + parents[1].bits[:N//2]

    return [Individual(child1), Individual(child2)]


def mutate(individuals: List[Individual]) -> List[Individual]:
    global DURATION
    global OPERATIONS
    start = count()
    for individual in individuals:
        for i in range(len(individual.bits)):
            if random.random() < MUTATION_RATE:
                # Flip the bit
                individual.bits[i] = ~individual.bits[i]
    end = count_end()
    #print("OPERATIONS BEFORE", OPERATIONS , len(individuals))
    if len(individuals) > 0 :
        OPERATIONS += len(individuals) * len(individuals[0].bits)
    #print("OPERATIONS AFTER", OPERATIONS)
    #print("DURATION BEFORE", DURATION)
    DURATION += (end-start)
    #print("DURATION AFTER", DURATION)

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
    print("Average fitness", sum([x.fitness() for x in population])/len(population))
    print("-" * 32)


def average_fitness(population: List[Individual]) -> float:
    return sum([i.fitness() for i in population]) / len(population)


def solve_knapsack() -> Individual:
    population = generate_initial_population()

    avg_fitnesses = []

    for _ in range(NUMBER_OF_GENERATIONS):
        avg_fitnesses.append(average_fitness(population))
        population = next_generation(population)

    population = sorted(population, key=lambda i: i.fitness(), reverse=True)
    return population[0]


if __name__ == '__main__':
    global DURATION
    global OPERATIONS
    DURATION = 0
    OPERATIONS = 0
    solution = solve_knapsack()
    #print(solution, solution.fitness())
    # No. of ops in one cycle x No. of generations / (throughput x SIMD_length) x Max. freq
    print( (OPERATIONS * NUMBER_OF_GENERATIONS )/ ()(3.4))