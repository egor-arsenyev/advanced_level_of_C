#include <stdio.h>

int main() {
    int N;
    scanf("%d ", &N);

    int currentNumber, maxValue, maxCount = 0;

    for (int i = 0; i < N; i++) {
        scanf("%d", &currentNumber);

        if (i == 0 || currentNumber > maxValue) {
            maxValue = currentNumber;
            maxCount = 1;             
        } else if (currentNumber == maxValue) {
            maxCount++;               
        }
    }

    printf("%d\n", maxCount);

    return 0;
}
