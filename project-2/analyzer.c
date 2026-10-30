#include "text_analyzer.h"

//define and initialize 
TextEngine engine = {NULL, 0, NULL, 0, 0, 0, "", ""};

void print_header(void) {
    printf("\n------------------------------------------------\n");
    printf("Student ID: 1001 (I don't know my student ID)\n");
    printf("Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
    printf("Source: %s by %s\n", 
           strlen(engine.source_title) > 0 ? engine.source_title : "Default",
           strlen(engine.source_author) > 0 ? engine.source_author : "Unknown");
    printf("Custom Analysis: Lexical Diversity Score\n");
    printf("---------------------------------------------------\n");
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

//parsing using pointers
void parse_and_process_words(void) {
    engine.distinct_words = 0;
    engine.total_words = 0;

    char *p = engine.text_buffer;
    while (*p != '\0') {
        while (*p != '\0' && !isalpha((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') break;

        //get word character by character
        char raw_word[128];
        int len = 0;
        while (*p != '\0' && (isalpha((unsigned char)*p) || *p == '-')) {
            if (len < (int)sizeof(raw_word) - 1) {
                raw_word[len++] = tolower((unsigned char)*p);
            }
            p++;
        }
        raw_word[len] = '\0';

        if (len > 0) {
            engine.total_words++;

            // Handle 39-character limit safely by truncating to 39 chars
            char clean_word[MAX_WORD_LEN];
            snprintf(clean_word, sizeof(clean_word), "%s", raw_word);

            //check if it already exists
            int found = 0;
            for (size_t i = 0; i < engine.distinct_words; i++) {
                if (strcmp(engine.stats[i].name, clean_word) == 0) {
                    engine.stats[i].count++;
                    found = 1;
                    break;
                }
            }

            //add to stats if found
            if (!found) {
                if (engine.distinct_words >= engine.stats_capacity) {
                    size_t new_cap = engine.stats_capacity + 25;
                    WordStat *temp = realloc(engine.stats, new_cap * sizeof(WordStat));
                    if (!temp) {
                        perror("Memory reallocation failed for word stats");
                        return;
                    }
                    engine.stats = temp;
                    engine.stats_capacity = new_cap;
                }
                strcpy(engine.stats[engine.distinct_words].name, clean_word);
                engine.stats[engine.distinct_words].count = 1;
                engine.distinct_words++;
            }
        }
    }
}

void memory_address_inspection(void) {
    print_header();
    printf("\nMEMORY ADDRESS INSPECTION\n");
    printf("Text buffer address          : %p\n", (void *)engine.text_buffer);
    printf("Word statistics array address: %p\n", (void *)engine.stats);
    if (engine.distinct_words > 0) {
        printf(" Selected word-stat entry [0] : %p\n", (void *)&engine.stats[0]);
    } else {
        printf(" Selected word-stat entry: N/A\n");
    }
}

void count_words(void) {
    print_header();
    printf("\n--- Word Count Analysis ---\n");
    printf("Total Words in Text : %d\n", engine.total_words);
    printf("Distinct Words      : %zu\n", engine.distinct_words);
}

void longest_word(void) {
    print_header();
    if (engine.distinct_words == 0) {
        printf("\nNo words recorded.\n");
        return;
    }

    size_t longest_idx = 0;
    for (size_t i = 1; i < engine.distinct_words; i++) {
        if (strlen(engine.stats[i].name) > strlen(engine.stats[longest_idx].name)) {
            longest_idx = i;
        }
    }

    printf("\n--- Longest Word Analysis ---\n");
    printf("Longest Word : %s\n", engine.stats[longest_idx].name);
    printf("Length       : %zu characters\n", strlen(engine.stats[longest_idx].name));
}

void most_frequent(void) {
    print_header();
    if (engine.distinct_words == 0) {
        printf("\nNo words recorded.\n");
        return;
    }

    size_t freq_idx = 0;
    for (size_t i = 1; i < engine.distinct_words; i++) {
        if (engine.stats[i].count > engine.stats[freq_idx].count) {
            freq_idx = i;
        }
    }

    printf("\n--- Most Frequent Word ---\n");
    printf("Word      : %s\n", engine.stats[freq_idx].name);
    printf("Frequency : %d occurrences\n", engine.stats[freq_idx].count);
}

void display_word_statistics(void) {
    print_header();
    printf("\n--- Distinct Word Frequency Table ---\n");
    printf("---------------------------------------------------\n");
    printf(" %-30s | %-10s \n", "Word", "Frequency");
    printf("---------------------------------------------------\n");
    for (size_t i = 0; i < engine.distinct_words; i++) {
        printf(" %-30s | %-10d \n", engine.stats[i].name, engine.stats[i].count);
    }
    printf("---------------------------------------------------\n");
}

void sort_word_statistics(void) {
    print_header();
    printf("\n--- Sort Word Statistics ---\n");
    printf("[1] Sort Alphabetically (A-Z)\n");
    printf("[2] Sort by Frequency (Highest to Lowest)\n");
    printf("Select option: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    for (size_t i = 0; i < engine.distinct_words; i++) {
        for (size_t j = i + 1; j < engine.distinct_words; j++) {
            int condition = 0;
            if (choice == 1) {
                condition = (strcmp(engine.stats[i].name, engine.stats[j].name) > 0);
            } else if (choice == 2) {
                condition = (engine.stats[i].count < engine.stats[j].count);
            }

            if (condition) {
                WordStat temp = engine.stats[i];
                engine.stats[i] = engine.stats[j];
                engine.stats[j] = temp;
            }
        }
    }
    printf("Word statistics successfully sorted!\n");
    display_word_statistics();
}

void search_word(void) {
    print_header();
    printf("\n--- Search Word ---\n");
    printf("Enter word to search: ");
    char target[MAX_WORD_LEN];
    if (fgets(target, sizeof(target), stdin)) {
        target[strcspn(target, "\n")] = '\0';
    }

    for (size_t i = 0; i < engine.distinct_words; i++) {
        if (strcmp(engine.stats[i].name, target) == 0) {
            printf("\nFound\nWord: '%s' appears %d time(s).\n", engine.stats[i].name, engine.stats[i].count);
            return;
        }
    }
    printf("Word '%s' not found.\n", target);
}

void custom_linguistic_analysis(void) {
    print_header();
    if (engine.total_words == 0) {
        printf("\nNo text analyzed.\n");
        return;
    }

    //ratio of distinct words to total words * 100
    float lds = ((float)engine.distinct_words / (float)engine.total_words) * 100.0f;

    printf("\n------------------------------------------------------\n");
    printf("CUSTOM ANALYSIS: LEXICAL DIVERSITY SCORE (LDS)\n");
    printf("--------------------------------------------------------\n");
    printf("Formula       : (Distinct Words / Total Words) * 100\n");
    printf("Calculation   : (%zu / %d) * 100\n", engine.distinct_words, engine.total_words);
    printf("LDS Score     : %.2f%%\n", lds);
    printf("A higher percentage means a rich vocabulary, a lower score means repetitions.\n");
}

void generate_text_summary(void) {
    print_header();
    if (engine.total_words == 0) {
        printf("\nNo text data available for summary.\n");
        return;
    }

    int total_chars = 0;
    for (size_t i = 0; i < engine.distinct_words; i++) {
        total_chars += (int)(strlen(engine.stats[i].name) * engine.stats[i].count);
    }
    float avg_len = (float)total_chars / (float)engine.total_words;

    //longest word
    size_t longest_idx = 0;
    for (size_t i = 1; i < engine.distinct_words; i++) {
        if (strlen(engine.stats[i].name) > strlen(engine.stats[longest_idx].name)) longest_idx = i;
    }

    //most frequent word
    size_t freq_idx = 0;
    for (size_t i = 1; i < engine.distinct_words; i++) {
        if (engine.stats[i].count > engine.stats[freq_idx].count) freq_idx = i;
    }

    float lds = ((float)engine.distinct_words / (float)engine.total_words) * 100.0f;

    printf("\n---------------------------------------------\n");
    printf("TEXT SUMMARY REPORT\n");
    printf("-----------------------------------------------\n");
    printf("Total Word Count       : %d\n", engine.total_words);
    printf("Distinct Word Count    : %zu\n", engine.distinct_words);
    printf("Average Word Length    : %.2f characters\n", avg_len);
    printf("Longest Word           : %s (%zu chars)\n", engine.stats[longest_idx].name, strlen(engine.stats[longest_idx].name));
    printf("Most Frequent Word     : %s (%d occurrences)\n", engine.stats[freq_idx].name, engine.stats[freq_idx].count);
    printf("Lexical Diversity Score: %.2f%%\n", lds);
}

void exit_program(void) {
    print_header();
    free(engine.text_buffer);
    free(engine.stats);
    printf("Memory successfully freed. Exiting program.\n");
    exit(0);
}
