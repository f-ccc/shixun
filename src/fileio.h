/* ============================================================
 * fileio.h - CSV File I/O Interface
 *
 * Load/save/export enrollment records in CSV format:
 * student_id,name,college,course_id,course_name,
 * credit,semester,enroll_date,score
 * ============================================================ */

#ifndef FILEIO_H
#define FILEIO_H
#include "record.h"

int load_records(const char *filename, StudentRecord *records, int max_size);
int save_to_csv(const char *filename, StudentRecord *arr, int size);
int export_to_csv(const char *filename, StudentRecord *arr, int size);

#endif
