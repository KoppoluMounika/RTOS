#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // Use PRO CPU when single-core
#else
  static const BaseType_t app_cpu = 1;   // Use APP CPU when dual-core
#endif

static int shared_var = 0;               // shared global variable
static SemaphoreHandle_t mutex;          // mutex handle

void incTask(void *parameter) {
  int local_var;
  while (1) {
    // try to take mutex immediately (timeout = 0 ticks)
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {
      local_var = shared_var;                          // read global
      local_var++;                                     // increment
      vTaskDelay(random(3000,5000)/portTICK_PERIOD_MS);// random delay
      shared_var = local_var;                          // write back
      xSemaphoreGive(mutex);                           // release mutex

      Serial.println(shared_var);                      // print value
    } else {
      // if mutex not available, wait a little before retry
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("---Race condition Demo with Mutex----");

  // create mutex before tasks
  mutex = xSemaphoreCreateMutex();

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
