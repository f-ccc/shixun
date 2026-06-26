/*
 * benchmark.c - 性能测试实现
 */
#include "benchmark.h"
#include "list.h"
#include "avl.h"
#include "hash.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double get_time_ms(void) {
    clock_t t;
    t = clock();
    return (double)t / CLOCKS_PER_SEC * 1000.0;
}

double time_insert(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start, end;
    (void)data;
    start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_insert((DLinkedList*)ds, data[i]); break;
            case DS_AVL:  avl_insert((AVLTree*)ds, data[i]); break;
            case DS_HASH: hash_insert((HashTable*)ds, data[i]); break;
        }
    }
    end = get_time_ms();
    return end - start;
}

double time_find(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start, end;
    (void)data;
    start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_find_by_id((DLinkedList*)ds, data[i].student_id); break;
            case DS_AVL:  avl_find((AVLTree*)ds, data[i].student_id); break;
            case DS_HASH: hash_find((HashTable*)ds, data[i].student_id); break;
        }
    }
    end = get_time_ms();
    return end - start;
}

double time_delete(void *ds, DataStructureType type, StudentRecord *data, int n) {
    int i;
    double start, end;
    (void)data;
    start = get_time_ms();
    for (i = 0; i < n; i++) {
        switch (type) {
            case DS_LIST: list_delete((DLinkedList*)ds, data[i].student_id, data[i].course_id); break;
            case DS_AVL:  avl_delete((AVLTree*)ds, data[i].student_id, data[i].course_id); break;
            case DS_HASH: hash_delete((HashTable*)ds, data[i].student_id, data[i].course_id); break;
        }
    }
    end = get_time_ms();
    return end - start;
}

double time_traverse(void *ds, DataStructureType type, int n) {
    StudentRecord *arr;
    double start, end;
    (void)type;
    arr = (StudentRecord*)malloc(n * sizeof(StudentRecord));
    start = get_time_ms();
    switch (type) {
        case DS_LIST: list_to_array((DLinkedList*)ds, arr, n); break;
        case DS_AVL:  avl_inorder_to_array(((AVLTree*)ds)->root, arr, 0); break;
        case DS_HASH: hash_to_array((HashTable*)ds, arr, n); break;
    }
    end = get_time_ms();
    free(arr);
    return end - start;
}

double time_sort(void *ds, DataStructureType type, int n) {
    StudentRecord *arr;
    double start, end;
    arr = (StudentRecord*)malloc(n * sizeof(StudentRecord));
    start = get_time_ms();
    switch (type) {
        case DS_LIST: {
            list_to_array((DLinkedList*)ds, arr, n);
            list_sort_by_score((DLinkedList*)ds, 1);
            break;
        }
        case DS_AVL:
            avl_sort_by_score((AVLTree*)ds, arr, 1);
            break;
        case DS_HASH:
            hash_sort_by_score((HashTable*)ds, arr, 1);
            break;
    }
    end = get_time_ms();
    free(arr);
    return end - start;
}

size_t estimate_memory(void *ds, DataStructureType type) {
    switch (type) {
        case DS_LIST: {
            DLinkedList *list = (DLinkedList*)ds;
            return sizeof(DLinkedList) + (size_t)list->size * sizeof(DListNode);
        }
        case DS_AVL: {
            AVLTree *tree = (AVLTree*)ds;
            return sizeof(AVLTree) + (size_t)tree->size * sizeof(AVLNode);
        }
        case DS_HASH: {
            HashTable *table = (HashTable*)ds;
            return sizeof(HashTable) + (size_t)table->size * sizeof(HashNode);
        }
    }
    return 0;
}

static void destroy_all(void *ds, DataStructureType type) {
    switch (type) {
        case DS_LIST: list_destroy((DLinkedList*)ds); break;
        case DS_AVL:  avl_destroy((AVLTree*)ds); break;
        case DS_HASH: hash_destroy((HashTable*)ds); break;
    }
}

static void* create_empty(DataStructureType type) {
    switch (type) {
        case DS_LIST: return list_init();
        case DS_AVL:  return avl_init();
        case DS_HASH: return hash_init();
    }
    return NULL;
}

