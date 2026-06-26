/*
 * operations.h - 统一操作接口模块
 */
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "record.h"
#include "list.h"
#include "avl.h"
#include "hash.h"

/* 统一数据结构管理器 */
typedef struct {
    DataStructureType type;
    DLinkedList *list;
    AVLTree *avl;
    HashTable *hash;
} DataManager;

/* 初始化数据管理器 */
DataManager* dm_init(DataStructureType type);

/* 统一插入 */
int dm_insert(DataManager *dm, StudentRecord rec);

/* 统一删除 */
int dm_delete(DataManager *dm, const char *sid, const char *cid);

/* 统一更新 */
int dm_update(DataManager *dm, const char *sid, const char *cid, StudentRecord new_rec);

/* 统一查找（按学号，返回数据副本） */
int dm_find(DataManager *dm, const char *sid, StudentRecord *result);

/* 统一遍历 */
void dm_traverse(DataManager *dm, VisitFunc visit);

/* 统一按成绩排序到数组 */
int dm_sort_by_score(DataManager *dm, StudentRecord *arr, int ascending);

/* 统一获取当前记录数 */
int dm_size(DataManager *dm);

/* 统一获取所有记录到数组 */
int dm_to_array(DataManager *dm, StudentRecord *arr, int max_size);

/* 销毁所有数据结构 */
void dm_destroy(DataManager *dm);

/* 同步插入到所有结构（用于多结构对比） */
void dm_insert_all(DataManager *dm_list, DataManager *dm_avl, DataManager *dm_hash, StudentRecord rec);

/* 同步删除 */
void dm_delete_all(DataManager *dm_list, DataManager *dm_avl, DataManager *dm_hash, const char *sid, const char *cid);

/* 切换数据结构类型（保留数据） */
void dm_switch_type(DataManager *dm, DataStructureType new_type);

#endif
