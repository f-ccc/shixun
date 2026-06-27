/* ============================================================
 * benchmark.c - Performance Benchmark Implementation
 *
 * Uses clock() for timing (ms precision). Each operation
 * runs on all n records; total / n = per-op average.
 * Memory estimated via sizeof * node_count.
 * ============================================================ */

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
    return (double)clock() / CLOCKS_PER_SEC * 1000.0;
}

static void* create_empty(DataStructureType type) {
    switch (type) { case DS_LIST: return list_init(); case DS_AVL: return avl_init(); case DS_HASH: return hash_init(); }
    return NULL;
}

static void destroy_all(void *ds, DataStructureType type) {
    switch (type) { case DS_LIST: list_destroy(ds); break; case DS_AVL: avl_destroy(ds); break; case DS_HASH: hash_destroy(ds); break; }
}

void run_benchmark(int data_size) {
    StudentRecord *data;
    int t, i;
    void *ds;
    double t_ins, t_find, t_del, t_trav, t_sort;
    size_t mem;
    DataStructureType types[] = {DS_LIST, DS_AVL, DS_HASH};
    const char *names[] = {"双向链表", "AVL 树", "哈希表"};

    data = (StudentRecord*)malloc(data_size * sizeof(StudentRecord));
    generate_records(data, data_size);

    printf("\n===============================================================\n");
    printf("  性能测试报告 (数据规模: %d 条)\n", data_size);
    printf("===============================================================\n");
    printf("+-----------+-------------+----------+----------+----------+\n");
    printf("| 操作      | 数据结构    | 耗时(ms) | 内存(MB) | 单次(us) |\n");
    printf("+-----------+-------------+----------+----------+----------+\n");

    for (t = 0; t < 3; t++) {
        ds = create_empty(types[t]);

        /* 插入 */
        t_ins = get_time_ms();
        for (i = 0; i < data_size; i++) {
            switch (types[t]) { case DS_LIST: list_insert(ds, data[i]); break; case DS_AVL: avl_insert(ds, data[i]); break; case DS_HASH: hash_insert(ds, data[i]); break; }
        }
        t_ins = get_time_ms() - t_ins;

        /* 查找 */
        t_find = get_time_ms();
        for (i = 0; i < data_size; i++) {
            switch (types[t]) { case DS_LIST: list_find_by_id(ds, data[i].student_id); break; case DS_AVL: avl_find(ds, data[i].student_id); break; case DS_HASH: hash_find(ds, data[i].student_id); break; }
        }
        t_find = get_time_ms() - t_find;

        /* 遍历 */
        t_trav = get_time_ms();
        { StudentRecord *tmp = malloc(data_size * sizeof(StudentRecord));
          switch (types[t]) { case DS_LIST: list_to_array(ds, tmp, data_size); break; case DS_AVL: avl_inorder_to_array(((AVLTree*)ds)->root, tmp, 0); break; case DS_HASH: hash_to_array(ds, tmp, data_size); break; }
          free(tmp); }
        t_trav = get_time_ms() - t_trav;

        /* 排序 */
        { StudentRecord *tmp = malloc(data_size * sizeof(StudentRecord));
          t_sort = get_time_ms();
          switch (types[t]) { case DS_LIST: list_sort_by_score(ds, 1); break; case DS_AVL: avl_sort_by_score(ds, tmp, 1); break; case DS_HASH: hash_sort_by_score(ds, tmp, 1); break; }
          t_sort = get_time_ms() - t_sort;
          free(tmp); }

        /* 内存 */
        mem = (types[t] == DS_LIST) ? sizeof(DLinkedList) + ((DLinkedList*)ds)->size * sizeof(DListNode) :
              (types[t] == DS_AVL)  ? sizeof(AVLTree)     + ((AVLTree*)ds)->size     * sizeof(AVLNode) :
                                       sizeof(HashTable)   + ((HashTable*)ds)->size   * sizeof(HashNode);

        printf("| 插入      | %-11s | %8.3f | %8.2f | %8.1f |\n", names[t], t_ins, (double)mem/1048576.0, t_ins/data_size*1000.0);
        printf("| 查找      | %-11s | %8.3f | %8.2f | %8.1f |\n", names[t], t_find, (double)mem/1048576.0, t_find/data_size*1000.0);
        printf("| 遍历      | %-11s | %8.3f | %8.2f | %8s |\n", names[t], t_trav, (double)mem/1048576.0, "");
        printf("| 排序      | %-11s | %8.3f | %8.2f | %8s |\n", names[t], t_sort, (double)mem/1048576.0, "");

        /* 删除 */
        t_del = get_time_ms();
        for (i = 0; i < data_size; i++) {
            switch (types[t]) { case DS_LIST: list_delete(ds, data[i].student_id, data[i].course_id); break; case DS_AVL: avl_delete(ds, data[i].student_id, data[i].course_id); break; case DS_HASH: hash_delete(ds, data[i].student_id, data[i].course_id); break; }
        }
        t_del = get_time_ms() - t_del;
        printf("| 删除      | %-11s | %8.3f | %8.2f | %8.1f |\n", names[t], t_del, (double)mem/1048576.0, t_del/data_size*1000.0);
        printf("+-----------+-------------+----------+----------+----------+\n");
        destroy_all(ds, types[t]);
    }

    printf("\n  理论时间复杂度对比:\n");
    printf("  +-----------+------+--------+--------+\n");
    printf("  | 操作      | 链表 | AVL树  | 哈希表 |\n");
    printf("  +-----------+------+--------+--------+\n");
    printf("  | 插入      | O(1) | O(logn)| O(1)   |\n");
    printf("  | 查找      | O(n) | O(logn)| O(1)   |\n");
    printf("  | 删除      | O(n) | O(logn)| O(1)   |\n");
    printf("  +-----------+------+--------+--------+\n");
    free(data);
    printf("\n  性能测试完成。\n");
}
