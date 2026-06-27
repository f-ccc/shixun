/* ============================================================
 * operations.h - Unified Operation Interface
 *
 * DataManager wraps list/avl/hash behind a single API.
 * Client code uses dm_insert/dm_find/... without knowing
 * which backend structure is active.
 * ============================================================ */

#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "record.h"
#include "list.h"
#include "avl.h"
#include "hash.h"

typedef struct {
    DataStructureType type;
    DLinkedList *list;
    AVLTree *avl;
    HashTable *hash;
} DataManager;

DataManager* dm_init(DataStructureType type);
int dm_insert(DataManager *dm, StudentRecord rec);
int dm_delete(DataManager *dm, const char *sid, const char *cid);
int dm_update(DataManager *dm, const char *sid, const char *cid, StudentRecord new_rec);
int dm_find(DataManager *dm, const char *sid, StudentRecord *result);
void dm_traverse(DataManager *dm, VisitFunc visit);
int dm_sort_by_score(DataManager *dm, StudentRecord *arr, int ascending);
int dm_size(DataManager *dm);
int dm_to_array(DataManager *dm, StudentRecord *arr, int max_size);
void dm_destroy(DataManager *dm);

#endif
