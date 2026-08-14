#include "../include/memory_manager.h"

Block *alloc_head = NULL;
Block *free_head = NULL;
static int TOTAL_M;

Block* create_block(int start, int size, const char *id) {
    Block *new = (Block*)malloc(sizeof(Block));
    new->start = start;
    new->size = size;
    if (id) strcpy(new->id, id);
    else new->id[0] = '\0';
    new->prev = new->next = NULL;
    return new;
}

void insert_sorted(Block **head, Block *node) {
    if (*head == NULL) { *head = node; return; }
    Block *cur = *head;
    while (cur && cur->start < node->start) cur = cur->next;
    if (cur == NULL) {
        Block *last = *head;
        while (last->next) last = last->next;
        last->next = node;
        node->prev = last;
    } else if (cur == *head) {
        node->next = *head;
        (*head)->prev = node;
        *head = node;
    } else {
        node->prev = cur->prev;
        node->next = cur;
        cur->prev->next = node;
        cur->prev = node;
    }
}

void delete_node(Block **head, Block *node) {
    if (*head == NULL || node == NULL) return;
    if (node->prev) node->prev->next = node->next;
    else *head = node->next;
    if (node->next) node->next->prev = node->prev;
}

void init_memory(int M) {
    TOTAL_M = M;
    free_head = create_block(0, M, NULL);
}

void dump_memory() {
    int count = 0;
    Block *cur = alloc_head;
    while (cur) { count++; cur = cur->next; }
    printf("ALLOCATED_BLOCKS %d\n", count);
    cur = alloc_head;
    while (cur) {
        printf("%s %d %d\n", cur->id, cur->start, cur->size);
        cur = cur->next;
    }
    count = 0;
    cur = free_head;
    while (cur) { count++; cur = cur->next; }
    printf("FREE_BLOCKS %d\n", count);
    cur = free_head;
    while (cur) {
        printf("%d %d\n", cur->start, cur->size);
        cur = cur->next;
    }
}

void query_block(const char *id) {
    Block *cur = alloc_head;
    while (cur) {
        if (strcmp(cur->id, id) == 0) {
            printf("BLOCK %s %d %d\n", id, cur->start, cur->size);
            return;
        }
        cur = cur->next;
    }
    printf("BLOCK_NOT_FOUND %s\n", id);
}

void alloc_block(const char *id, int size) {
    Block *cur = alloc_head;
    while (cur) {
        if (strcmp(cur->id, id) == 0) {
            printf("ALLOC_FAILED %s DUPLICATE\n", id);
            return;
        }
        cur = cur->next;
    }
    Block *best = NULL;
    int min_gap = 1000000001;
    cur = free_head;
    while (cur) {
        if (cur->size >= size && cur->size < min_gap) {
            min_gap = cur->size;
            best = cur;
        }
        cur = cur->next;
    }
    if (best == NULL) {
        printf("ALLOC_FAILED %s NO_SPACE\n", id);
        return;
    }
    int alloc_start = best->start;
    best->start += size;
    best->size -= size;
    if (best->size == 0) {
        delete_node(&free_head, best);
        free(best);
    }
    Block *new_block = create_block(alloc_start, size, id);
    insert_sorted(&alloc_head, new_block);
    printf("ALLOCATED %s %d\n", id, alloc_start);
}

void free_block(const char *id) {
    Block *node = alloc_head;
    while (node) {
        if (strcmp(node->id, id) == 0) break;
        node = node->next;
    }
    if (node == NULL) {
        printf("FREE_FAILED %s NOT_FOUND\n", id);
        return;
    }
    int freed_start = node->start;
    int freed_size = node->size;
    delete_node(&alloc_head, node);
    node->id[0] = '\0';
    insert_sorted(&free_head, node);
    Block *cur = free_head;
    while (cur && cur->next) {
        if (cur->start + cur->size == cur->next->start) {
            cur->size += cur->next->size;
            Block *to_delete = cur->next;
            cur->next = to_delete->next;
            if (to_delete->next) to_delete->next->prev = cur;
            free(to_delete);
        } else {
            cur = cur->next;
        }
    }
    printf("FREED %s %d %d\n", id, freed_start, freed_size);
}

typedef struct {
    char id[16];
    int old_start;
    int new_start;
} MoveLog;

void compact_memory() {
    if (alloc_head == NULL) {
        while (free_head) { Block *tmp = free_head; free_head = free_head->next; free(tmp); }
        free_head = create_block(0, TOTAL_M, NULL);
        printf("COMPACTED 0\n");
        return;
    }
    MoveLog logs[5000];
    int moved_count = 0;
    int current_addr = 0;
    Block *cur = alloc_head;
    while (cur) {
        if (cur->start != current_addr) {
            strcpy(logs[moved_count].id, cur->id);
            logs[moved_count].old_start = cur->start;
            logs[moved_count].new_start = current_addr;
            moved_count++;
            cur->start = current_addr;
        }
        current_addr += cur->size;
        cur = cur->next;
    }
    while (free_head) { Block *tmp = free_head; free_head = free_head->next; free(tmp); }
    free_head = create_block(current_addr, TOTAL_M - current_addr, NULL);
    printf("COMPACTED %d\n", moved_count);
    for (int i = 0; i < moved_count; i++) {
        printf("%s %d %d\n", logs[i].id, logs[i].old_start, logs[i].new_start);
    }
}
