#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 40
#define INITIAL_TEXT_CAPACITY 1024
#define INITIAL_STATS_CAPACITY 50

typedef struct wordStat {
    char name[MAX_WORD_LEN];
    int count;
} WordStat;

//keeps track of all state variables needed
typedef struct {
    char *text_buffer;
    size_t text_capacity;
    WordStat *stats;
    size_t distinct_words;
    size_t stats_capacity;
    int total_words;
    char source_title[100];
    char source_author[100];
} TextEngine;

//a shared global state so effect persists
extern TextEngine engine;

typedef void (*AnalysisFunc)(void);

//function prototypes
void print_header(void);
void init_engine(void);
void load_default_text(void);
void input_custom_text(void);
void parse_and_process_words(void);
void memory_address_inspection(void);
void count_words(void);
void longest_word(void);
void most_frequent(void);
void display_word_statistics(void);
void sort_word_statistics(void);
void search_word(void);
void custom_linguistic_analysis(void);
void generate_text_summary(void);
void exit_program(void);

#endif
