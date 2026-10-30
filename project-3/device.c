#include "iot_engine.h"

IoTEngine engine = {NULL, 0, 0, 0, 0, 0, {0}, 0};

void init_iot_engine(void) {
    engine.capacity = INITIAL_DEVICE_CAPACITY;
    engine.devices = malloc(engine.capacity * sizeof(struct Device));
    if (!engine.devices) {
        perror("Failed to allocate device array");
        exit(1);
    }
    engine.count = 0;
    srand((unsigned int)time(NULL));

    register_device(101, "Server-Room-Temp-1", TYPE_TEMP);
    register_device(102, "Hydraulic-Press-A", TYPE_PRESSURE);
    register_device(103, "Backup-Battery-Bank", TYPE_BATTERY);
}

void register_device(int id, const char *name, int type) {
    //check for duplicate ID
    for (size_t i = 0; i < engine.count; i++) {
        if (engine.devices[i].id == id) {
            printf("Error: Device ID %d already exists\n", id);
            return;
        }
    }

    //expand if full using realloc
    if (engine.count >= engine.capacity) {
        size_t new_cap = engine.capacity * 2;
        struct Device *temp = realloc(engine.devices, new_cap * sizeof(struct Device));
        if (!temp) {
            perror("Memory reallocation failed");
            return;
        }
        engine.devices = temp;
        engine.capacity = new_cap;
    }

    struct Device *d = &engine.devices[engine.count];
    d->id = id;
    snprintf(d->name, sizeof(d->name), "%s", name);
    d->type = type;
    d->status = STATUS_NORMAL;
    d->custom_data.has_previous = 0;
    d->custom_data.max_allowed_drift = (type == TYPE_TEMP) ? 5.0f : (type == TYPE_PRESSURE) ? 50.0f : 0.5f;

    //initialize union reading to 0
    if (type == TYPE_TEMP) d->reading.temperature = 0.0f;
    else if (type == TYPE_PRESSURE) d->reading.pressure = 0;
    else if (type == TYPE_BATTERY) d->reading.voltage = 0.0f;

    engine.count++;
    printf("Successfully registered device: [%d] %s\n", id, name);
}

