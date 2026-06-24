#ifndef BUSINESS_H
#define BUSINESS_H

#include "List.h"
#include "avl.h"
#include "HashTable.h"

// 多条件过滤规则结构体定义
typedef struct {
    char course_name[100]; // 模糊或精确课程名匹配，空字符串代表不生效
    char semester[8];      // 精确学期匹配
    char college[100];     // 精确学院匹配
    int score_min;         // 成绩下限
    int score_max;         // 成绩上限
} FilterCriteria;

// 多关键字排序规则定义
typedef struct {
    int key_types[3];  // 1:按成绩, 2:按学号, 3:按学分
    int directions[3]; // 1:升序, -1:降序
    int criterion_count;
} SortConfig;

// 业务全局管理容器
typedef struct {
    DoublyLinkedList list;
    HashTable hash_table;
    AVLNode* avl_root;
} SystemContainer;

void sys_init(SystemContainer* sys);
void sys_destroy(SystemContainer* sys);
int sys_load_csv(SystemContainer* sys, const char* filename);
int sys_save_csv(SystemContainer* sys, const char* filename);

// 核心同步操作接口（增、删、改、查）
int sys_insert_sync(SystemContainer* sys, CourseSelection data);
int sys_delete_sync(SystemContainer* sys, const char* student_id, const char* course_id);
int sys_update_sync(SystemContainer* sys, const char* student_id, const char* course_id, int new_score);
void sys_search_flexible(SystemContainer* sys, int search_type, const char* keyword);

void generate_mock_data_file(const char* filename, int count);
void execute_filter(SystemContainer* sys, FilterCriteria criteria, const char* out_filename);
void execute_multi_sort(SystemContainer* sys, SortConfig config);

void stats_course_capacity(SystemContainer* sys);
void stats_student_credits(SystemContainer* sys);
void stats_college_distribution(SystemContainer* sys);
void stats_score_sections(SystemContainer* sys);
int purge_expired_records(SystemContainer* sys);

#endif
