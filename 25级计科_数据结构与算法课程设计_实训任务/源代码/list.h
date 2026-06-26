/*
 * list.h - 双向链表模块接口
 */
#ifndef LIST_H
#define LIST_H
#include "record.h"

/* 双向链表节点 */
typedef struct DListNode {
    StudentRecord data;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

/* 双向链表结构 */
typedef struct {
    DListNode *head;    /* 头节点 */
    DListNode *tail;    /* 尾节点 */
    int size;           /* 节点数量 */
} DLinkedList;

/* 初始化空链表 */
DLinkedList* list_init(void);

/* 插入记录（尾部插入） */
int list_insert(DLinkedList *list, StudentRecord rec);

/* 根据学号+课程编号删除记录 */
int list_delete(DLinkedList *list, const char *sid, const char *cid);

/* 根据学号+课程编号修改记录 */
int list_update(DLinkedList *list, const char *sid, const char *cid, StudentRecord new_rec);

/* 按学号查找（精确匹配） */
DListNode* list_find_by_id(DLinkedList *list, const char *sid);

/* 按姓名查找（返回第一个匹配） */
DListNode* list_find_by_name(DLinkedList *list, const char *name);

/* 按课程名称查找（返回第一个匹配） */
DListNode* list_find_by_course(DLinkedList *list, const char *cname);

/* 遍历链表 */
void list_traverse(DLinkedList *list, VisitFunc visit);

/* 按成绩排序（冒泡排序，供性能对比用） */
void list_sort_by_score(DLinkedList *list, int ascending);

/* 获取所有记录到数组（供排序等操作使用） */
int list_to_array(DLinkedList *list, StudentRecord *arr, int max_size);

/* 销毁链表释放内存 */
void list_destroy(DLinkedList *list);

/* 获取链表大小 */
int list_size(DLinkedList *list);

#endif
