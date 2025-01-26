#include <stdio.h>

int main() {
    int N;
    char c;

    scanf("%d", &N);

    while (1) {
        c = getchar();
        if (c == '.') { 
            putchar(c); 
            break;
        } else if (c >= 'a' && c <= 'z') {
            putchar('a' + (c - 'a' + N) % 26);
        } else if (c >= 'A' && c <= 'Z') {
            putchar('A' + (c - 'A' + N) % 26);
        } else if (c == ' ') {
            putchar(c);
        }
    }

    return 0;
}
