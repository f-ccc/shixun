/*
 * operations.c - 统一操作接口实现
 */
#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DataManager* dm_init(DataStructureType type) {
    DataManager *dm = (DataManager*)malloc(sizeof(DataManager));
    if (!dm) return NULL;
    dm->type = type;
    dm->list = NULL;
    dm->avl = NULL;
    dm->hash = NULL;

    switch (type) {
        case DS_LIST:
            dm->list = list_init();
            break;
        case DS_AVL:
            dm->avl = avl_init();
            break;
        case DS_HASH:
            dm->hash = hash_init();
            break;
    }
    return dm;
}

int dm_insert(DataManager *dm, StudentRecord rec) {
    if (!dm) return ERROR;
    switch (dm->type) {
        case DS_LIST: return list_insert(dm->list, rec);
        case DS_AVL:  return avl_insert(dm->avl, rec);
        case DS_HASH: return hash_insert(dm->hash, rec);
    }
    return ERROR;
}

int dm_delete(DataManager *dm, const char *sid, const char *cid) {
    if (!dm) return ERROR;
    switch (dm->type) {
        case DS_LIST: return list_delete(dm->list, sid, cid);
        case DS_AVL:  return avl_delete(dm->avl, sid, cid);
        case DS_HASH: return hash_delete(dm->hash, sid, cid);
    }
    return ERROR;
}

int dm_update(DataManager *dm, const char *sid, const char *cid, StudentRecord new_rec) {
    /* 先删除后插入 */
    int ret = dm_delete(dm, sid, cid);
    if (ret != OK) return ret;
    /* 保持学号和课程编号不变 */
    strcpy(new_rec.student_id, sid);
    strcpy(new_rec.course_id, cid);
    return dm_insert(dm, new_rec);
}

int dm_find(DataManager *dm, const char *sid, StudentRecord *result) {
    if (!dm || !result) return ERROR;
    switch (dm->type) {
        case DS_LIST: {
            DListNode *node = list_find_by_id(dm->list, sid);
            if (!node) return NOT_FOUND;
            *result = node->data;
            return OK;
        }
        case DS_AVL: {
            AVLNode *node = avl_find(dm->avl, sid);
            if (!node) return NOT_FOUND;
            *result = node->data;
            return OK;
        }
        case DS_HASH: {
            HashNode *node = hash_find(dm->hash, sid);
            if (!node) return NOT_FOUND;
            *result = node->data;
            return OK;
        }
    }
    return ERROR;
}

void dm_traverse(DataManager *dm, VisitFunc visit) {
    if (!dm) return;
    switch (dm->type) {
        case DS_LIST: list_traverse(dm->list, visit); break;
        case DS_AVL:  avl_traverse(dm->avl, visit); break;
        case DS_HASH: hash_traverse(dm->hash, visit); break;
    }
}

int dm_sort_by_score(DataManager *dm, StudentRecord *arr, int ascending) {
    if (!dm) return 0;
    switch (dm->type) {
        case DS_LIST: {
            int n = list_size(dm->list);
            if (n <= 0) return 0;
            list_to_array(dm->list, arr, n);
            list_sort_by_score(dm->list, ascending);
            list_to_array(dm->list, arr, n);
            return n;
        }
        case DS_AVL:
            return avl_sort_by_score(dm->avl, arr, ascending);
        case DS_HASH:
            return hash_sort_by_score(dm->hash, arr, ascending);
    }
    return 0;
}

int dm_size(DataManager *dm) {
    if (!dm) return 0;
    switch (dm->type) {
        case DS_LIST: return list_size(dm->list);
        case DS_AVL:  return avl_size(dm->avl);
        case DS_HASH: return hash_size(dm->hash);
    }
    return 0;
}

int dm_to_array(DataManager *dm, StudentRecord *arr, int max_size) {
    if (!dm) return 0;
    switch (dm->type) {
        case DS_LIST: return list_to_array(dm->list, arr, max_size);
        case DS_AVL:  return avl_inorder_to_array(dm->avl->root, arr, 0);
        case DS_HASH: return hash_to_array(dm->hash, arr, max_size);
    }
    return 0;
}

void dm_destroy(DataManager *dm) {
    if (!dm) return;
    if (dm->list) list_destroy(dm->list);
    if (dm->avl) avl_destroy(dm->avl);
    if (dm->hash) hash_destroy(dm->hash);
    free(dm);
}

void dm_insert_all(DataManager *dm_list, DataManager *dm_avl, DataManager *dm_hash, StudentRecord rec) {
    if (dm_list) list_insert(dm_list->list, rec);
    if (dm_avl) avl_insert(dm_avl->avl, rec);
    if (dm_hash) hash_insert(dm_hash->hash, rec);
}

void dm_delete_all(DataManager *dm_list, DataManager *dm_avl, DataManager *dm_hash,
                   const char *sid, const char *cid) {
    if (dm_list) list_delete(dm_list->list, sid, cid);
    if (dm_avl) avl_delete(dm_avl->avl, sid, cid);
    if (dm_hash) hash_delete(dm_hash->hash, sid, cid);
}
