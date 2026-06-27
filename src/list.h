/* ============================================================
 * list.h - Doubly Linked List Interface
 *
 * Doubly linked list storing StudentRecord entries.
 * O(1) insert/delete at ends, O(n) linear search.
 * Supports in-place bubble sort by score.
 * ============================================================ */

#ifndef LIST_H
#define LIST_H
#include "record.h"

typedef struct DListNode {
    StudentRecord data;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

typedef struct {
    DListNode *head;
    DListNode *tail;
    int size;
} DLinkedList;

DLinkedList* list_init(void);
int list_insert(DLinkedList *list, StudentRecord rec);
int list_delete(DLinkedList *list, const char *sid, const char *cid);
int list_update(DLinkedList *list, const char *sid, const char *cid, StudentRecord new_rec);
DListNode* list_find_by_id(DLinkedList *list, const char *sid);
DListNode* list_find_by_name(DLinkedList *list, const char *name);
void list_traverse(DLinkedList *list, VisitFunc visit);
void list_sort_by_score(DLinkedList *list, int ascending);
int list_to_array(DLinkedList *list, StudentRecord *arr, int max_size);
void list_destroy(DLinkedList *list);
int list_size(DLinkedList *list);

#endif
