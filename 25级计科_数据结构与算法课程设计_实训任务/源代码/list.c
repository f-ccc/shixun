/*
 * list.c - 双向链表实现
 */
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DLinkedList* list_init(void) {
    DLinkedList *list;
    list = (DLinkedList*)malloc(sizeof(DLinkedList));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

int list_insert(DLinkedList *list, StudentRecord rec) {
    DListNode *node;
    if (!list) return RES_ERR;
    node = (DListNode*)malloc(sizeof(DListNode));
    if (!node) return RES_ERR;
    node->data = rec;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
    return RES_OK;
}

int list_delete(DLinkedList *list, const char *sid, const char *cid) {
    DListNode *cur;
    if (!list || !list->head) return RES_ERR;
    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.student_id, sid) == 0 &&
            strcmp(cur->data.course_id, cid) == 0) {
            if (cur->prev)
                cur->prev->next = cur->next;
            else
                list->head = cur->next;
            if (cur->next)
                cur->next->prev = cur->prev;
            else
                list->tail = cur->prev;
            free(cur);
            list->size--;
            return RES_OK;
        }
        cur = cur->next;
    }
    return RES_NOT_FOUND;
}

int list_update(DLinkedList *list, const char *sid, const char *cid, StudentRecord new_rec) {
    DListNode *node;
    (void)cid;
    node = list_find_by_id(list, sid);
    if (!node) return RES_NOT_FOUND;
    strcpy(new_rec.student_id, node->data.student_id);
    strcpy(new_rec.course_id, node->data.course_id);
    node->data = new_rec;
    return RES_OK;
}

DListNode* list_find_by_id(DLinkedList *list, const char *sid) {
    DListNode *cur;
    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.student_id, sid) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

DListNode* list_find_by_name(DLinkedList *list, const char *name) {
    DListNode *cur;
    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

DListNode* list_find_by_course(DLinkedList *list, const char *cname) {
    DListNode *cur;
    cur = list->head;
    while (cur) {
        if (strcmp(cur->data.course_name, cname) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

void list_traverse(DLinkedList *list, VisitFunc visit) {
    DListNode *cur;
    cur = list->head;
    while (cur) {
        visit(&cur->data);
        cur = cur->next;
    }
}

int list_to_array(DLinkedList *list, StudentRecord *arr, int max_size) {
    DListNode *cur;
    int count;
    cur = list->head;
    count = 0;
    while (cur && count < max_size) {
        arr[count++] = cur->data;
        cur = cur->next;
    }
    return count;
}

static void swap_records(StudentRecord *a, StudentRecord *b) {
    StudentRecord tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void list_sort_by_score(DLinkedList *list, int ascending) {
    int i, j, n;
    StudentRecord *arr;
    DListNode *cur;
    if (!list || list->size <= 1) return;
    n = list->size;
    arr = (StudentRecord*)malloc(n * sizeof(StudentRecord));
    list_to_array(list, arr, n);
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (ascending) {
                if (arr[j].score > arr[j + 1].score)
                    swap_records(&arr[j], &arr[j + 1]);
            } else {
                if (arr[j].score < arr[j + 1].score)
                    swap_records(&arr[j], &arr[j + 1]);
            }
        }
    }
    cur = list->head;
    for (i = 0; i < n && cur; i++) {
        cur->data = arr[i];
        cur = cur->next;
    }
    free(arr);
}

void list_destroy(DLinkedList *list) {
    DListNode *cur, *next;
    if (!list) return;
    cur = list->head;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

int list_size(DLinkedList *list) {
    return list ? list->size : 0;
}
