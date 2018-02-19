
void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  int leds[4] = {4, 5, 6, 7};
  
  for (int i = 0; i <= 3  ; i++) {
    digitalWrite(leds[i], LOW);
  }

  for (int i = 0; i <= 3  ; i++) {
    digitalWrite(leds[i - 1], LOW);
    digitalWrite(leds[i], HIGH);
    delay(100);
  }

  for (int i = 2; i >= 1 ; i--) {

    digitalWrite(leds[i], HIGH);
    digitalWrite(leds[i + 1], LOW);
    delay(100);
  }
}
