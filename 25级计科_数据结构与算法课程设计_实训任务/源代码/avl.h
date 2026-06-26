/*
 * avl.h - AVL树模块接口
 */
#ifndef AVL_H
#define AVL_H
#include "record.h"

typedef struct AVLNode {
    StudentRecord data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct {
    AVLNode *root;
    int size;
} AVLTree;

AVLTree* avl_init(void);
int avl_insert(AVLTree *tree, StudentRecord rec);
int avl_delete(AVLTree *tree, const char *sid, const char *cid);
AVLNode* avl_find(AVLTree *tree, const char *sid);
void avl_inorder_traverse(AVLNode *node, VisitFunc visit);
int avl_inorder_to_array(AVLNode *node, StudentRecord *arr, int index);
int avl_sort_by_score(AVLTree *tree, StudentRecord *arr, int ascending);
int avl_size(AVLTree *tree);
void avl_destroy(AVLTree *tree);
void avl_traverse(AVLTree *tree, VisitFunc visit);

#endif
