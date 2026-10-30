#ifndef IOT_ENGINE_H
#define IOT_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TYPE_TEMP 1
#define TYPE_PRESSURE 2
#define TYPE_BATTERY 3

#define STATUS_NORMAL 0
#define STATUS_ALERT 1

#define INITIAL_DEVICE_CAPACITY 5

typedef struct {
    float previous_reading;
    int has_previous;
    float max_allowed_drift;
} CustomData;

struct Device {
    int id;
    char name[40];
    int type; 
    union {
        float temperature;
        int pressure;
        float voltage;
    } reading;
    int status; //0 for normal, 1 for alert
    CustomData custom_data;
};

typedef struct {
    struct Device *devices;
    size_t count;
    size_t capacity;
    int total_readings_processed;
    int normal_readings_count;
    int abnormal_readings_count;
    int alerts_by_type[4];
    int custom_alerts_count;
} IoTEngine;

extern IoTEngine engine;

typedef void (*DeviceCallback)(struct Device *);

void init_iot_engine(void);
void register_device_interactive(void);
void register_device(int id, const char *name, int type);
void display_devices(void);
void remove_device(void);
void simulate_readings(void);
void process_device(struct Device *device, DeviceCallback callback);
void temperature_monitor(struct Device *d);
void pressure_monitor(struct Device *d);
void battery_monitor(struct Device *d);
void sensor_drift_detector(struct Device *d); // Custom callback
void display_alerts(void);
void generate_summary(void);
void cleanup_engine(void);

#endif
