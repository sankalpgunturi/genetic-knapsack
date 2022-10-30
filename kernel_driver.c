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

struct Item{
  double weight, value; 
  Item(double w, double v){
    weight = w;
    value = v;
  } 
  Item(){}
}

struct Individual{
  double* representation;
  double fitness;
}

void init(){
   // initialize random populations
   // 
}

int main(){


 return 0;
}