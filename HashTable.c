#include "HashTable.h"
#include "CourseSelection.h"
#include <stdlib.h>
#include <string.h>

void hash_init(HashTable* ht) {
    if (ht == NULL) return;
    int i; 
    for ( i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

unsigned int hash_function(const char* student_id, const char* course_id) {
    unsigned int hash = 0;
    unsigned int seed = 131; // BKDR Hash算法常用的质数特征种子
    
    // 依次计算学号字符的权重散列值
    while (*student_id) {
        hash = hash * seed + (*student_id++);
    }
    // 叠加组合课程编号的权重散列值
    while (*course_id) {
        hash = hash * seed + (*course_id++);
    }
    return hash % HASH_TABLE_SIZE;
}

int hash_insert(HashTable* ht, CourseSelection data) {
    if (ht == NULL) return 0;
    if (hash_search(ht, data.student_id, data.course_id) != NULL) {
        return 0; // 重复冲突记录
    }
    
    unsigned int idx = hash_function(data.student_id, data.course_id);
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    if (new_node == NULL) return 0;
    
    new_node->data = data;
    // 使用头插法挂载到单链表冲突桶中
    new_node->next = ht->buckets[idx];
    ht->buckets[idx] = new_node;
    ht->size++;
    return 1;
}

int hash_delete(HashTable* ht, const char* student_id, const char* course_id) {
    if (ht == NULL) return 0;
    unsigned int idx = hash_function(student_id, course_id);
    HashNode* curr = ht->buckets[idx];
    HashNode* prev = NULL;
    
    while (curr != NULL) {
        if (strcmp(curr->data.student_id, student_id) == 0 && strcmp(curr->data.course_id, course_id) == 0) {
            if (prev != NULL) {
                prev->next = curr->next;
            } else {
                ht->buckets[idx] = curr->next;
            }
            free(curr);
            ht->size--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

HashNode* hash_search(HashTable* ht, const char* student_id, const char* course_id) {
    if (ht == NULL) return NULL;
    unsigned int idx = hash_function(student_id, course_id);
    HashNode* curr = ht->buckets[idx];
    while (curr != NULL) {
        if (strcmp(curr->data.student_id, student_id) == 0 && strcmp(curr->data.course_id, course_id) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void hash_traverse(HashTable* ht, void (*visit)(CourseSelection)) {
    if (ht == NULL || visit == NULL) return;
    int i;
    for ( i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* curr = ht->buckets[i];
        while (curr != NULL) {
            visit(curr->data);
            curr = curr->next;
        }
    }
}

void hash_destroy(HashTable* ht) {
    if (ht == NULL) return;
    int i;
    for ( i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode* curr = ht->buckets[i];
        while (curr != NULL) {
            HashNode* tmp = curr->next;
            free(curr);
            curr = tmp;
        }
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}