void register_device_interactive(void) {
    int id, type;
    char name[40];

    printf("\n--- Register New IoT Device ---\n");
    printf("Enter Device ID (integer): ");
    if (scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    while(getchar() != '\n');

    printf("Enter Device Name: ");
    if (fgets(name, sizeof(name), stdin)) {
        name[strcspn(name, "\n")] = '\0';
    }

    printf("Select Device Type:\n");
    printf("[1] Temperature Sensor\n");
    printf("[2] Pressure Sensor\n");
    printf("[3] Battery Voltage Monitor\n");
    printf("Enter choice (1-3): ");
    if (scanf("%d", &type) != 1 || type < 1 || type > 3) {
        printf("Invalid device type selected.\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');

    register_device(id, name, type);
}

void display_devices(void) {
    printf("\nREGISTERED IoT DEVICES (%zu active)\n", engine.count);
    printf("-------------------------------------------------------------------\n");
    if (engine.count == 0) {
        printf("No devices currently registered.\n");
        return;
    }
    printf(" %-6s  %-25s  %-15s  %-10s\n", "ID", "Name", "Type", "Status");
    printf("-------------------------------------------------------------------\n");
    for (size_t i = 0; i < engine.count; i++) {
        struct Device *d = &engine.devices[i];
        const char *t_str = (d->type == TYPE_TEMP) ? "Temperature" : 
                            (d->type == TYPE_PRESSURE) ? "Pressure" : "Battery Monitor";
        const char *s_str = (d->status == STATUS_ALERT) ? "ALERT!" : "Normal";
        printf(" %-6d  %-25s  %-15s  %-10s\n", d->id, d->name, t_str, s_str);
    }
    printf("-------------------------------------------------------------------\n");
    printf("Memory address of device array base : %p\n", (void *)engine.devices);
    if (engine.count > 0) {
        printf("Memory address of device entry [0]  : %p\n", (void *)&engine.devices[0]);
    }
}

void remove_device(void) {
    if (engine.count == 0) {
        printf("\nNo devices to remove.\n");
        return;
    }

    int target_id;
    printf("\nEnter Device ID to remove: ");
    if (scanf("%d", &target_id) != 1) { while(getchar() != '\n'); return; }
    while(getchar() != '\n');

    int found_idx = -1;
    for (size_t i = 0; i < engine.count; i++) {
        if (engine.devices[i].id == target_id) {
            found_idx = (int)i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Device ID %d not found.\n", target_id);
        return;
    }

    //shift elements down to delete
    for (size_t i = found_idx; i < engine.count - 1; i++) {
        engine.devices[i] = engine.devices[i + 1];
    }
    engine.count--;

    //shrink allocation if capacity allows
    if (engine.count > 0 && engine.count <= engine.capacity / 2 && engine.capacity > INITIAL_DEVICE_CAPACITY) {
        engine.capacity /= 2;
        struct Device *temp = realloc(engine.devices, engine.capacity * sizeof(struct Device));
        if (temp) engine.devices = temp;
    }

    printf("Device ID %d successfully removed.\n", target_id);
}

void process_device(struct Device *device, DeviceCallback callback) {
    engine.total_readings_processed++;
    callback(device);
}

void temperature_monitor(struct Device *d) {
    float val = d->reading.temperature;
    if (val < 10.0f || val > 55.0f) {
        d->status = STATUS_ALERT;
        engine.abnormal_readings_count++;
        engine.alerts_by_type[TYPE_TEMP]++;
        printf("[ALERT] Temp Sensor [%d] %s: %.2f°C out of range [10.0 - 55.0]!\n", d->id, d->name, val);
    } else {
        d->status = STATUS_NORMAL;
        engine.normal_readings_count++;
    }
}

void pressure_monitor(struct Device *d) {
    int val = d->reading.pressure;
    if (val < 850 || val > 1150) {
        d->status = STATUS_ALERT;
        engine.abnormal_readings_count++;
        engine.alerts_by_type[TYPE_PRESSURE]++;
        printf("[ALERT] Pressure Sensor [%d] %s: %d hPa out of range [850 - 1150]!\n", d->id, d->name, val);
    } else {
        d->status = STATUS_NORMAL;
        engine.normal_readings_count++;
    }
}

void battery_monitor(struct Device *d) {
    float val = d->reading.voltage;
    if (val < 3.1f) {
        d->status = STATUS_ALERT;
        engine.abnormal_readings_count++;
        engine.alerts_by_type[TYPE_BATTERY]++;
        printf("[ALERT] Battery Monitor [%d] %s: Critical voltage %.2fV (< 3.1V)!\n", d->id, d->name, val);
    } else {
        d->status = STATUS_NORMAL;
        engine.normal_readings_count++;
    }
}

void sensor_drift_detector(struct Device *d) {
    float current_val = 0.0f;
    if (d->type == TYPE_TEMP) current_val = d->reading.temperature;
    else if (d->type == TYPE_PRESSURE) current_val = (float)d->reading.pressure;
    else if (d->type == TYPE_BATTERY) current_val = d->reading.voltage;

    if (d->custom_data.has_previous) {
        float diff = fabsf(current_val - d->custom_data.previous_reading);
        if (diff > d->custom_data.max_allowed_drift) {
            engine.custom_alerts_count++;
            printf("[DRIFT ALERT] Device [%d] %s experienced rapid fluctuation! Delta: %.2f (Threshold: %.2f)\n", 
                   d->id, d->name, diff, d->custom_data.max_allowed_drift);
        }
    }

    d->custom_data.previous_reading = current_val;
    d->custom_data.has_previous = 1;
}

void simulate_readings(void) {
    if (engine.count == 0) {
        printf("\nNo devices available to simulate readings.\n");
        return;
    }

    printf("\n--- Simulating IoT Device Readings ---\n");
    for (size_t i = 0; i < engine.count; i++) {
        struct Device *d = &engine.devices[i];

        // Generate reading based on device type using union safety rules
        if (d->type == TYPE_TEMP) {
            d->reading.temperature = -5.0f + ((float)rand() / (float)(RAND_MAX)) * 70.0f;
            process_device(d, temperature_monitor);
        } else if (d->type == TYPE_PRESSURE) {
            d->reading.pressure = 800 + (rand() % 401);
            process_device(d, pressure_monitor);
        } else if (d->type == TYPE_BATTERY) {
            d->reading.voltage = 2.8f + ((float)rand() / (float)(RAND_MAX)) * 1.5f;
            process_device(d, battery_monitor);
        }

        // Run custom callback as well
        sensor_drift_detector(d);
    }
    printf("Simulation complete across all %zu devices!\n", engine.count);
}

void display_alerts(void) {
    printf("\n-------------------------------------------------------------------\n");
    printf(" CURRENT DEVICE ALERTS & STATUS REPORT\n");
    printf("-------------------------------------------------------------------\n");
    int alert_count = 0;
    for (size_t i = 0; i < engine.count; i++) {
        if (engine.devices[i].status == STATUS_ALERT) {
            alert_count++;
            struct Device *d = &engine.devices[i];
            printf(" Device ID   : %d\n", d->id);
            printf(" Name        : %s\n", d->name);
            printf(" Type        : %d\n", d->type);
            if (d->type == TYPE_TEMP) printf(" Value       : %.2f °C\n", d->reading.temperature);
            else if (d->type == TYPE_PRESSURE) printf(" Value       : %d hPa\n", d->reading.pressure);
            else if (d->type == TYPE_BATTERY) printf(" Value       : %.2f V\n", d->reading.voltage);
            printf(" Status      : ALERT TRIGGERED\n");
            printf("---------------------------------------------------\n");
        }
    }
    if (alert_count == 0) {
        printf("No active alerts detected. All systems operating normally.\n");
    }
}

void generate_summary(void) {
    printf("\n-------------------------------------------------------------------\n");
    printf(" IOT MONITORING ENGINE SUMMARY REPORT\n");
    printf("-------------------------------------------------------------------\n");
    printf(" Total Registered Devices    : %zu\n", engine.count);
    printf(" Total Readings Processed    : %d\n", engine.total_readings_processed);
    printf(" Normal Readings Count       : %d\n", engine.normal_readings_count);
    printf(" Abnormal Readings Count     : %d\n", engine.abnormal_readings_count);
    printf(" Alerts by Type:\n");
    printf("   - Temperature Alerts      : %d\n", engine.alerts_by_type[TYPE_TEMP]);
    printf("   - Pressure Alerts         : %d\n", engine.alerts_by_type[TYPE_PRESSURE]);
    printf("   - Battery Voltage Alerts  : %d\n", engine.alerts_by_type[TYPE_BATTERY]);
    printf(" Custom Drift Alerts Triggered : %d\n", engine.custom_alerts_count);
    printf("-------------------------------------------------------------------\n");
}

void cleanup_engine(void) {
    if (engine.devices) {
        free(engine.devices);
        engine.devices = NULL;
    }
    printf("Memory successfully freed.\n");
}
