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

void load_default_text(void) {
    snprintf(engine.source_title, sizeof(engine.source_title), "Harry Potter and the Sorcerer's Stone");
    snprintf(engine.source_author, sizeof(engine.source_author), "J.K. Rowling");

    const char *default_paragraph = 
        "Mr. and Mrs. Dursley, of number four, Privet Drive, were proud to say that they were perfectly normal, thank you very much. "
        "They were the last people you'd expect to be involved in anything strange or mysterious, "
        "because they just didn't hold with such nonsense. "
        "Mr. Dursley was the director of a firm called Grunnings, which made drills. "
        "He was a big, beefy man with hardly any neck, although he did have a very large mustache. "
        "Mrs. Dursley was thin and blonde and had nearly twice the usual amount of neck,";

    if (strlen(default_paragraph) >= engine.text_capacity) {
        engine.text_capacity = strlen(default_paragraph) + 1;
        char *temp = realloc(engine.text_buffer, engine.text_capacity);
        if (temp) engine.text_buffer = temp;
    }
    strcpy(engine.text_buffer, default_paragraph);
    parse_and_process_words();
}

void input_custom_text(void) {
    print_header();
    printf("\n--- Input Custom Text ---\n");
    printf("Enter Source Title: ");
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        snprintf(engine.source_title, sizeof(engine.source_title), "%s", buffer);
    }

    printf("Enter Author/Publisher: ");
    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        snprintf(engine.source_author, sizeof(engine.source_author), "%s", buffer);
    }

    printf("Paste your paragraph:\n> ");
    if (fgets(engine.text_buffer, engine.text_capacity, stdin)) {
        engine.text_buffer[strcspn(engine.text_buffer, "\n")] = '\0';
    }

    parse_and_process_words();
    printf("Successful! Total words: %d\n", engine.total_words);
}
