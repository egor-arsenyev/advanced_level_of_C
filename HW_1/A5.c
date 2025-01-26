#include <stdio.h>
#include <stdint.h>

// Function to count the number of set bits in an unsigned 32-bit integer
unsigned int countSetBits(uint32_t n) {
    unsigned int count = 0;
    while (n) {
        count += (n & 1);  // Check the least significant bit
        n >>= 1;
    }           
    return count;
}

int main() {
    uint32_t number;

    printf("Enter a 32-bit unsigned integer: ");
    if (scanf("%u", &number) != 1) { 
        printf("Invalid input.\n");
        return 1;
    }

    printf("Number of set bits: ");
    printf("%u\n", countSetBits(number));   

    return 0;
}
