/* ============================================================
 * record.h - Common Type Definitions
 *
 * Core data structures for the Course Registration System:
 * StudentRecord (main entity), enum types, filter/sort structs,
 * and function pointer typedefs for traversal callbacks.
 * ============================================================ */

#ifndef RECORD_H
#define RECORD_H

typedef struct {
    char student_id[13];
    char name[20];
    char college[30];
    char course_id[9];
    char course_name[50];
    float credit;
    char semester[8];
    char enroll_date[11];
    int score;
} StudentRecord;

typedef enum { DS_LIST, DS_AVL, DS_HASH } DataStructureType;
typedef enum { RES_OK = 0, RES_ERR = -1, RES_NOT_FOUND = -2, RES_DUP = -3, RES_INVALID = -4 } ResultCode;
typedef enum { ASCENDING = 1, DESCENDING = 0 } SortDirection;

typedef struct {
    char course_name[50];
    int fuzzy_match;
    char semester[8];
    int score_min;
    int score_max;
    char college[30];
} FilterCondition;

typedef struct {
    char field[20];
    SortDirection dir;
} SortCondition;

typedef int (*CompareFunc)(const StudentRecord*, const StudentRecord*);
typedef void (*VisitFunc)(const StudentRecord*);

#endif
