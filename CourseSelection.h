#ifndef COURSESELECTION_H
#define COURSESELECTION_H

#include<stdio.h>

// 选课记录数据结构体定义
typedef struct {
    char student_id[13];     // 学号：12位数字 + 字符串结束符'\0'，总长13
    char name[50];           // 姓名，最多49个汉字/字母
    char college[100];        // 学院全称存储
    char course_id[9];       // 课程编号8位 + '\0'，总长9
    char course_name[100];   // 课程名称
    float credit;            // 学分（浮点，如3.5学分）
    char semester[8];        // 学期格式 YYYY-01 / YYYY-02，例2026-01
    char course_date[12];    // 选课日期 YYYY-MM-DD，例2026-06-24
    int score;               // 百分制成绩 0~100
}CourseSelection;

#endif
