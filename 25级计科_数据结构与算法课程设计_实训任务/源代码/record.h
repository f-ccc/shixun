/*
 * record.h - 选课记录结构体与公共类型定义
 * 校园选课记录检索与大数据分析系统
 */
#ifndef RECORD_H
#define RECORD_H

/* 选课记录结构体 */
typedef struct {
    char student_id[13];    /* 学号（12位） */
    char name[20];          /* 姓名 */
    char college[30];       /* 学院 */
    char course_id[9];      /* 课程编号（8位） */
    char course_name[50];   /* 课程名称 */
    float credit;           /* 学分 */
    char semester[8];       /* 选课学期（如2024-02） */
    char enroll_date[11];   /* 选课日期（YYYY-MM-DD） */
    int score;              /* 成绩（0-100） */
} StudentRecord;

/* 数据结构类型枚举 */
typedef enum {
    DS_LIST,    /* 双向链表 */
    DS_AVL,     /* AVL树 */
    DS_HASH     /* 哈希表 */
} DataStructureType;

/* 操作结果 */
typedef enum {
    RES_OK = 0,
    RES_ERR = -1,
    RES_NOT_FOUND = -2,
    RES_DUP = -3,
    RES_INVALID = -4
} ResultCode;

/* 排序方向 */
typedef enum {
    ASCENDING = 1,
    DESCENDING = 0
} SortDirection;

/* 筛选条件结构体 */
typedef struct {
    char course_name[50];   /* 课程名称（空=不限制） */
    int fuzzy_match;        /* 1=模糊匹配, 0=精确匹配 */
    char semester[8];       /* 选课学期（空=不限制） */
    int score_min;          /* 成绩下限（-1=不限制） */
    int score_max;          /* 成绩上限（-1=不限制） */
    char college[30];       /* 学院（空=不限制） */
} FilterCondition;

/* 排序条件结构体 */
typedef struct {
    char field[20];         /* 排序字段名 */
    SortDirection dir;      /* 排序方向 */
} SortCondition;

/* 比较函数指针类型 */
typedef int (*CompareFunc)(const StudentRecord*, const StudentRecord*);

/* 遍历访问函数指针类型 */
typedef void (*VisitFunc)(const StudentRecord*);

#endif /* RECORD_H */
