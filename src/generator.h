/* ============================================================
 * generator.h - Test Data Generator Interface
 *
 * Generates randomized enrollment records for testing.
 * Scores follow a normal distribution (mean=75, sd=12).
 * ============================================================ */

#ifndef GENERATOR_H
#define GENERATOR_H
#include "record.h"

int generate_records(StudentRecord *records, int count);
int generate_to_csv(const char *filename, int count);

#endif
