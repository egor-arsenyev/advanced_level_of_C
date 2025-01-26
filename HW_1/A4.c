#include <stdio.h>
#include <stdint.h>

// Function to find the maximum K consecutive bits in N
uint32_t find_max_bits(uint32_t n, uint8_t k) {
    uint32_t max_value = 0; 
    uint32_t mask = (1U << k) - 1;

    for (int i = 0; i <= 32 - k; i++) {
        // Extract the K bits starting from bit i
        uint32_t current_value = (n >> i) & mask;

        if (current_value > max_value) {
            max_value = current_value;
        }
    }
    
    return max_value;
}

int main() {
    uint32_t N;
    uint8_t K;

    printf("Enter a 32-bit unsigned integer (N, 0 <= N <= 4294967295): ");
    if (scanf("%u", &N) != 1) {
        printf("Invalid input for N.\n");
        return 1;
    }

    printf("Enter an integer (K, 1 <= K <= 31): ");
    if (scanf("%hhu", &K) != 1 || K < 1 || K > 31) {
        printf("Invalid input for K. It must be between 1 and 31.\n");
        return 1;
    }

    uint32_t result = find_max_bits(N, K);
    printf("Maximum number from %u consecutive bits: ", K);
    printf("%u\n", result);

    return 0;
}
