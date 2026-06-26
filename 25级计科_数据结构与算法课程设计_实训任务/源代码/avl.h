/*
 * avl.h - AVL树模块接口
 */
#ifndef AVL_H
#define AVL_H
#include "record.h"

/* AVL树节点 */
typedef struct AVLNode {
    StudentRecord data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

/* AVL树结构 */
typedef struct {
    AVLNode *root;
    int size;
} AVLTree;

/* 初始化空AVL树 */
AVLTree* avl_init(void);

/* 插入记录 */
int avl_insert(AVLTree *tree, StudentRecord rec);

/* 删除指定记录 */
int avl_delete(AVLTree *tree, const char *sid, const char *cid);

/* 按学号查找 */
AVLNode* avl_find(AVLTree *tree, const char *sid);

/* 中序遍历 */
void avl_inorder_traverse(AVLNode *node, VisitFunc visit);

/* 中序遍历收集到数组 */
int avl_inorder_to_array(AVLNode *node, StudentRecord *arr, int index);

/* 按成绩排序（中序遍历获取所有节点后排序） */
int avl_sort_by_score(AVLTree *tree, StudentRecord *arr, int ascending);

/* 获取树中节点数 */
int avl_size(AVLTree *tree);

/* 销毁AVL树 */
void avl_destroy(AVLTree *tree);

/* 公共遍历入口 */
void avl_traverse(AVLTree *tree, VisitFunc visit);

#endif