void run_benchmark(int data_size) {
    StudentRecord *data;
    int type_count, t, i;
    DataStructureType types[3];
    void *ds;
    double t_insert, t_find, t_trav, t_sort, t_delete;
    double avg_insert, avg_find, avg_delete, mem_mb;
    size_t mem;
    printf("\n=================================================================\n");
    printf("  \u6027\u80fd\u6d4b\u8bd5\u62a5\u544a (\u6570\u636e\u89c4\u6a21: %d \u6761)\n", data_size);
    printf("=================================================================\n");
    data = (StudentRecord*)malloc(data_size * sizeof(StudentRecord));
    generate_records(data, data_size);
    type_count = 3;
    types[0] = DS_LIST;
    types[1] = DS_AVL;
    types[2] = DS_HASH;
    printf("\n+------------+--------------+------------+------------+------------+\n");
    printf("| \u64cd\u4f5c       | \u6570\u636e\u7ed3\u6784     | \u8017\u65f6(ms)   | \u5185\u5b58(MB)   | \u5355\u6b21(us)   |\n");
    printf("+------------+--------------+------------+------------+------------+\n");
    for (t = 0; t < type_count; t++) {
        ds = create_empty(types[t]);
        t_insert = time_insert(ds, types[t], data, data_size);
        avg_insert = t_insert / data_size * 1000.0;
        t_find = time_find(ds, types[t], data, data_size);
        avg_find = t_find / data_size * 1000.0;
        t_trav = time_traverse(ds, types[t], data_size);
        t_sort = time_sort(ds, types[t], data_size);
        mem = estimate_memory(ds, types[t]);
        mem_mb = (double)mem / (1024.0 * 1024.0);
        printf("| \u63d2\u5165       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               types[t] == DS_LIST ? "\u53cc\u5411\u94fe\u8868" : types[t] == DS_AVL ? "AVL\u6811" : "\u54c8\u5e0c\u8868",
               t_insert, mem_mb, avg_insert);
        printf("| \u67e5\u627e       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               types[t] == DS_LIST ? "\u53cc\u5411\u94fe\u8868" : types[t] == DS_AVL ? "AVL\u6811" : "\u54c8\u5e0c\u8868",
               t_find, mem_mb, avg_find);
        printf("| \u904d\u5386       | %-12s | %10.3f | %10.2f | %10s |\n",
               types[t] == DS_LIST ? "\u53cc\u5411\u94fe\u8868" : types[t] == DS_AVL ? "AVL\u6811" : "\u54c8\u5e0c\u8868",
               t_trav, mem_mb, "");
        printf("| \u6392\u5e8f       | %-12s | %10.3f | %10.2f | %10s |\n",
               types[t] == DS_LIST ? "\u53cc\u5411\u94fe\u8868" : types[t] == DS_AVL ? "AVL\u6811" : "\u54c8\u5e0c\u8868",
               t_sort, mem_mb, "");
        destroy_all(ds, types[t]);
        ds = create_empty(types[t]);
        for (i = 0; i < data_size; i++) {
            switch (types[t]) {
                case DS_LIST: list_insert((DLinkedList*)ds, data[i]); break;
                case DS_AVL:  avl_insert((AVLTree*)ds, data[i]); break;
                case DS_HASH: hash_insert((HashTable*)ds, data[i]); break;
            }
        }
        t_delete = time_delete(ds, types[t], data, data_size);
        avg_delete = t_delete / data_size * 1000.0;
        printf("| \u5220\u9664       | %-12s | %10.3f | %10.2f | %10.1f |\n",
               types[t] == DS_LIST ? "\u53cc\u5411\u94fe\u8868" : types[t] == DS_AVL ? "AVL\u6811" : "\u54c8\u5e0c\u8868",
               t_delete, mem_mb, avg_delete);
        printf("+------------+--------------+------------+------------+------------+\n");
        destroy_all(ds, types[t]);
    }
    printf("\n  \u7406\u8bba\u65f6\u95f4\u590d\u6742\u5ea6\u5bf9\u6bd4:\n");
    printf("  +------------+------------+------------+------------+\n");
    printf("  | \u64cd\u4f5c       | \u53cc\u5411\u94fe\u8868   | AVL\u6811      | \u54c8\u5e0c\u8868     |\n");
    printf("  +------------+------------+------------+------------+\n");
    printf("  | \u63d2\u5165       | O(1)       | O(log n)   | O(1)\u5e73\u5747   |\n");
    printf("  | \u67e5\u627e       | O(n)       | O(log n)   | O(1)\u5e73\u5747   |\n");
    printf("  | \u5220\u9664       | O(n)       | O(log n)   | O(1)\u5e73\u5747   |\n");
    printf("  | \u904d\u5386       | O(n)       | O(n)       | O(n)       |\n");
    printf("  | \u6392\u5e8f       | O(nlog n)  | O(n)\u4e2d\u5e8f   | O(nlog n)  |\n");
    printf("  +------------+------------+------------+------------+\n");
    free(data);
    printf("\n  \u6027\u80fd\u6d4b\u8bd5\u5b8c\u6210\u3002\n");
}
