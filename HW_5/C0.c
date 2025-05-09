#include <stdio.h>
#include <string.h>

int main() {
    char s[100001]; // Буфер для ввода числа N
    scanf("%100000s", s);
    int len = strlen(s);
    
    if (len < 3) {
        printf("0\n");
        return 0;
    }

    // Инициализация таблицы переходов
    int next_pos[len + 1][10];
    for (int c = 0; c < 10; c++) {
        next_pos[len][c] = -1;
    }
    for (int i = len - 1; i >= 0; i--) {
        for (int c = 0; c < 10; c++) {
            next_pos[i][c] = next_pos[i + 1][c];
        }
        int current_digit = s[i] - '0';
        next_pos[i][current_digit] = i;
    }

    int count = 0;

    // Перебор всех трехзначных чисел от 100 до 999
    for (int num = 100; num <= 999; num++) {
        int a = num / 100;
        int b = (num / 10) % 10;
        int c = num % 10;

        int pos = 0;

        // Проверка первой цифры
        pos = next_pos[pos][a];
        if (pos == -1) continue;
        pos++;

        // Проверка второй цифры
        pos = next_pos[pos][b];
        if (pos == -1) continue;
        pos++;

        // Проверка третьей цифры
        pos = next_pos[pos][c];
        if (pos == -1) continue;

        count++;
    }

    printf("%d\n", count);
    return 0;
}
