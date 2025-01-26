#include <stdio.h>
#include <stdint.h>

// Function to invert the highest byte of a 32-bit unsigned integer
uint32_t inv_high_byte(uint32_t n) {
    uint32_t mask = 0xFF000000;// Mask for the highest byte (0xFF000000)
    
    uint32_t invHighByte = (~n & mask);// Extract the highest byte and invert its bits

    // Preserve the lower bytes and combine with the inverted high byte
    return (n & ~mask) | invHighByte;
}

int main() {
    uint32_t n;

    printf("Enter a 32-bit unsigned integer: ");
    if (scanf("%u", &n) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    uint32_t result = inv_high_byte(n);
    printf("Result: ");
    printf("%u\n", result);
    
    return 0;
}
