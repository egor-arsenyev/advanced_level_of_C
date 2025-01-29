#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct list {
    uint64_t address;
    size_t size;
    char comment[64];
    struct list *next;
} list;

uint64_t findMaxBlock(list *head) {
    if (head == NULL) {
        return 0; // Return NULL equivalent for empty list
    }

    list *current = head;
    size_t max_size = current->size;
    uint64_t max_address = current->address;

    current = current->next;
    while (current != NULL) {
        if (current->size > max_size) {
            max_size = current->size;
            max_address = current->address;
        }
        current = current->next;
    }

    return max_address;
}

void free_list(list *head) {
    while (head) {
        list *next = head->next;
        free(head);
        head = next;
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    if (n == 0) {
        printf("0\n");
        return 0;
    }

    list *head = NULL, *tail = NULL;

    for (int i = 0; i < n; ++i) {
        list *node = malloc(sizeof(list));
        if (!node) {
            fprintf(stderr, "Memory error\n");
            free_list(head);
            return 1;
        }

        if (scanf("%" SCNu64 " %zu", 
                 &node->address, 
                 &node->size) != 2) {
            fprintf(stderr, "Invalid element\n");
            free(node);
            free_list(head);
            return 1;
        }

        node->next = NULL;
        
        if (!head) 
			head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }

    printf("%" PRIu64 "\n", findMaxBlock(head));
    free_list(head);
    return 0;
}
