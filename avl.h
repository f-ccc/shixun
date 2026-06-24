#include<stdio.h>
#include"CourseSelection.h"
// AVL树平衡二叉树节点结构体定义
typedef struct AVLNode {
    CourseSelection data;       // 存储的选课记录数据域
    struct AVLNode* left;       // 指向左孩子的指针
    struct AVLNode* right;      // 指向右孩子的指针
    int height;                 // 节点当前高度，用于动态计算平衡因子（左右子树高度差）
} AVLNode;

// AVL树基本操作接口声明 [cite: 112, 167, 204]
// 1. 初始化AVL树（返回空根节点指针）
AVLNode* avl_init();

// 2. 插入新记录（根据学号+课程编号复合键比较，插入后自动进行平衡旋转调整）
AVLNode* avl_insert(AVLNode* root, CourseSelection data, int* success);

// 3. 删除指定记录（平衡删除操作，若移除节点破坏了平衡性则触发旋转修正）
AVLNode* avl_delete(AVLNode* root, const char* student_id, const char* course_id, int* success);

// 4. 精确查找（基于二叉搜索树逻辑，通过复合主键快速定位节点）
AVLNode* avl_search(AVLNode* root, const char* student_id, const char* course_id);

// 5. 中序遍历（利用中序遍历天然有序的特征，输出升序的数据序列）
void avl_inorder(AVLNode* root, void (*visit)(CourseSelection));

// 6. 动态销毁AVL树（后序遍历释放整棵树所有离散节点的物理堆内存）
void avl_destroy(AVLNode* root);
