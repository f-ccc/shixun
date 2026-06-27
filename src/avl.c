/* ============================================================
 * avl.c - AVL Tree Implementation
 *
 * Recursive insert/delete with height tracking and balance
 * restoration. Four rotation cases: LL (right rotate),
 * RR (left rotate), LR (left+right), RL (right+left).
 * Internal helpers: create_node, height, get_balance.
 * ============================================================ */

#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int height(AVLNode *node) { return node ? node->height : 0; }
static int max2(int a, int b) { return a > b ? a : b; }

static AVLNode* create_node(StudentRecord rec) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->data = rec; node->left = NULL; node->right = NULL; node->height = 1;
    return node;
}

static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left, *T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max2(height(y->left), height(y->right)) + 1;
    x->height = max2(height(x->left), height(x->right)) + 1;
    return x;
}

static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right, *T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max2(height(x->left), height(x->right)) + 1;
    y->height = max2(height(y->left), height(y->right)) + 1;
    return y;
}

static int get_balance(AVLNode *node) { return node ? height(node->left) - height(node->right) : 0; }

static AVLNode* insert_node(AVLNode *node, StudentRecord rec, int *result) {
    int cmp, balance;
    if (!node) { *result = RES_OK; return create_node(rec); }
    cmp = strcmp(rec.student_id, node->data.student_id);
    if (cmp < 0) node->left = insert_node(node->left, rec, result);
    else if (cmp > 0) node->right = insert_node(node->right, rec, result);
    else { *result = RES_DUP; return node; }
    node->height = 1 + max2(height(node->left), height(node->right));
    balance = get_balance(node);
    if (balance > 1 && strcmp(rec.student_id, node->left->data.student_id) < 0) return rotate_right(node);
    if (balance < -1 && strcmp(rec.student_id, node->right->data.student_id) > 0) return rotate_left(node);
    if (balance > 1 && strcmp(rec.student_id, node->left->data.student_id) > 0) { node->left = rotate_left(node->left); return rotate_right(node); }
    if (balance < -1 && strcmp(rec.student_id, node->right->data.student_id) < 0) { node->right = rotate_right(node->right); return rotate_left(node); }
    return node;
}

static AVLNode* min_value_node(AVLNode *node) {
    while (node->left) node = node->left;
    return node;
}

static AVLNode* delete_node(AVLNode *node, const char *sid, const char *cid, int *result) {
    int cmp, balance;
    AVLNode *temp;
    if (!node) { *result = RES_NOT_FOUND; return NULL; }
    cmp = strcmp(sid, node->data.student_id);
    if (cmp < 0) node->left = delete_node(node->left, sid, cid, result);
    else if (cmp > 0) node->right = delete_node(node->right, sid, cid, result);
    else {
        if (strcmp(node->data.course_id, cid) != 0) {
            node->right = delete_node(node->right, sid, cid, result);
        } else {
            *result = RES_OK;
            if (!node->left || !node->right) {
                temp = node->left ? node->left : node->right;
                if (!temp) { temp = node; node = NULL; } else { *node = *temp; }
                free(temp);
            } else {
                int sub;
                temp = min_value_node(node->right);
                node->data = temp->data;
                node->right = delete_node(node->right, temp->data.student_id, temp->data.course_id, &sub);
            }
        }
    }
    if (!node) return NULL;
    node->height = 1 + max2(height(node->left), height(node->right));
    balance = get_balance(node);
    if (balance > 1 && get_balance(node->left) >= 0) return rotate_right(node);
    if (balance > 1 && get_balance(node->left) < 0) { node->left = rotate_left(node->left); return rotate_right(node); }
    if (balance < -1 && get_balance(node->right) <= 0) return rotate_left(node);
    if (balance < -1 && get_balance(node->right) > 0) { node->right = rotate_right(node->right); return rotate_left(node); }
    return node;
}

AVLTree* avl_init(void) {
    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (!tree) return NULL;
    tree->root = NULL; tree->size = 0;
    return tree;
}

int avl_insert(AVLTree *tree, StudentRecord rec) {
    int result;
    if (!tree) return RES_ERR;
    tree->root = insert_node(tree->root, rec, &result);
    if (result == RES_OK) tree->size++;
    return result;
}

int avl_delete(AVLTree *tree, const char *sid, const char *cid) {
    int result;
    if (!tree) return RES_ERR;
    tree->root = delete_node(tree->root, sid, cid, &result);
    if (result == RES_OK) tree->size--;
    return result;
}

AVLNode* avl_find(AVLTree *tree, const char *sid) {
    AVLNode *cur;
    int cmp;
    if (!tree) return NULL;
    cur = tree->root;
    while (cur) {
        cmp = strcmp(sid, cur->data.student_id);
        if (cmp == 0) return cur;
        cur = cmp < 0 ? cur->left : cur->right;
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
    return avl_inorder_to_array(node->right, arr, index);
}

static void swap_rec(StudentRecord *a, StudentRecord *b) {
    StudentRecord t = *a; *a = *b; *b = t;
}

int avl_sort_by_score(AVLTree *tree, StudentRecord *arr, int ascending) {
    int i, j, n;
    if (!tree) return 0;
    n = avl_inorder_to_array(tree->root, arr, 0);
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if ((ascending && arr[j].score > arr[j+1].score) || (!ascending && arr[j].score < arr[j+1].score))
                swap_rec(&arr[j], &arr[j+1]);
    return n;
}

int avl_size(AVLTree *tree) { return tree ? tree->size : 0; }

static void destroy_node(AVLNode *node) {
    if (!node) return;
    destroy_node(node->left); destroy_node(node->right);
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
