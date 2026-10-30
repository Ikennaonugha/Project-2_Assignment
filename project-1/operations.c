#include "student.h"

StudentRegistry registry = {NULL, 0, 0};

void initial_dataset(void) {
    registry.capacity = INITIAL_CAPACITY;
    registry.data = malloc(registry.capacity * sizeof(Student *));
    if (registry.data == NULL) {
        perror("Critical: Initial memory allocation failed");
        exit(1);
    }

    int initial_ids[10] = {1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010};
    const char *names[10] = {"Harry Kane", "Kylian Mbappe", "Lamine Yamal", "Rodri", "Alexia Putellas", 
                               "Khvicha Kvaratskhelia", "Ewa Pajor", "Erling Halland", "Michael Olise", "Khadija Shaw"};
    const char courses[] = {"C Programming"};
    float grades[10] = {88.5f, 45.0f, 72.0f, 91.5f, 63.0f, 38.5f, 79.0f, 84.0f, 55.5f, 95.0f};

    for (int i = 0; i < 10; i++) {
        registry.data[i] = malloc(sizeof(Student));
        if (registry.data[i] == NULL) {
            perror("Memory allocation failed for student record");
            exit(1);
        }
        registry.data[i]->id = initial_ids[i];
        snprintf(registry.data[i]->name, MAX_STR, "%s", names[i]);
        snprintf(registry.data[i]->course, MAX_STR, "%s", courses);
        registry.data[i]->grade = grades[i];
    }
    registry.size = 10;
}

//to prevent duplicate ids, done recursively
int id_exists(int id, size_t idx) {
    if (idx >= registry.size)
        return 0;
    if (registry.data[idx]->id == id)
        return 1;
    return id_exists(id, idx + 1);
}

//to clear the input buffer
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//output header
void print_header(void) {
    printf("\nStudent ID: 1001 (I don't know my student ID)\n");
    printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index (ARI)\n");
}

