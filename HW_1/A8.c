#include <stdio.h>

// Function to calculate the number of weights needed
int calculate_weights(int weight) {
    int count = 0;       // Counter for the number of weights used
    int power = 1;       // Current power of 3 (1, 3, 9, 27, ...)
    
    while (weight > 0) {
        int remainder = weight % 3;
        
        if (remainder == 2) {
            if (power > 1000000 / 3) {
                return -1; // Overflow check
            }
            weight = weight / 3 + 1; // Move the balance up to the next ternary digit
            count++;
        }
        else if (remainder == 1) {
            count++;
            weight /= 3; // Move to the next higher power of 3
        }
        // Remainder is 0 - no weight is needed for this power
        else {
            weight /= 3; // Move to the next higher power of 3
        }
        // Check if we exceed the maximum weight limit (moving beyond valid bounds)
        if (power > 1000000) {
            return -1;
        }
        power *= 3; // Move to the next power of 3
    }
    return count;
}

int main() {
    int weight;

    printf("Enter the mass in grams: ");
    if (scanf("%d", &weight) != 1 || weight <= 0 || weight > 1000000) {
        printf("-1\n"); // Invalid input
        return 0;
    }

    int result = calculate_weights(weight);

    printf("%d\n", result);

    return 0;
}
