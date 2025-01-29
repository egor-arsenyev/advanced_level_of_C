#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    void *address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

size_t totalMemoryUsage(list *head) {
    size_t total = 0;
    list *current = head;
    
    while (current != NULL) {
        total += current->size;
        current = current->next;
    }
    
    return total;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("0\n");
        return 0;
    }

    list *head = NULL;
    list *tail = NULL;

    for (int i = 0; i < n; i++) {
        unsigned long long addr;
        size_t size;
        
        if (scanf("%llu %zu", &addr, &size) != 2) {
            while (head) {
                list *tmp = head;
                head = head->next;
                free(tmp);
            }
            printf("0\n");
            return 0;
        }

        list *node = malloc(sizeof(list));
        if (!node) {
            while (head) {
                list *tmp = head;
                head = head->next;
                free(tmp);
            }
            printf("0\n");
            return 0;
        }

        node->address = (void *)(uintptr_t)addr;
        node->size = size;
        node->next = NULL;

        if (!head) head = tail = node;
        else tail = tail->next = node;
    }

    printf("%zu\n", totalMemoryUsage(head));

    while (head) {
        list *tmp = head;
        head = head->next;
        free(tmp);
    }

    return 0;
}
