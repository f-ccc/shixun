/*
 * hash.h - 哈希表模块接口（链地址法）
 */
#ifndef HASH_H
#define HASH_H
#include "record.h"

#define HASH_TABLE_SIZE 997  /* 素数，减少冲突 */

/* 哈希表节点 */
typedef struct HashNode {
    StudentRecord data;
    struct HashNode *next;
} HashNode;

/* 哈希表结构 */
typedef struct {
    HashNode *buckets[HASH_TABLE_SIZE];
    int size;
} HashTable;

/* 初始化哈希表 */
HashTable* hash_init(void);

/* 哈希函数：取学号后6位模表大小 */
int hash_func(const char *student_id);

/* 插入记录 */
int hash_insert(HashTable *table, StudentRecord rec);

/* 删除指定记录 */
int hash_delete(HashTable *table, const char *sid, const char *cid);

/* 查找（按学号） */
HashNode* hash_find(HashTable *table, const char *sid);

/* 遍历所有记录 */
void hash_traverse(HashTable *table, VisitFunc visit);

/* 收集所有记录到数组 */
int hash_to_array(HashTable *table, StudentRecord *arr, int max_size);

/* 导出并按成绩排序 */
int hash_sort_by_score(HashTable *table, StudentRecord *arr, int ascending);

/* 获取大小 */
int hash_size(HashTable *table);

/* 销毁哈希表 */
void hash_destroy(HashTable *table);

#endif
