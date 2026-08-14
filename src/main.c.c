#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: 后续在这里定义链表结构体

int main() {
    int M, Q;
    scanf("%d %d", &M, &Q);
    
    // TODO: 在这里初始化内存（初始时只有一块空闲内存 [0, M-1]）
    
    for (int i = 0; i < Q; i++) {
        char cmd[20];
        scanf("%s", cmd);
        
        if (strcmp(cmd, "ALLOC") == 0) {
            char id[16];
            int size;
            scanf("%s %d", id, &size);
            // TODO: 调用你后面写的分配函数
            printf("ALLOCATED %s 0\n", id); // 临时占位，方便测试
        } 
        else if (strcmp(cmd, "FREE") == 0) {
            char id[16];
            scanf("%s", id);
            // TODO
        } 
        else if (strcmp(cmd, "DUMP") == 0) {
            // TODO
            printf("ALLOCATED_BLOCKS 0\nFREE_BLOCKS 1\n0 %d\n", M);
        } 
        else if (strcmp(cmd, "COMPACT") == 0) {
            // TODO
            printf("COMPACTED 0\n");
        }
    }
    return 0;
}