/*
 * avl.c - AVL树实现（平衡二叉搜索树）
 */
#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 获取节点高度 */
static int height(AVLNode *node) {
    return node ? node->height : 0;
}

/* 获取较大值 */
static int max2(int a, int b) {
    return (a > b) ? a : b;
}

/* 创建新节点 */
static AVLNode* create_node(StudentRecord rec) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->data = rec;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

/* 右旋转 */
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max2(height(y->left), height(y->right)) + 1;
    x->height = max2(height(x->left), height(x->right)) + 1;
    return x;
}

/* 左旋转 */
static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max2(height(x->left), height(x->right)) + 1;
    y->height = max2(height(y->left), height(y->right)) + 1;
    return y;
}

/* 获取平衡因子 */
static int get_balance(AVLNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

/* 递归插入（内部函数） */
static AVLNode* insert_node(AVLNode *node, StudentRecord rec, int *result) {
    if (node == NULL) {
        *result = OK;
        return create_node(rec);
    }

    int cmp = strcmp(rec.student_id, node->data.student_id);
    if (cmp < 0)
        node->left = insert_node(node->left, rec, result);
    else if (cmp > 0)
        node->right = insert_node(node->right, rec, result);
    else {
        *result = DUPLICATE;  /* 学号重复 */
        return node;
    }

    node->height = 1 + max2(height(node->left), height(node->right));
    int balance = get_balance(node);

    /* 左左 */
    if (balance > 1 && strcmp(rec.student_id, node->left->data.student_id) < 0)
        return rotate_right(node);
    /* 右右 */
    if (balance < -1 && strcmp(rec.student_id, node->right->data.student_id) > 0)
        return rotate_left(node);
    /* 左右 */
    if (balance > 1 && strcmp(rec.student_id, node->left->data.student_id) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    /* 右左 */
    if (balance < -1 && strcmp(rec.student_id, node->right->data.student_id) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

/* 找到最小值节点 */
static AVLNode* min_value_node(AVLNode *node) {
    AVLNode *cur = node;
    while (cur->left) cur = cur->left;
    return cur;
}

/* 递归删除（内部函数） */
static AVLNode* delete_node(AVLNode *node, const char *sid, const char *cid, int *result) {
    if (node == NULL) {
        *result = NOT_FOUND;
        return NULL;
    }

    int cmp = strcmp(sid, node->data.student_id);
    if (cmp < 0)
        node->left = delete_node(node->left, sid, cid, result);
    else if (cmp > 0)
        node->right = delete_node(node->right, sid, cid, result);
    else {
        /* 还要检查课程编号 */
        if (strcmp(node->data.course_id, cid) != 0) {
            /* 课程编号不匹配，继续在右子树找（学号可能重复，但我们的设计中学号唯一） */
            node->right = delete_node(node->right, sid, cid, result);
        } else {
            *result = OK;
            if (node->left == NULL || node->right == NULL) {
                AVLNode *temp = node->left ? node->left : node->right;
                if (temp == NULL) {
                    temp = node;
                    node = NULL;
                } else {
                    *node = *temp;
                }
                free(temp);
            } else {
                AVLNode *temp = min_value_node(node->right);
                node->data = temp->data;
                int sub_result;
                node->right = delete_node(node->right, temp->data.student_id,
                                          temp->data.course_id, &sub_result);
            }
        }
    }

    if (node == NULL) return NULL;

    node->height = 1 + max2(height(node->left), height(node->right));
    int balance = get_balance(node);

    if (balance > 1 && get_balance(node->left) >= 0)
        return rotate_right(node);
    if (balance > 1 && get_balance(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && get_balance(node->right) <= 0)
        return rotate_left(node);
    if (balance < -1 && get_balance(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

AVLTree* avl_init(void) {
    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

int avl_insert(AVLTree *tree, StudentRecord rec) {
    if (!tree) return ERROR;
    int result;
    tree->root = insert_node(tree->root, rec, &result);
    if (result == OK) tree->size++;
    return result;
}

int avl_delete(AVLTree *tree, const char *sid, const char *cid) {
    if (!tree) return ERROR;
    int result;
    tree->root = delete_node(tree->root, sid, cid, &result);
    if (result == OK) tree->size--;
    return result;
}

AVLNode* avl_find(AVLTree *tree, const char *sid) {
    if (!tree) return NULL;
    AVLNode *cur = tree->root;
    while (cur) {
        int cmp = strcmp(sid, cur->data.student_id);
        if (cmp == 0) return cur;
        if (cmp < 0) cur = cur->left;
        else cur = cur->right;
    }
    return NULL;
}

void avl_inorder_traverse(AVLNode *node, VisitFunc visit) {
    if (!node) return;
    avl_inorder_traverse(node->left, visit);
    visit(&node->data);
    avl_inorder_traverse(node->right, visit);
}

int avl_inorder_to_array(AVLNode *node, StudentRecord *arr, int index) {
    if (!node) return index;
    index = avl_inorder_to_array(node->left, arr, index);
    arr[index++] = node->data;
    index = avl_inorder_to_array(node->right, arr, index);
    return index;
}

static void swap_rec(StudentRecord *a, StudentRecord *b) {
    StudentRecord t = *a;
    *a = *b;
    *b = t;
}

int avl_sort_by_score(AVLTree *tree, StudentRecord *arr, int ascending) {
    if (!tree) return 0;
    int n = avl_inorder_to_array(tree->root, arr, 0);
    /* 按成绩排序（冒泡） */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (ascending) {
                if (arr[j].score > arr[j + 1].score)
                    swap_rec(&arr[j], &arr[j + 1]);
            } else {
                if (arr[j].score < arr[j + 1].score)
                    swap_rec(&arr[j], &arr[j + 1]);
            }
        }
    }
    return n;
}

int avl_size(AVLTree *tree) {
    return tree ? tree->size : 0;
}

static void destroy_node(AVLNode *node) {
    if (!node) return;
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
}

void avl_destroy(AVLTree *tree) {
    if (!tree) return;
    destroy_node(tree->root);
    free(tree);
}

void avl_traverse(AVLTree *tree, VisitFunc visit) {
    if (tree) avl_inorder_traverse(tree->root, visit);
}
