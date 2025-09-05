

/*Task to blink the LED based on delay value passed as a parameter*/
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;   // If only one core, use PRO CPU (core 0)
#else
  static const BaseType_t app_cpu = 1;   // Otherwise, use to APP CPU (core 1)
#endif
static const int led_pin = 2;//led pin will be connected to pin 2 in esp32
static SemaphoreHandle_t bin_sem;//global semaphore

void blinkLED(void *parameters) {
  int num = *(int *)parameters;
  xSemaphoreGive(bin_sem);
  Serial.print("Received: ");// Print the value we received from setup()
  Serial.println(num);
  pinMode(led_pin, OUTPUT);//led pin is the output

  while (1) {
    digitalWrite(led_pin, HIGH);                  // Turn LED ON
    vTaskDelay(num / portTICK_PERIOD_MS);         // Wait for "num" ms
    digitalWrite(led_pin, LOW);                   // Turn LED OFF
    vTaskDelay(num / portTICK_PERIOD_MS);         // Wait again
  }
}

void setup() {

  long int delay_arg;   // blink delay in ms

  // Start Serial communication
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Mutex Solution---");
  Serial.println("Enter a number for delay (milliseconds)");

  // Wait until the user enters 
  while (Serial.available() <= 0);

  // Read the integer value 
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);
  
  // Create the binary semaphore
  bin_sem = xSemaphoreCreateBinary();

  // Create the blinkLED task
  xTaskCreatePinnedToCore(
                          blinkLED,       // Task function
                          "Blink LED",    // Task name
                          1024,           // Stack size in words
                          (void *)&delay_arg, // Pass address of our variable
                          1,              // Task priority
                          NULL,           // Task handle (not needed here)
                          app_cpu);       // Core to pin the task to

  xSemaphoreTake(bin_sem, portMAX_DELAY);
  Serial.println("Done!");
}
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
