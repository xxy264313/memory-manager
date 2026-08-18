#include "../include/memory_manager.h"

int main() {
    int M, Q;
    scanf("%d %d", &M, &Q);
    init_memory(M);

    for (int i = 0; i < Q; i++) {
        char cmd[20];
        scanf("%s", cmd);

        if (strcmp(cmd, "ALLOC") == 0) {
            char id[16];
            int size;
            scanf("%s %d", id, &size);
            alloc_block(id, size);
        } 
        else if (strcmp(cmd, "FREE") == 0) {
            char id[16];
            scanf("%s", id);
            free_block(id);
        } 
        else if (strcmp(cmd, "QUERY") == 0) {
            char id[16];
            scanf("%s", id);
            query_block(id);
        } 
        else if (strcmp(cmd, "DUMP") == 0) {
            dump_memory();
        } 
        else if (strcmp(cmd, "COMPACT") == 0) {
            compact_memory();
        }
    }

    // 防止内存泄漏
    free_all_memory();
    return 0;
}