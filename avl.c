#include "avl.h"
#include "CourseSelection.h"
#include <stdlib.h>
#include <string.h>

// 获取节点高度的辅助函数
static int get_height(AVLNode* node) {
    return node ? node->height : 0;
}

// 计算平衡因子的辅助函数
static int get_balance(AVLNode* node) {
    return node ? (get_height(node->left) - get_height(node->right)) : 0;
}

// 辅助更新节点高度
static void update_height(AVLNode* node) {
    if (node) {
        int hl = get_height(node->left);
        int hr = get_height(node->right);
        node->height = (hl > hr ? hl : hr) + 1;
    }
}

// 右单旋（LL型失衡调整）
static AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    update_height(y);
    update_height(x);
    return x;
}

// 左单旋（RR型失衡调整）
static AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    update_height(x);
    update_height(y);
    return y;
}

AVLNode* avl_init() {
    return NULL;
}

// 辅助比较主键大小
static int compare_keys(const char* s1, const char* c1, const char* s2, const char* c2) {
    int res = strcmp(s1, s2);
    if (res != 0) return res;
    return strcmp(c1, c2);
}

AVLNode* avl_insert(AVLNode* root, CourseSelection data, int* success) {
    if (root == NULL) {
        AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
        if (node == NULL) {
            *success = 0;
            return NULL;
        }
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        *success = 1;
        return node;
    }
    
    int cmp = compare_keys(data.student_id, data.course_id, root->data.student_id, root->data.course_id);
    if (cmp < 0) {
        root->left = avl_insert(root->left, data, success);
    } else if (cmp > 0) {
        root->right = avl_insert(root->right, data, success);
    } else {
        *success = 0; // 键值重复
        return root;
    }
    
    if (!(*success)) return root;
    
    update_height(root);
    int balance = get_balance(root);
    
    // LL型失衡
    if (balance > 1 && compare_keys(data.student_id, data.course_id, root->left->data.student_id, root->left->data.course_id) < 0) {
        return rotate_right(root);
    }
    // RR型失衡
    if (balance < -1 && compare_keys(data.student_id, data.course_id, root->right->data.student_id, root->right->data.course_id) > 0) {
        return rotate_left(root);
    }
    // LR型失衡
    if (balance > 1 && compare_keys(data.student_id, data.course_id, root->left->data.student_id, root->left->data.course_id) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // RL型失衡
    if (balance < -1 && compare_keys(data.student_id, data.course_id, root->right->data.student_id, root->right->data.course_id) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    
    return root;
}

// 寻找最小值节点
static AVLNode* min_value_node(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

AVLNode* avl_delete(AVLNode* root, const char* student_id, const char* course_id, int* success) {
    if (root == NULL) {
        *success = 0;
        return root;
    }
    
    int cmp = compare_keys(student_id, course_id, root->data.student_id, root->data.course_id);
    if (cmp < 0) {
        root->left = avl_delete(root->left, student_id, course_id, success);
    } else if (cmp > 0) {
        root->right = avl_delete(root->right, student_id, course_id, success);
    } else {
        *success = 1;
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            AVLNode* temp = min_value_node(root->right);
            root->data = temp->data;
            int sub_success = 0;
            root->right = avl_delete(root->right, temp->data.student_id, temp->data.course_id, &sub_success);
        }
    }
    
    if (root == NULL) return root;
    
    update_height(root);
    int balance = get_balance(root);
    
    // LL
    if (balance > 1 && get_balance(root->left) >= 0) return rotate_right(root);
    // LR
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // RR
    if (balance < -1 && get_balance(root->right) <= 0) return rotate_left(root);
    // RL
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    
    return root;
}

AVLNode* avl_search(AVLNode* root, const char* student_id, const char* course_id) {
    if (root == NULL) return NULL;
    int cmp = compare_keys(student_id, course_id, root->data.student_id, root->data.course_id);
    if (cmp < 0) return avl_search(root->left, student_id, course_id);
    if (cmp > 0) return avl_search(root->right, student_id, course_id);
    return root;
}

void avl_inorder(AVLNode* root, void (*visit)(CourseSelection)) {
    if (root == NULL || visit == NULL) return;
    avl_inorder(root->left, visit);
    visit(root->data);
    avl_inorder(root->right, visit);
}

void avl_destroy(AVLNode* root) {
    if (root == NULL) return;
    avl_destroy(root->left);
    avl_destroy(root->right);
    free(root);
}
