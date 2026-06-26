/*
 * fileio.c - CSV文件读写实现
 */
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_records(const char *filename, StudentRecord *records, int max_size) {
    FILE *fp;
    char line[256];
    int count;
    fp = fopen(filename, "r");
    if (!fp) {
        printf("  \u63d0\u793a\uff1a\u6587\u4ef6 %s \u4e0d\u5b58\u5728\uff0c\u5c06\u4f7f\u7528\u7a7a\u6570\u636e\u5f00\u59cb\u3002\n", filename);
        return 0;
    }
    count = 0;
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    while (fgets(line, sizeof(line), fp) && count < max_size) {
        StudentRecord rec;
        char *token;
        int field;
        size_t len;
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        if (len > 1 && line[len - 2] == '\r') line[len - 2] = '\0';
        field = 0;
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
    printf("  \u5df2\u4ece %s \u52a0\u8f7d %d \u6761\u8bb0\u5f55\u3002\n", filename, count);
    return count;
}

int save_to_csv(const char *filename, StudentRecord *arr, int size) {
    int i;
    FILE *fp;
    (void)arr;
    fp = fopen(filename, "w");
    if (!fp) return RES_ERR;
    fprintf(fp, "\u5b66\u53f7,\u59d3\u540d,\u5b66\u9662,\u8bfe\u7a0b\u7f16\u53f7,\u8bfe\u7a0b\u540d\u79f0,\u5b66\u5206,\u9009\u8bfe\u5b66\u671f,\u9009\u8bfe\u65e5\u671f,\u6210\u7ee9\n");
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
