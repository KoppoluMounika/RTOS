#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // If single core, use core 0
#else
  static const BaseType_t app_cpu = 1;   // Otherwise use core 1
#endif

// Timer 
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;


// timer "fires"
void myTimerCallback(TimerHandle_t xTimer) {
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
    Serial.println("One-shot timer expired");   // Print for one-shot timer
  }

  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    Serial.println("Auto-reload timer expired"); // Print for auto-reload timer
  }
}
void setup() {

  //Serial monitor
  Serial.begin(115200);

  //delay 
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Timer Demo---");

  // Create a one-shot timer (runs only once)
  one_shot_timer = xTimerCreate(
                      "One-shot timer",           // Name 
                      2000 / portTICK_PERIOD_MS,  // Time period = 2000 ms
                      pdFALSE,                    // pdFALSE = one-shot
                      (void *)0,                  // ID = 0
                      myTimerCallback);           // Function to call

  // Create an auto-reload timer (runs again and again)
  auto_reload_timer = xTimerCreate(
                      "Auto-reload timer",        // Name 
                      1000 / portTICK_PERIOD_MS,  // Time period = 1000 ms
                      pdTRUE,                     // pdTRUE = auto-reload
                      (void *)1,                  // ID = 1
                      myTimerCallback);           // Function to call

  if (one_shot_timer == NULL || auto_reload_timer == NULL) {
    Serial.println("Could not create one of the timers");
  } else {
    
    //delay 
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");

    // Start both timers
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }
  vTaskDelete(NULL);
}
void loop() {
 
}
