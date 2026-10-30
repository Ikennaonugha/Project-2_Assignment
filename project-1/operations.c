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
