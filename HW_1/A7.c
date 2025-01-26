#include <stdio.h>

int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    
    int singleNumber = 0;
    for (int i = 0; i < n; ++i) {
        int x;
        if (scanf("%d", &x) != 1) {
            return 1;
        }
        singleNumber ^= x;
    }
    
    printf("%d\n", singleNumber);
    return 0;
}
