/*
 * generator.c - 数据生成实现
 * 按照任务书附录的数据生成规则生成测试数据
 */
#include "generator.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static const char *surnames[] = {
    "赵","钱","孙","李","周","吴","郑","王","冯","陈",
    "褚","卫","蒋","沈","韩","杨","朱","秦","尤","许",
    "何","吕","施","张","孔","曹","严","华","金","魏",
    "陶","姜","戚","谢","邹","喻","柏","水","窦","章"
};
#define SURNAME_COUNT (sizeof(surnames) / sizeof(surnames[0]))

static const char *given_names[] = {
    "伟","芳","娜","秀英","敏","静","丽","强","磊","军",
    "洋","勇","艳","杰","娟","涛","明","超","秀兰","霞",
    "平","刚","桂英","文","华","飞","玉兰","斌","宇","鑫"
};
#define GIVEN_COUNT (sizeof(given_names) / sizeof(given_names[0]))

static const char *colleges[] = {
    "计算机科学与工程学院",
    "数学与统计学院",
    "电子信息工程学院",
    "机电工程学院",
    "经济与管理学院",
    "外国语学院",
    "土木工程学院",
    "材料科学与工程学院"
};
#define COLLEGE_COUNT (sizeof(colleges) / sizeof(colleges[0]))

typedef struct {
    const char *id;
    const char *name;
    float credit;
} CourseInfo;

static const CourseInfo courses[] = {
    {"CS300101", "数据结构与算法", 3.5},
    {"CS300102", "操作系统原理", 3.0},
    {"CS300103", "数据库系统原理", 3.0},
    {"CS300104", "计算机网络", 3.0},
    {"CS300201", "软件工程", 2.5},
    {"CS300202", "人工智能基础", 2.5},
    {"MA400101", "高等数学A", 4.0},
    {"MA400201", "线性代数", 3.0},
    {"EE500101", "电路分析", 3.0},
    {"EE500102", "数字逻辑设计", 2.5},
    {"ME600101", "机械设计基础", 3.0},
    {"ME600102", "工程制图", 2.0},
    {"EC700101", "微观经济学", 2.5},
    {"EC700102", "会计学原理", 2.5},
    {"FL800101", "大学英语", 3.0},
    {"FL800102", "日语入门", 2.0},
    {"CE900101", "土木工程概论", 2.0},
    {"MS100101", "材料科学导论", 2.5}
};
#define COURSE_COUNT (sizeof(courses) / sizeof(courses[0]))

static void generate_name(char *name) {
    int si, gi;
    si = rand() % SURNAME_COUNT;
    gi = rand() % GIVEN_COUNT;
    sprintf(name, "%s%s", surnames[si], given_names[gi]);
}

static void generate_id(char *id, int index) {
    int year, college_code, seq;
    year = 2020 + (index % 5);
    college_code = 1 + (index % 20);
    seq = (index % 999999) + 1;
    sprintf(id, "%d%02d%06d", year, college_code, seq);
}

static void generate_semester(char *sem, int index) {
    int year, term;
    year = 2020 + (index % 6);
    term = (rand() % 2) + 1;
    sprintf(sem, "%d-%02d", year, term);
}

static void generate_date(char *date, const char *semester) {
    int year, term, month, day;
    sscanf(semester, "%d-%d", &year, &term);
    month = (term == 1) ? 2 + (rand() % 3) : 9 + (rand() % 3);
    day = 1 + (rand() % 28);
    sprintf(date, "%d-%02d-%02d", year, month, day);
}

static int generate_score(void) {
    double u1, u2, z;
    int score;
    u1 = (double)rand() / RAND_MAX;
    u2 = (double)rand() / RAND_MAX;
    z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.1415926 * u2);
    score = (int)(75.0 + 12.0 * z + 0.5);
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    return score;
}

int generate_records(StudentRecord *records, int count) {
    int i, ci;
    srand((unsigned int)time(NULL));
    for (i = 0; i < count; i++) {
        StudentRecord rec;
        generate_id(rec.student_id, i);
        generate_name(rec.name);
        strcpy(rec.college, colleges[rand() % COLLEGE_COUNT]);
        ci = rand() % COURSE_COUNT;
        strcpy(rec.course_id, courses[ci].id);
        strcpy(rec.course_name, courses[ci].name);
        rec.credit = courses[ci].credit;
        generate_semester(rec.semester, i);
        generate_date(rec.enroll_date, rec.semester);
        rec.score = generate_score();
        records[i] = rec;
    }
    return count;
}

int generate_to_csv(const char *filename, int count) {
    StudentRecord *records;
    int ret;
    records = (StudentRecord*)malloc(count * sizeof(StudentRecord));
    if (!records) return RES_ERR;
    generate_records(records, count);
    ret = save_to_csv(filename, records, count);
    free(records);
    return ret;
}
