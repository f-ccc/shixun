/*
 * hash.h - 哈希表模块接口（链地址法）
 */
#ifndef HASH_H
#define HASH_H
#include "record.h"

#define HASH_TABLE_SIZE 997

typedef struct HashNode {
    StudentRecord data;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *buckets[HASH_TABLE_SIZE];
    int size;
} HashTable;

HashTable* hash_init(void);
int hash_func(const char *student_id);
int hash_insert(HashTable *table, StudentRecord rec);
int hash_delete(HashTable *table, const char *sid, const char *cid);
HashNode* hash_find(HashTable *table, const char *sid);
void hash_traverse(HashTable *table, VisitFunc visit);
int hash_to_array(HashTable *table, StudentRecord *arr, int max_size);
int hash_sort_by_score(HashTable *table, StudentRecord *arr, int ascending);
int hash_size(HashTable *table);
void hash_destroy(HashTable *table);

#endif
