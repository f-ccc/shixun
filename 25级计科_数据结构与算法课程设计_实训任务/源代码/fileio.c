/*
 * fileio.c - CSV文件读写实现
 */
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_records(const char *filename, StudentRecord *records, int max_size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("  提示：文件 %s 不存在，将使用空数据开始。\n", filename);
        return 0;
    }

    char line[256];
    int count = 0;

    /* 跳过表头行 */
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(line, sizeof(line), fp) && count < max_size) {
        /* 去除末尾换行符 */
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        if (len > 1 && line[len - 2] == '\r') line[len - 2] = '\0';

        /* 解析CSV行：学号,姓名,学院,课程编号,课程名称,学分,学期,日期,成绩 */
        StudentRecord rec;
        char *token;
        int field = 0;

        token = strtok(line, ",");
        while (token && field < 9) {
            switch (field) {
                case 0: strncpy(rec.student_id, token, 12); rec.student_id[12] = '\0'; break;
                case 1: strncpy(rec.name, token, 19); rec.name[19] = '\0'; break;
                case 2: strncpy(rec.college, token, 29); rec.college[29] = '\0'; break;
                case 3: strncpy(rec.course_id, token, 8); rec.course_id[8] = '\0'; break;
                case 4: strncpy(rec.course_name, token, 49); rec.course_name[49] = '\0'; break;
                case 5: rec.credit = (float)atof(token); break;
                case 6: strncpy(rec.semester, token, 6); rec.semester[6] = '\0'; break;
                case 7: strncpy(rec.enroll_date, token, 10); rec.enroll_date[10] = '\0'; break;
                case 8: rec.score = atoi(token); break;
            }
            field++;
            token = strtok(NULL, ",");
        }
        if (field == 9) {
            records[count++] = rec;
        }
    }
    fclose(fp);
    printf("  已从 %s 加载 %d 条记录。\n", filename, count);
    return count;
}

int save_to_csv(const char *filename, StudentRecord *arr, int size) {
    int i;
    FILE *fp = fopen(filename, "w");
    if (!fp) return RES_ERR;

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,选课学期,选课日期,成绩\n");
    for (i = 0; i < size; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                arr[i].student_id, arr[i].name, arr[i].college,
                arr[i].course_id, arr[i].course_name,
                arr[i].credit, arr[i].semester,
                arr[i].enroll_date, arr[i].score);
    }
    fclose(fp);
    return RES_OK;
}

int export_to_csv(const char *filename, StudentRecord *arr, int size) {
    return save_to_csv(filename, arr, size);
}
