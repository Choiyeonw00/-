#define PIN_LED 7

int i=0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 0);
  delay(1000);

  

}

void loop() {
  

  while(i<5){

  digitalWrite(PIN_LED, 1);
  delay(100);
  digitalWrite(PIN_LED, 0);
  delay(100);
  i++;


  
  }
  digitalWrite(PIN_LED, 1);
}
