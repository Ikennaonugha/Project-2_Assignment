#include "text_analyzer.h"

//define and initialize 
TextEngine engine = {NULL, 0, NULL, 0, 0, 0, "", ""};

void print_header(void) {
    printf("\n------------------------------\n");
    printf(" Student ID: 1001 (I don't know my student ID\n");
    printf(" Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
    printf(" Source: %s by %s\n", 
           strlen(engine.source_title) > 0 ? engine.source_title : "Default",
           strlen(engine.source_author) > 0 ? engine.source_author : "Unknown");
    printf(" Custom Analysis: Lexical Diversity Score (LDS)\n");
    printf("-------------------------------\n");
}

void init_engine(void) {
    engine.text_capacity = INITIAL_TEXT_CAPACITY;
    engine.text_buffer = malloc(engine.text_capacity * sizeof(char));
    if (!engine.text_buffer) {
        perror("Critical: Text buffer allocation failed");
        exit(1);
    }
    engine.text_buffer[0] = '\0';

    engine.stats_capacity = INITIAL_STATS_CAPACITY;
    engine.stats = malloc(engine.stats_capacity * sizeof(WordStat));
    if (!engine.stats) {
        perror("Critical: Word stats allocation failed");
        free(engine.text_buffer);
        exit(1);
    }
    engine.distinct_words = 0;
    engine.total_words = 0;
    load_default_text();
}
