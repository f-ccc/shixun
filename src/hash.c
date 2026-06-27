/* ============================================================
 * hash.c - Hash Table Implementation
 *
 * Separate chaining (singly linked list per bucket).
 * Insert uses head insertion for O(1). Find/delete
 * traverse the bucket chain linearly.
 * ============================================================ */

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashTable* hash_init(void) {
    int i;
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;
    for (i = 0; i < HASH_TABLE_SIZE; i++) table->buckets[i] = NULL;
    table->size = 0;
    return table;
}

int hash_func(const char *student_id) {
    int key = 0, i, len = (int)strlen(student_id);
    for (i = (len > 6) ? len - 6 : 0; i < len; i++)
        if (student_id[i] >= '0' && student_id[i] <= '9') key = key * 10 + (student_id[i] - '0');
    return key % HASH_TABLE_SIZE;
}

int hash_insert(HashTable *table, StudentRecord rec) {
    int index;
    HashNode *cur, *node;
    if (!table) return RES_ERR;
    index = hash_func(rec.student_id);
    cur = table->buckets[index];
    while (cur) { if (strcmp(cur->data.student_id, rec.student_id) == 0) return RES_DUP; cur = cur->next; }
    node = (HashNode*)malloc(sizeof(HashNode));
    if (!node) return RES_ERR;
    node->data = rec; node->next = table->buckets[index];
    table->buckets[index] = node; table->size++;
    return RES_OK;
}

int hash_delete(HashTable *table, const char *sid, const char *cid) {
    int index;
    HashNode *cur, *prev = NULL;
    if (!table) return RES_ERR;
    index = hash_func(sid);
    cur = table->buckets[index];
    while (cur) {
        if (strcmp(cur->data.student_id, sid) == 0 && strcmp(cur->data.course_id, cid) == 0) {
            if (prev) prev->next = cur->next;
            else table->buckets[index] = cur->next;
            free(cur); table->size--;
            return RES_OK;
        }
        prev = cur; cur = cur->next;
    }
    return RES_NOT_FOUND;
}

HashNode* hash_find(HashTable *table, const char *sid) {
    int index; HashNode *cur;
    if (!table) return NULL;
    index = hash_func(sid);
    cur = table->buckets[index];
    while (cur) { if (strcmp(cur->data.student_id, sid) == 0) return cur; cur = cur->next; }
    return NULL;
}

void hash_traverse(HashTable *table, VisitFunc visit) {
    int i; HashNode *cur;
    if (!table) return;
    for (i = 0; i < HASH_TABLE_SIZE; i++)
        for (cur = table->buckets[i]; cur; cur = cur->next) visit(&cur->data);
}

int hash_to_array(HashTable *table, StudentRecord *arr, int max_size) {
    int i, count = 0; HashNode *cur;
    if (!table) return 0;
    for (i = 0; i < HASH_TABLE_SIZE && count < max_size; i++)
        for (cur = table->buckets[i]; cur && count < max_size; cur = cur->next)
            arr[count++] = cur->data;
    return count;
}

static void swap_rec(StudentRecord *a, StudentRecord *b) { StudentRecord t = *a; *a = *b; *b = t; }

int hash_sort_by_score(HashTable *table, StudentRecord *arr, int ascending) {
    int i, j, n = hash_to_array(table, arr, 100000);
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if ((ascending && arr[j].score > arr[j+1].score) || (!ascending && arr[j].score < arr[j+1].score))
                swap_rec(&arr[j], &arr[j+1]);
    return n;
}

int hash_size(HashTable *table) { return table ? table->size : 0; }

void hash_destroy(HashTable *table) {
    int i; HashNode *cur, *next;
    if (!table) return;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        cur = table->buckets[i];
        while (cur) { next = cur->next; free(cur); cur = next; }
    }
    free(table);
}
