#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // Run tasks on single core ESP32
#else
  static const BaseType_t app_cpu = 1;   // Run tasks on core 1 
#endif

// Settings
TickType_t cs_wait = 250;    // Time (ms) each task spends inside the critical section
TickType_t med_wait = 5000;  // Time (ms) medium-priority task spends 

// Globals variable
static SemaphoreHandle_t lock; 
// Low priority
void doTaskL(void *parameters) {
  TickType_t timestamp;

  while (1) {
    Serial.println("Task L trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;

    // Take the mutex
    xSemaphoreTake(lock, portMAX_DELAY);
    Serial.print("Task L got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    //cs_wait ms
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait);

    //mutex released
    Serial.println("Task L releasing lock.");
    xSemaphoreGive(lock);

    // delay 2 sec
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
//Medium priority
void doTaskM(void *parameters) {
  TickType_t timestamp;

  while (1) {
    Serial.println("Task M doing some work...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < med_wait);

    Serial.println("Task M done!");

    //delay 2 sec
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
//High priority
void doTaskH(void *parameters) {
  TickType_t timestamp;

  while (1) {
    Serial.println("Task H trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);
    Serial.print("Task H got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work");

    //cs_wait ms
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait);
    //mutex released
    Serial.println("Task H releasing lock.");
    xSemaphoreGive(lock);

    //delay 2 seconds
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
  Serial.println();
  Serial.println("--- FreeRTOS Priority Inheritance Demo ---");
  lock = xSemaphoreCreateMutex();

  // Start Task L (low priority = 1)
  xTaskCreatePinnedToCore(doTaskL, "Task L", 1024, NULL, 1, NULL, app_cpu);
  vTaskDelay(1 / portTICK_PERIOD_MS);

  // Start Task H (high priority = 3)
  xTaskCreatePinnedToCore(doTaskH, "Task H", 1024, NULL, 3, NULL, app_cpu);

  // Start Task M (medium priority = 2)
  xTaskCreatePinnedToCore(doTaskM, "Task M", 1024, NULL, 2, NULL, app_cpu);

  // Delete Arduino's default setup/loop task
  vTaskDelete(NULL);
}
void loop() {
  
}
