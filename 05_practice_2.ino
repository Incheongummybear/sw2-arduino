int toggle = 1;
int times = 0;
int pin = 7;

void setup() {
  pinMode(pin, OUTPUT); 
  digitalWrite(pin, 1);
  delay(1000);
}

void loop() {
  if (times == 6) {
    digitalWrite(pin, 0);
    while (1) {}
  }
  else if (toggle == 1) {
    digitalWrite(pin, 1);
    toggle = 0;
    delay(100);
  }
  else if (toggle == 0) {
    digitalWrite(pin, 0);
    toggle = 1;
    delay(100);
    times++;
  }
}
