#include "iot_engine.h"

typedef void (*MenuAction)(void);

void exit_app(void) {
    cleanup_engine();
    exit(0);
}

int main(void) {
    //initialize
    init_iot_engine();

    MenuAction menu_actions[] = {
        NULL,//unused
        register_device_interactive,
        display_devices,
        remove_device,
        simulate_readings,
        display_alerts,
        generate_summary,
        exit_app
    };

    int choice;
    while (1) {
        printf("\n--- IOT EVENT MONITORING & CALLBACK ENGINE ---\n");
        printf("[1] Register a new device\n");
        printf("[2] Display all registered devices\n");
        printf("[3] Remove a device by ID\n");
        printf("[4] Simulate readings & execute callbacks\n");
        printf("[5] Display current alerts\n");
        printf("[6] Generate monitoring summary report\n");
        printf("[7] Exit program and free memory\n");
        printf("----------------------------------------------------\n");
        printf("Enter your choice (1-7): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        if (choice >= 1 && choice <= 7) {
            menu_actions[choice]();
        } else {
            printf("Invalid selection. Choose between 1 and 7.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}
