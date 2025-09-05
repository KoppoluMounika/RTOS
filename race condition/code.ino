#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // Use PRO CPU when single-core
#else
  static const BaseType_t app_cpu = 1;   // Use APP CPU when dual-core
#endif

volatile int shared_var = 0;  // shared global variable

void incTask(void *parameter) {
  int local_var;
  while (1) {
    local_var = shared_var;                           // read global
    local_var++;                                      // increment
    vTaskDelay(random(3000,5000)/portTICK_PERIOD_MS);   // random delay
    shared_var = local_var;                           // write back
    Serial.println(shared_var);                       // print value
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("---Race condition Demo----");

  // create first increment task
  xTaskCreatePinnedToCore(
    incTask,              // Task function
    "Increment Task 1",   // Task name
    1024,                 // Stack size in words
    NULL,                 // Parameters
    1,                    // Priority
    NULL,                 // Task handle
    app_cpu               // CPU core to run on
  );

  // create second increment task
  xTaskCreatePinnedToCore(
    incTask,              // Task function
    "Increment Task 2",   // Task name
    2048,                 // Stack size in words
    NULL,                 // Parameters
    1,                    // Priority
    NULL,                 // Task handle
    app_cpu               // CPU core to run on
  );

  vTaskDelete(NULL);  // delete setup task
}

void loop() {
  // nothing here
}
