#include <stdio.h>
#include <stdint.h>

int extractExp(float input) {
    union {
        float f;
        uint32_t i;
    } data;

    data.f = input;
    int exponent = (data.i >> 23) & 0xFF;

    return exponent;
}

int main() {
    float input;

    if (scanf("%f", &input) != 1) {
        fprintf(stderr, "Error reading input.\n");
        return 1; 
    }

    int exponent = extractExp(input);

    printf("%d\n", exponent);

    return 0;
}
