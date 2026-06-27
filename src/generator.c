/* ============================================================
 * generator.c - Test Data Generator Implementation
 *
 * Uses rand() with surname/given_name lists, course table,
 * auto-incrementing student IDs. Date/semester are derived
 * from the record index. Score uses Box-Muller transform
 * for approximate normal distribution.
 * ============================================================ */

#include "generator.h"
#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static const char *surnames[] = {"Smith","Johnson","Brown","Davis","Miller","Wilson","Moore","Taylor","Anderson","Lee",
    "White","Harris","Martin","Thompson","Clark","Lewis","Walker","Hall","Allen","Young"};
#define SN_CNT 20

static const char *given_names[] = {"James","John","Robert","Michael","William","David","Richard","Joseph","Thomas","Emma",
    "Olivia","Ava","Sophia","Mia","Charlotte","Amelia","Luna","Evelyn","Harper","Liam"};
#define GN_CNT 20

static const char *colleges[] = {"Computer Science","Mathematics","Electronics","Mechanical Eng","Economics",
    "Foreign Languages","Civil Engineering","Materials Science"};
#define CLG_CNT 8

typedef struct { const char *id; const char *name; float credit; } CourseInfo;
static const CourseInfo courses[] = {
    {"CS300101","Data Structures",3.5},{"CS300102","Operating Systems",3.0},{"CS300103","Database Systems",3.0},
    {"CS300104","Computer Networks",3.0},{"CS300201","Software Engineering",2.5},{"CS300202","AI Fundamentals",2.5},
    {"MA400101","Advanced Math A",4.0},{"MA400201","Linear Algebra",3.0},{"EE500101","Circuit Analysis",3.0},
    {"EE500102","Digital Logic",2.5},{"ME600101","Machine Design",3.0},{"ME600102","Engineering Drawing",2.0},
    {"EC700101","Microeconomics",2.5},{"EC700102","Accounting",2.5},{"FL800101","College English",3.0},
    {"FL800102","Japanese Intro",2.0},{"CE900101","Civil Engineering",2.0},{"MS100101","Materials Science",2.5}
};
#define CRS_CNT 18

static void generate_name(char *name) {
    sprintf(name, "%s%s", surnames[rand()%SN_CNT], given_names[rand()%GN_CNT]);
}

static void generate_id(char *id, int index) {
    int year = 2020 + (index % 5);
    int college_code = 1 + (index % 20);
    int seq = (index % 999999) + 1;
    sprintf(id, "%d%02d%06d", year, college_code, seq);
}

static void generate_semester(char *sem, int index) {
    sprintf(sem, "%d-%02d", 2020 + (index % 6), (rand()%2)+1);
}

static void generate_date(char *date, const char *semester) {
    int y, m;
    sscanf(semester, "%d-%d", &y, &m);
    m = (m == 1) ? 2 + (rand()%3) : 9 + (rand()%3);
    sprintf(date, "%d-%02d-%02d", y, m, 1 + (rand()%28));
}

static int generate_score(void) {
    double u1 = (double)rand()/RAND_MAX, u2 = (double)rand()/RAND_MAX;
    double z = sqrt(-2.0*log(u1))*cos(2.0*3.1415926*u2);
    int s = (int)(75.0+12.0*z+0.5);
    if (s<0) s=0;
    if (s>100) s=100;
    return s;
}

int generate_records(StudentRecord *records, int count) {
    int i;
    srand((unsigned)time(NULL));
    for (i = 0; i < count; i++) {
        StudentRecord rec;
        int ci;
        generate_id(rec.student_id, i);
        generate_name(rec.name);
        strcpy(rec.college, colleges[rand()%CLG_CNT]);
        ci = rand()%CRS_CNT;
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
    StudentRecord *records = malloc(count*sizeof(StudentRecord));
    int ret;
    if (!records) return RES_ERR;
    generate_records(records, count);
    ret = save_to_csv(filename, records, count);
    free(records);
    return ret;
}
