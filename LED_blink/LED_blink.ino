void setup(){
  pinMode(2, OUTPUT);//pin 2 is ouput it is conneted to inbuilt led
}
void loop() {
 digitalWrite(2, HIGH);//led ON
  delay(1000); // 
  digitalWrite(2, LOW);//LED OFF
  delay(1000);
  digitalWrite(2, HIGH);LED ON
  delay(1000);
}