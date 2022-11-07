#include <stdio.h>
#include <stdlib.h>
#include "immintrin.h"

int main()
{
    __m256d result = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    __m256d totalWeight = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);
    __m256d totalValue = _mm256_set_pd(0.0, 0.0, 0.0, 0.0);

    result = _mm256_cmp_pd(totalWeight, totalValue);
    printf("%f", result);

    printf("Hello World");
    return 0;
}
