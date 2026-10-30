#include "student.h"

int main(){
  //initialize dataset with 10 students
  initial_dataset();

  //function pointer
  OperationFunc operations[] = {
        add_student,
        display_students,
        sort_students,
        calculate_average,
        find_top_student,
        search_student,
        remove_student,
        custom_analysis,
        generate_summary_report,
        exit_program
    };

    int choice;
    while (1) {
        print_header();
        printf("\n--- MAIN MENU ---\n");
        printf("[0] Add a new student record\n");
        printf("[1] Display all student records\n");
        printf("[2] Sort students (Grade, Name, ID)\n");
        printf("[3] Calculate average grade\n");
        printf("[4] Identify top-performing student\n");
        printf("[5] Search for a student (ID or Name)\n");
        printf("[6] Remove a student record\n");
        printf("[7] Run custom academic analysis (ARI)\n");
        printf("[8] Display summary report\n");
        printf("[9] Exit program\n");
        printf("-----------------------------\n");
        printf("Enter your choice (0-9): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid integer.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice >= 0 && choice <= 9) {
            operations[choice]();
        } else {
            printf("Invalid selection. Please choose between 0 and 9.\n");
        }

        printf("\nPress Enter to continue..");
        getchar();
    }

    return 0;
}
