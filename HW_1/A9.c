#include <stdio.h>
#include <stdint.h>

struct pack_array {
    uint32_t array;       
    uint32_t count0 : 8;  
    uint32_t count1 : 8;  
};

// Function to pack the array into the given structure
void array2struct(int arr[], struct pack_array *result) {
    result->array = 0; 
    result->count0 = 0;
    result->count1 = 0;

    for (int i = 0; i < 32; ++i) {
        if (arr[i] != 0 && arr[i] != 1) {
            fprintf(stderr, "Error: Invalid input value at position %d. Only 0 and 1 are allowed.\n", i);
            return;
        }
        // Shift and set the bits in the packed representation
        result->array |= (arr[i] << (31 - i));

        if (arr[i] == 0) {
            ++result->count0;
        } else {
            ++result->count1;
        }
    }
}

int main() {
    int input_array[32];
    struct pack_array result;

    printf("Enter 32 elements (0 or 1) separated by spaces:\n");

    for (int i = 0; i < 32; ++i) {
        if (scanf("%d", &input_array[i]) != 1) {
            fprintf(stderr, "Error: Failed to read input at position %d.\n", i);
            return 1;
        }

        if (input_array[i] != 0 && input_array[i] != 1) {
            fprintf(stderr, "Error: Invalid input value (must be 0 or 1) at position %d.\n", i);
            return 1;
        }
    }

    array2struct(input_array, &result);

    printf("Packed array: %u\n", result.array);
    printf("Count of 0s: %u\n", result.count0);
    printf("Count of 1s: %u\n", result.count1);

//	printf("%u %u %u\n", result.array, result.count0, result.count1);

    return 0;
}
