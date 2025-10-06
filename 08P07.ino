#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100.0
#define _DIST_MAX 300.0

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

unsigned long last_sampling_time;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  
  Serial.begin(57600);
}

void loop() { 
  float distance;
  
  if (millis() < (last_sampling_time + INTERVAL))
    return;
    
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  int brightness;

  if (distance >= _DIST_MIN && distance <= 200.0) {
    brightness = map(distance, _DIST_MIN, 200.0, 255, 0);
  } else if (distance > 200.0 && distance <= _DIST_MAX) {
    brightness = map(distance, 200.0, _DIST_MAX, 0, 255);
  } else {
    brightness = 255;
  }

  analogWrite(PIN_LED, brightness);
  
  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.print(",Brightness:"); Serial.print(brightness);
  Serial.println("");
  
  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
