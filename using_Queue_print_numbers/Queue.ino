#include <Arduino.h>   // include Arduino core

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // run on PRO CPU if only 1 core available
#else
  static const BaseType_t app_cpu = 1;   // run on APP CPU if dual core
#endif
static const uint8_t msg_queue_len=5;    // max number of items the queue can hold
//global variable
static QueueHandle_t msg_queue;          // handle for the queue
// task function to print items received from queue
void printMessages(void *parameters){
  int item;  // variable to store received value

  while(1){  
    if(xQueueReceive(msg_queue,(void *)&item,0)==pdTRUE){  
      //receive item from queue
      Serial.println(item);  // print the received number
    }
    vTaskDelay(1000/portTICK_PERIOD_MS); // delay 1 second
  }
}

 

void setup() {
  Serial.begin(115200);                   
  vTaskDelay(1000 / portTICK_PERIOD_MS);  
  Serial.println();
  Serial.println("---FreeRTOS QUEUE Demo----"); 

  msg_queue=xQueueCreate(msg_queue_len,sizeof(int));  
  // create queue with capacity = msg_queue_len, item size = int
  xTaskCreatePinnedToCore(
    printMessages,       // function to run
    "print Messages",    // task name
    1024,                // stack size in words
    NULL,                // parameters passed to task
    1,                   // priority (low = 1)
    NULL,                // no task handle needed
    app_cpu              // run on app_cpu
  );
}
void loop() {
  static int num=0;  // counter that increases each loop

  if(xQueueSend(msg_queue,(void *)&num,10)!=pdTRUE){
    // if queue is full, print warning
    Serial.println("Queue Full");
  }
  num++;  // increment number
  vTaskDelay(1000/portTICK_PERIOD_MS);  // wait 100 ms 
}
