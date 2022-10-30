// SIMD version of genetic_baseline
void selection(double* pack_a, double* a, int mc, int m, int k)
{

}

// input n individuals(fitness array with length = n)
// 256 items
// representations: n * 256 bit (n rows, 4 columns)
void crossover(int n, double* fitness, double* representations)
{   
    int m = 4;
    for(int i=0;i<n;i+=2){
        // load
        __m256d P1 = _mm256_load_pd(representations[i*4]);
        __m256d P2 = _mm256_load_pd(representations[(i+1)*4]);
        // permute
        __m256d C1 = mm256_permute2f128_pd(P1, P2, (0|(3<<4)));
        __m256d C2 = mm256_permute2f128_pd(P1, P2, (1|(2<<4)));
        // load into memory
        _mm256_storeu_pd(&representations[i*4], C1);
        _mm256_storeu_pd(&representations[(i+1)*4], C2);
    }
}

void mutation(){

}