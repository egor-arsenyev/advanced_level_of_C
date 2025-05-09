#include <stdio.h>
#include <string.h>

#define MAX_LEN 10001

int main() {
    char s1[MAX_LEN], s2[MAX_LEN];
    fgets(s1, MAX_LEN, stdin);
    fgets(s2, MAX_LEN, stdin);

    // Удаляем символы новой строки
    size_t len1 = strlen(s1);
    if (len1 > 0 && s1[len1 - 1] == '\n') {
        s1[--len1] = '\0';
    }
    size_t len2 = strlen(s2);
    if (len2 > 0 && s2[len2 - 1] == '\n') {
        s2[--len2] = '\0';
    }

    int max_k1 = 0;
    size_t max_possible1 = len1 < len2 ? len1 : len2;
    for (int k = max_possible1; k > 0; k--) {
        if (memcmp(s1, s2 + len2 - k, k) == 0) {
            max_k1 = k;
            break;
        }
    }

    int max_k2 = 0;
    size_t max_possible2 = len1 < len2 ? len1 : len2;
    for (int k = max_possible2; k > 0; k--) {
        if (memcmp(s1 + len1 - k, s2, k) == 0) {
            max_k2 = k;
            break;
        }
    }

    printf("%d %d\n", max_k1, max_k2);
    return 0;
}
