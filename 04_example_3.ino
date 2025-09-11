#define PIN_LED 13
int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Hello World!");
  count = toggle = 0;
  digitalWrite(PIN_LED, toggle); 
}

void loop() {
  Serial.println(++count);
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle); 
 
}

int toggle_state(int toggle) {
  if (toggle == 0) {
    delay(1000);
    toggle += 1;}
  else if (toggle == 1) {
    delay(1000);
    toggle -= 1;}
  return toggle;
}
