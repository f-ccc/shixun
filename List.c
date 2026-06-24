#include "List.h"
#include <stdlib.h>
#include <string.h>

void list_init(DoublyLinkedList* list) {
    if (list == NULL) return;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int list_insert(DoublyLinkedList* list, CourseSelection data) {
    if (list == NULL) return 0;
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (new_node == NULL) return 0;
    
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    list->size++;
    return 1;
}

int list_delete(DoublyLinkedList* list, const char* student_id, const char* course_id) {
    if (list == NULL || list->head == NULL) return 0;
    ListNode* curr = list->head;
    
    while (curr != NULL) {
        if (strcmp(curr->data.student_id, student_id) == 0 && strcmp(curr->data.course_id, course_id) == 0) {
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            } else {
                list->head = curr->next;
            }
            
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            } else {
                list->tail = curr->prev;
            }
            
            free(curr);
            list->size--;
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

ListNode* list_search(DoublyLinkedList* list, const char* student_id, const char* course_id) {
    if (list == NULL) return NULL;
    ListNode* curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->data.student_id, student_id) == 0 && strcmp(curr->data.course_id, course_id) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void list_traverse(DoublyLinkedList* list, void (*visit)(CourseSelection)) {
    if (list == NULL || visit == NULL) return;
    ListNode* curr = list->head;
    while (curr != NULL) {
        visit(curr->data);
        curr = curr->next;
    }
}

void list_destroy(DoublyLinkedList* list) {
    if (list == NULL) return;
    ListNode* curr = list->head;
    while (curr != NULL) {
        ListNode* next_node = curr->next;
        free(curr);
        curr = next_node;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
