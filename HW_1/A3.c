#include <stdio.h>
#include <stdint.h>

uint32_t extract_bits(uint32_t N, unsigned int K) {
    if (K > 31 || K == 0) {
        fprintf(stderr, "Error: K must be in the range [1, 31]\n");
        return 0;
    }
    uint32_t mask = (1U << K) - 1;
    return N & mask;
}

int main() {
    uint32_t N;
    unsigned int K;

    printf("Enter a 32-bit unsigned number (N): ");
    if (scanf("%u", &N) != 1) {
        fprintf(stderr, "Error: Invalid input for N.\n");
        return 1;
    }

    printf("Enter a natural number (K) in the range [1, 31]: ");
    if (scanf("%u", &K) != 1 || K < 1 || K > 31) {
        fprintf(stderr, "Error: K must be a natural number in the range [1, 31].\n");
        return 1;
    }

    uint32_t result = extract_bits(N, K);
    printf("result: ");
    printf("%u\n",result);
    
    return 0;
}
