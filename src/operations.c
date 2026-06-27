/* ============================================================
 * operations.c - Unified Operation Implementation
 *
 * Delegates each dm_* call to the active backend via
 * a switch on DataManager.type.
 * ============================================================ */

#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DataManager* dm_init(DataStructureType type) {
    DataManager *dm = (DataManager*)malloc(sizeof(DataManager));
    if (!dm) return NULL;
    dm->type = type; dm->list = NULL; dm->avl = NULL; dm->hash = NULL;
    switch (type) {
        case DS_LIST: dm->list = list_init(); break;
        case DS_AVL:  dm->avl = avl_init(); break;
        case DS_HASH: dm->hash = hash_init(); break;
    }
    return dm;
}

int dm_insert(DataManager *dm, StudentRecord rec) {
    if (!dm) return RES_ERR;
    switch (dm->type) {
        case DS_LIST: return list_insert(dm->list, rec);
        case DS_AVL:  return avl_insert(dm->avl, rec);
        case DS_HASH: return hash_insert(dm->hash, rec);
    }
    return RES_ERR;
}

int dm_delete(DataManager *dm, const char *sid, const char *cid) {
    if (!dm) return RES_ERR;
    switch (dm->type) {
        case DS_LIST: return list_delete(dm->list, sid, cid);
        case DS_AVL:  return avl_delete(dm->avl, sid, cid);
        case DS_HASH: return hash_delete(dm->hash, sid, cid);
    }
    return RES_ERR;
}

int dm_update(DataManager *dm, const char *sid, const char *cid, StudentRecord new_rec) {
    int ret = dm_delete(dm, sid, cid);
    if (ret != RES_OK) return ret;
    strcpy(new_rec.student_id, sid); strcpy(new_rec.course_id, cid);
    return dm_insert(dm, new_rec);
}

int dm_find(DataManager *dm, const char *sid, StudentRecord *result) {
    DListNode *ln; AVLNode *an; HashNode *hn;
    if (!dm || !result) return RES_ERR;
    switch (dm->type) {
        case DS_LIST: ln = list_find_by_id(dm->list, sid); if (!ln) return RES_NOT_FOUND; *result = ln->data; return RES_OK;
        case DS_AVL:  an = avl_find(dm->avl, sid); if (!an) return RES_NOT_FOUND; *result = an->data; return RES_OK;
        case DS_HASH: hn = hash_find(dm->hash, sid); if (!hn) return RES_NOT_FOUND; *result = hn->data; return RES_OK;
    }
    return RES_ERR;
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
    int n;
    if (!dm) return 0;
    switch (dm->type) {
        case DS_LIST: n = list_size(dm->list); if (n <= 0) return 0; list_to_array(dm->list, arr, n); list_sort_by_score(dm->list, ascending); return list_to_array(dm->list, arr, n);
        case DS_AVL:  return avl_sort_by_score(dm->avl, arr, ascending);
        case DS_HASH: return hash_sort_by_score(dm->hash, arr, ascending);
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
