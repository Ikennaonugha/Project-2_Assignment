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
