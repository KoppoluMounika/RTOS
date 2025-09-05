#include <Arduino.h>
// Timer handle
hw_timer_t *timer = NULL;

volatile int isrCounter = 0;  //isrcounter will be 0
volatile bool isrFlag = false;  //flag is in false condition

// ISR function
void IRAM_ATTR onTimer() {
  isrCounter++;         // increment the counter
  isrFlag = true;       // set flag to tell loop() to print
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("---- ESP32 Timer ISR Test ----");
  timer = timerBegin(1);   // 1 Hz frequency gor evaery 1 sec
  // Attach ISR
  timerAttachInterrupt(timer, &onTimer);

  // Configure alarm: 1s, autoreload = true, start = true
  timerAlarm(timer, 1000000, true, 0);

  // Start timer
  timerStart(timer);
}

void loop() {
  if (isrFlag) {  // Only print when ISR sets flag
    isrFlag = false;
    Serial.print("ISR Triggered: ");
    Serial.println(isrCounter);
  }
}
