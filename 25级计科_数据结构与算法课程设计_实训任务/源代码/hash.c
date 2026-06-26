/*
 * hash.c - 哈希表实现（链地址法解决冲突）
 */
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashTable* hash_init(void) {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        table->buckets[i] = NULL;
    table->size = 0;
    return table;
}

int hash_func(const char *student_id) {
    /* 取学号后6位作为键值 */
    int key = 0;
    int len = strlen(student_id);
    for (int i = (len > 6) ? len - 6 : 0; i < len; i++) {
        if (student_id[i] >= '0' && student_id[i] <= '9')
            key = key * 10 + (student_id[i] - '0');
    }
    return key % HASH_TABLE_SIZE;
}

int hash_insert(HashTable *table, StudentRecord rec) {
    if (!table) return ERROR;
    int index = hash_func(rec.student_id);

    /* 检查是否已存在 */
    HashNode *cur = table->buckets[index];
    while (cur) {
        if (strcmp(cur->data.student_id, rec.student_id) == 0)
            return DUPLICATE;
        cur = cur->next;
    }

    /* 头插法 */
    HashNode *node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return ERROR;
    node->data = rec;
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
    return OK;
}

int hash_delete(HashTable *table, const char *sid, const char *cid) {
    if (!table) return ERROR;
    int index = hash_func(sid);
    HashNode *cur = table->buckets[index];
    HashNode *prev = NULL;

    while (cur) {
        if (strcmp(cur->data.student_id, sid) == 0 &&
            strcmp(cur->data.course_id, cid) == 0) {
            if (prev)
                prev->next = cur->next;
            else
                table->buckets[index] = cur->next;
            free(cur);
            table->size--;
            return OK;
        }
        prev = cur;
        cur = cur->next;
    }
    return NOT_FOUND;
}

HashNode* hash_find(HashTable *table, const char *sid) {
    if (!table) return NULL;
    int index = hash_func(sid);
    HashNode *cur = table->buckets[index];
    while (cur) {
        if (strcmp(cur->data.student_id, sid) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

void hash_traverse(HashTable *table, VisitFunc visit) {
    if (!table) return;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *cur = table->buckets[i];
        while (cur) {
            visit(&cur->data);
            cur = cur->next;
        }
    }
}

int hash_to_array(HashTable *table, StudentRecord *arr, int max_size) {
    if (!table) return 0;
    int count = 0;
    for (int i = 0; i < HASH_TABLE_SIZE && count < max_size; i++) {
        HashNode *cur = table->buckets[i];
        while (cur && count < max_size) {
            arr[count++] = cur->data;
            cur = cur->next;
        }
    }
    return count;
}

static void swap_rec(StudentRecord *a, StudentRecord *b) {
    StudentRecord t = *a;
    *a = *b;
    *b = t;
}

int hash_sort_by_score(HashTable *table, StudentRecord *arr, int ascending) {
    int n = hash_to_array(table, arr, 100000);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (ascending) {
                if (arr[j].score > arr[j + 1].score)
                    swap_rec(&arr[j], &arr[j + 1]);
            } else {
                if (arr[j].score < arr[j + 1].score)
                    swap_rec(&arr[j], &arr[j + 1]);
            }
        }
    }
    return n;
}

int hash_size(HashTable *table) {
    return table ? table->size : 0;
}

void hash_destroy(HashTable *table) {
    if (!table) return;
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *cur = table->buckets[i];
        while (cur) {
            HashNode *next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(table);
}
