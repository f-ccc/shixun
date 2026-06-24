#include<stdio.h>
#include"CourseSelection.h"

// 双向链表节点结构体定义
typedef struct ListNode {
    CourseSelection data;       // 存储的选课记录数据域
    struct ListNode* prev;      // 指向前驱节点的指针域
    struct ListNode* next;      // 指向后继节点的指针域
} ListNode;

// 双向链表主体控制结构体定义
typedef struct {
    ListNode* head;             // 链表头指针（带头节点或不带头节点）
    ListNode* tail;             // 链表尾指针，便于执行尾插法
    int size;                   // 当前链表中存储的记录总数
} DoublyLinkedList;

// 双向链表基本操作接口声明 [cite: 112, 167, 204]
// 1. 初始化双向链表
void list_init(DoublyLinkedList* list);

// 2. 插入新记录（默认采用尾插法添加一条新数据）
int list_insert(DoublyLinkedList* list, CourseSelection data);

// 3. 删除特定记录（根据学号+课程编号复合主键精准匹配删除）
int list_delete(DoublyLinkedList* list, const char* student_id, const char* course_id);

// 4. 精确查找（根据学号与课程编号检索指定节点）
ListNode* list_search(DoublyLinkedList* list, const char* student_id, const char* course_id);

// 5. 遍历链表（传入函数指针，用于多条件筛选或多指标教务数据统计）
void list_traverse(DoublyLinkedList* list, void (*visit)(CourseSelection));

// 6. 销毁链表（依次释放所有动态申请的节点内存，防止内存泄漏）
void list_destroy(DoublyLinkedList* list);
