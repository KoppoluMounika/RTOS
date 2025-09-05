#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // Use PRO CPU when single-core
#else
  static const BaseType_t app_cpu = 1;   // Use APP CPU when dual-core
#endif

void testTask(void *parameter) {
  while (1) {
    int a = 1;
    int b[100];
    for (int i = 0; i < 100; i++) {
      b[i] = a + i;                     
      //Serial.println(b[i]);     //print 1-100 
    }
    //printout remaining stack memory(in words)
    Serial.print("High water mark(words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));  
    
    //print out numbers of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());  

    int *ptr=(int*)pvPortMalloc(1024 * sizeof(int)); 
    if (ptr==NULL){
      Serial.println("not enough heap");
    }
    else{
      for(int i=0;i<1024;i++){
        ptr[i]=3;
      }
    }

    //print out numbers of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());  

    vPortFree(ptr);//free the pointer

    vTaskDelay(1000 / portTICK_PERIOD_MS); // fixed macro name
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("---FreeRTOS Memory Demo----");

  // increased stack size to 2048 for safety
  xTaskCreatePinnedToCore(
    testTask,       // Task function
    "Test Task",    // Task name
    2048,           // Stack size in words
    NULL,           // Parameters
    1,              // Priority
    NULL,           // Task handle
    app_cpu         // CPU core to run on
  );

  vTaskDelete(NULL);  // delete setup task
}

void loop() {
  
}
