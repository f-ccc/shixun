#include<stdio.h>

// 选课记录数据结构体定义
typedef struct {
    char student_id[13];     // 学号：12位数字固定长度字符串，末尾留'\0' 
    char name[50];           // 姓名：动态分配或固定上限字符串 
    char college[100];       // 学院：学生所属学院全称 
    char course_id[9];       // 课程编号：8位字符串，末尾留'\0' 
    char course_name[100];   // 课程名称：修读课程全称 
    float credit;            // 学分：课程对应学分 
    char semester[8];        // 选课学期：格式"YYYY-01"或"YYYY-02" 
    char course_date[12];    // 选课日期：具体选课日期，格式"YYYY-MM-DD" 
    int score;               // 成绩：百分制整数(0-100) 
} CourseSelection;

//双向链表节点结构体定义
typedef struct ListNode{
	CourseSelection date;		//储存选课记录 
	struct ListNode* prev;		//指向前驱节点指针 
	struct ListNode* next;		//指向后继节点指针 
}ListNode;

//双向链表主体控制结构体定义 
typedef struct{
	ListNode* head;		//头指针 
	ListNode* tail;		//尾指针 
	int size;			//当前链表中数据总数 
}List;
