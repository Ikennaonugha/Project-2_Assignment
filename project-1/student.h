#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_CAPACITY 15
#define MAX_STR 50

typedef struct {
    int id;
    char name[MAX_STR];
    char course[MAX_STR];
    float grade;
} Student;

typedef struct {
    Student **data;
    size_t size;
    size_t capacity;
} StudentRegistry;

extern StudentRegistry registry;

typedef void (*OperationFunc)(void);

//function Prototypes
void print_header(void);
void init_initial_dataset(void);
void clear_input_buffer(void);
void add_student(void);
void display_students(void);
void sort_students(void);
void calculate_average(void);
void find_top_student(void);
void search_student(void);
void remove_student(void);
void custom_analysis(void);
void generate_summary_report(void);
void exit_program(void);

#endif // STUDENT_H
