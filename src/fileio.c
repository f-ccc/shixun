#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_records(const char *filename, StudentRecord *records, int max_size) {
    FILE *fp = fopen(filename, "r");
    char line[256];
    int count = 0, field;
    if (!fp) return 0;
    if (fgets(line, sizeof(line), fp) == NULL) { fclose(fp); return 0; }
    while (fgets(line, sizeof(line), fp) && count < max_size) {
        size_t len = strlen(line);
        char *token;
        StudentRecord rec;
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        if (len > 1 && line[len-2] == '\r') line[len-2] = '\0';
        field = 0; token = strtok(line, ",");
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
            field++; token = strtok(NULL, ",");
        }
        if (field == 9) records[count++] = rec;
    }
    fclose(fp);
    return count;
}

int save_to_csv(const char *filename, StudentRecord *arr, int size) {
    int i;
    FILE *fp = fopen(filename, "w");
    if (!fp) return RES_ERR;
    for (i = 0; i < size; i++)
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                arr[i].student_id, arr[i].name, arr[i].college,
                arr[i].course_id, arr[i].course_name,
                arr[i].credit, arr[i].semester, arr[i].enroll_date, arr[i].score);
    fclose(fp);
    return RES_OK;
}

int export_to_csv(const char *filename, StudentRecord *arr, int size) {
    return save_to_csv(filename, arr, size);
}
