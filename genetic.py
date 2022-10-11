import random
from typing import List
# from hwcounter import Timer, count, count_end
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


MAX_KNAPSACK_WEIGHT = 15
CROSSOVER_RATE = 0.53
MUTATION_RATE = 0.013
REPRODUCTION_RATE = 0.15
MAX_INITAL_POPULATION = 100
MAX_CONTENDERS_FOR_TOURNAMENT = 50
NUMBER_OF_GENERATIONS = 500

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
    Item("J", 10, 2)
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

    # we use the first MAX_CONTENDERS_FOR_TOURNAMENT individuals
    # run a tournament between them and
    # get two fit parents for the next steps of evolution

    for population_index in range(0, MAX_CONTENDERS_FOR_TOURNAMENT - 1):
        if population[population_index].fitness() > population[population_index + 1].fitness():
            parents.append(population[population_index])
        else:
            parents.append(population[population_index + 1])

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
    # start = count()
    start = time.clock() 
    population = generate_initial_population()

    avg_fitnesses = []

    for _ in range(NUMBER_OF_GENERATIONS):
        avg_fitnesses.append(average_fitness(population))
        population = next_generation(population)

    population = sorted(population, key=lambda i: i.fitness(), reverse=True)
    # elapsed = count_end() - start
    end = time.clock() 
    # print(f'elapsed cycles: {elapsed}')
    print("Time elapsed during the calculation:", end - start)  
    return population[0]


if __name__ == '__main__':
    solution = solve_knapsack()
    print(solution, solution.fitness())