void add_student(void) {
    print_header();
    printf("\n--- Add New Student Record ---\n");
    
    int id;
    printf("Enter Unique Student ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (id_exists(id, 0)) {
        printf("Error: Student ID %d already exists!\n", id);
        return;
    }

    char name[MAX_STR], course[MAX_STR];
    printf("Enter Full Name: ");
    if (fgets(name, sizeof(name), stdin) != NULL) {
        name[strcspn(name, "\n")] = '\0';
    }

    printf("Enter Course: ");
    if (fgets(course, sizeof(course), stdin) != NULL) {
        course[strcspn(course, "\n")] = '\0';
    }

    float grade;
    printf("Enter Grade: ");
    if (scanf("%f", &grade) != 1 || grade < 0.0f || grade > 100.0f) {
        printf("Error: Invalid grade! Must be between 0 and 100.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (registry.size >= registry.capacity) {
        size_t new_capacity = registry.capacity + 5;
        Student **temp = realloc(registry.data, new_capacity * sizeof(Student *));
        if (temp == NULL) {
            perror("Memory reallocation failed");
            return;
        }
        registry.data = temp;
        registry.capacity = new_capacity;
    }

    registry.data[registry.size] = malloc(sizeof(Student));
    if (registry.data[registry.size] == NULL) {
        perror("Memory allocation failed for new student");
        return;
    }

    registry.data[registry.size]->id = id;
    snprintf(registry.data[registry.size]->name, MAX_STR, "%s", name);
    snprintf(registry.data[registry.size]->course, MAX_STR, "%s", course);
    registry.data[registry.size]->grade = grade;
    registry.size++;

    printf("Success: Student record added. Total records: %zu\n", registry.size);
}

void display_students(void) {
    print_header();
    printf("\n--- All Student Records ---\n");
    printf("-------------------------------------------------------------------\n");
    printf(" %-6s | %-20s | %-20s | %-6s \n", "ID", "Name", "Course", "Grade");
    printf("-------------------------------------------------------------------\n");

    for (size_t i = 0; i < registry.size; i++) {
        Student *s = *(registry.data + i);
        printf(" %-6d | %-20s | %-20s | %6.2f \n", s->id, s->name, s->course, s->grade);
    }
}

void sort_students(void) {
    print_header();
    printf("\n--- Sort Students ---\n");
    printf("[1] Sort by Grade\n");
    printf("[2] Sort by Name\n");
    printf("[3] Sort by Student ID\n");
    printf("Select sorting choice: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (size_t i = 0; i < registry.size; i++) {
        for (size_t j = i + 1; j < registry.size; j++) {
            Student *s1 = registry.data[i];
            Student *s2 = registry.data[j];
            int condition = 0;

            if (choice == 1) {
                condition = (s1->grade < s2->grade);
            } else if (choice == 2) {
                condition = (strcmp(s1->name, s2->name) > 0);
            } else if (choice == 3) {
                condition = (s1->id > s2->id);
            }

            if (condition) {
                Student *temp = registry.data[i];
                registry.data[i] = registry.data[j];
                registry.data[j] = temp;
            }
        }
    }
    printf("Students successfully sorted\n");
    display_students();
}

void calculate_average(void) {
    print_header();
    if (registry.size == 0) {
        printf("\nNo student records available.\n");
        return;
    }

    float sum = 0.0f;
    for (size_t i = 0; i < registry.size; i++) {
        sum += (*(registry.data + i))->grade;
    }
    float avg = sum / (float)registry.size;
    printf("\nClass Average Grade: %.2f across %zu students.\n", avg, registry.size);
}

void find_top_student(void) {
    print_header();
    if (registry.size == 0) {
        printf("\nNo student records available.\n");
        return;
    }

    size_t top_idx = 0;
    for (size_t i = 1; i < registry.size; i++) {
        if ((*(registry.data + i))->grade > registry.data[top_idx]->grade) {
            top_idx = i;
        }
    }

    Student *top = registry.data[top_idx];
    printf("\n--- Top-Performing Student ---\n");
    printf("Name: %s\n", top->name);
    printf("ID: %d\n", top->id);
    printf("Course: %s\n", top->course);
    printf("Grade: %.2f\n", top->grade);
}

void search_student(void) {
    print_header();
    printf("\n--- Search Student ---\n");
    printf("[1] Search by ID\n");
    printf("[2] Search by Name\n");
    printf("Select option: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (choice == 1) {
        int target_id;
        printf("Enter Student ID: ");
        scanf("%d", &target_id);
        clear_input_buffer();

        int found = 0;
        for (size_t i = 0; i < registry.size; i++) {
            if (registry.data[i]->id == target_id) {
                Student *s = registry.data[i];
                printf("\n--- Found ---\nID: %d \nName: %s \nCourse: %s \nGrade: %.2f\n", s->id, s->name, s->course, s->grade);
                found = 1;
                break;
            }
        }
        if (!found)
            printf("Student with ID %d not found.\n", target_id);

    } else if (choice == 2) {
        char target_name[MAX_STR];
        printf("Enter Name: ");
        fgets(target_name, sizeof(target_name), stdin);
        target_name[strcspn(target_name, "\n")] = '\0';

        int found = 0;
        for (size_t i = 0; i < registry.size; i++) {
            if (strstr(registry.data[i]->name, target_name) != NULL) {
                Student *s = registry.data[i];
                printf("\n--- Found ---\nID: %d \nName: %s \nCourse: %s \nGrade: %.2f\n", s->id, s->name, s->course, s->grade);
                found = 1;
            }
        }
        if (!found)
            printf("No student named '%s' found.\n", target_name);
    }
}

void remove_student(void) {
    print_header();
    printf("\n--- Remove Student Record ---\n");
    int target_id;
    printf("Enter Student ID to remove: ");
    if (scanf("%d", &target_id) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int found_idx = -1;
    for (size_t i = 0; i < registry.size; i++) {
        if (registry.data[i]->id == target_id) {
            found_idx = (int)i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Student ID %d not found.\n", target_id);
        return;
    }

    free(registry.data[found_idx]);

    for (size_t i = found_idx; i < registry.size - 1; i++) {
        registry.data[i] = registry.data[i + 1];
    }
    registry.size--;

    if (registry.capacity > INITIAL_CAPACITY && registry.size < registry.capacity - 5) {
        size_t new_capacity = registry.capacity - 5;
        Student **temp = realloc(registry.data, new_capacity * sizeof(Student *));
        if (temp != NULL) {
            registry.data = temp;
            registry.capacity = new_capacity;
        }
    }

    printf("Student record removed successfully. Total records: %zu\n", registry.size);
}

void custom_analysis(void) {
    print_header();
    printf("\n-----------------------------------------\n");
    printf(" CUSTOM ANALYSIS: ACADEMIC RISK INDEX (ARI)\n");
    printf("------------------------------------------\n");
    printf(" %-6s  %-20s  %-6s  %-10s  %-15s \n", "ID", "Name", "Grade", "ARI Score", "Risk Category");
    printf("-------------------------------------------------------------------\n");

    for (size_t i = 0; i < registry.size; i++) {
        Student *s = registry.data[i];
        float deficit = 100.0f - s->grade;
        float multiplier = (s->grade < 50.0f) ? 1.5f : 1.0f;
        float ari = deficit * multiplier;

        const char *category = (ari > 50.0f) ? "HIGH RISK" : (ari > 20.0f ? "MODERATE" : "LOW RISK");

        printf(" %-6d  %-20s  %6.2f  %10.2f  %-15s \n", s->id, s->name, s->grade, ari, category);
    }
}

void generate_summary_report(void) {
    print_header();
    if (registry.size == 0) {
        printf("\nNo records available to summarize.\n");
        return;
    }

    float sum = 0.0f;
    float highest = registry.data[0]->grade;
    float lowest = registry.data[0]->grade;
    int pass_count = 0;
    int fail_count = 0;
    float pass_threshold = 50.0f;

    for (size_t i = 0; i < registry.size; i++) {
        float g = registry.data[i]->grade;
        sum += g;
        if (g > highest) highest = g;
        if (g < lowest) lowest = g;
        if (g >= pass_threshold) pass_count++;
        else fail_count++;
    }

    float average = sum / (float)registry.size;

    printf("\n----------------------------------------\n");
    printf(" SUMMARY REPORT\n");
    printf("------------------------------------------\n");
    printf(" Total Student Records  : %zu\n", registry.size);
    printf(" Current Memory Capacity: %zu slots\n", registry.capacity);
    printf(" Class Average Grade    : %.2f\n", average);
    printf(" Highest Grade          : %.2f\n", highest);
    printf(" Lowest Grade           : %.2f\n", lowest);
    printf(" Passing Students: %d\n", pass_count);
    printf(" At-Risk Students: %d\n", fail_count);
    printf("------------------------------------------\n");
    
    custom_analysis();
}

void exit_program(void) {
    print_header();
    printf("\nReleasing all dynamically allocated heap memory\n");
    for (size_t i = 0; i < registry.size; i++) {
        free(registry.data[i]);
    }
    free(registry.data);
    printf("Memory successfully freed. Exiting program.\n");
    exit(0);
}
