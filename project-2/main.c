#include "text_analyzer.h"

int main(void) {
    //initialize
    init_engine();

    //arrray of function pointers
    AnalysisFunc operations[] = {
        input_custom_text,
        count_words,
        longest_word,
        most_frequent,
        display_word_statistics,
        sort_word_statistics,
        search_word,
        memory_address_inspection,
        custom_linguistic_analysis,
        generate_text_summary,
        exit_program
    };

    int choice;
    while (1) {
        print_header();
        printf("\n--- TEXT MINING MENU ---\n");
        printf("[0] Input text paragraph\n");
        printf("[1] Count total and distinct words\n");
        printf("[2] Identify longest word and length\n");
        printf("[3] Identify most frequent word\n");
        printf("[4] Display distinct word statistics table\n");
        printf("[5] Sort word statistics\n");
        printf("[6] Search for a specific word\n");
        printf("[7] Inspect memory addresses (%p)\n");
        printf("[8] Custom Analysis (Lexical Diversity Score)\n");
        printf("[9] Summary report\n");
        printf("[10] Exit program\n");
        printf("------------------------------------------------------\n");
        printf("Enter your choice (0-10): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n'); // clear buffer

        if (choice >= 0 && choice <= 10) {
            operations[choice]();
        } else {
            printf("Invalid selection. Please choose between 0 and 10.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}
