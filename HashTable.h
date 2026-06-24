#define HASH_TABLE_SIZE 10007   // 哈希桶数组的固定初始容量（选用质数减少冲突）
#include "CourseSelection.h"
// 哈希表链地址法冲突链节点定义
typedef struct HashNode {
    CourseSelection data;       // 存储的选课记录数据域
    struct HashNode* next;      // 指向下一个冲突节点的单链表指针
} HashNode;

// 哈希表主体结构体定义
typedef struct {
    HashNode* buckets[HASH_TABLE_SIZE]; // 一维连续的指针数组（哈希桶头指针数组）
    int size;                           // 全局记录总数计数器
} HashTable;

// 哈希表基本操作接口声明 [cite: 112, 167, 204]
// 1. 初始化散列表（清空各桶的头指针为NULL）
void hash_init(HashTable* ht);

// 2. 字符串散列函数（如高效的BKDR-Hash散列算法，计算复合主键对应的桶索引）
unsigned int hash_function(const char* student_id, const char* course_id);

// 3. 插入新记录（计算散列值后，若发生冲突则采用链表头插法挂载到对应桶中）
int hash_insert(HashTable* ht, CourseSelection data);

// 4. 删除指定记录（根据学号+课程编号，在对应的冲突单链表中执行节点切断与释放）
int hash_delete(HashTable* ht, const char* student_id, const char* course_id);

// 5. 精确查找（利用哈希函数直接定位到指定的桶，再遍历冲突单链表精准匹配）
HashNode* hash_search(HashTable* ht, const char* student_id, const char* course_id);

// 6. 遍历散列表（双重循环遍历一维桶数组及桶后的所有单链表节点）
void hash_traverse(HashTable* ht, void (*visit)(CourseSelection));

// 7. 动态销毁散列表（彻底清空并逐个释放所有哈希桶中的链表动态节点）
void hash_destroy(HashTable* ht);
