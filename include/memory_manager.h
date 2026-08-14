#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Block {
    char id[16];
    int start;
    int size;
    struct Block *prev;
    struct Block *next;
} Block;

extern Block *alloc_head;
extern Block *free_head;

Block* create_block(int start, int size, const char *id);
void insert_sorted(Block **head, Block *node);
void delete_node(Block **head, Block *node);

void init_memory(int M);
void alloc_block(const char *id, int size);
void free_block(const char *id);
void query_block(const char *id);
void dump_memory();
void compact_memory();

#endif
