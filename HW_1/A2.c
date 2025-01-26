#include <stdio.h>
#include <stdint.h>

// Function to perform cyclic right shift
uint32_t cyclic_shift(uint32_t n, uint8_t k) {
    k = k % 32; // Restrict k to range [0, 31]
    return (n >> k) | (n << (32 - k));
}

int main() {
    uint32_t n; 
    uint8_t k;  

//    printf("Enter the number N (unsigned 32-bit integer): ");
    if (scanf("%u", &n) != 1) {
        fprintf(stderr, "Error: Invalid input for N.\n");
        return 1;
    }

//    printf("Enter the number K (1 <= K <= 31): ");
    if (scanf("%hhu", &k) != 1 || k < 1 || k > 31) { 
        fprintf(stderr, "Error: K must be in the range 1 ≤ K ≤ 31.\n");
        return 1; 
    }

    uint32_t result = cyclic_shift(n, k);

//    printf("Result after cyclic shift: ");
    printf("%u\n", result);

    return 0; 
}
