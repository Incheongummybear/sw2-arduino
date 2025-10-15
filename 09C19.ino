// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0      // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25        // sampling interval (unit: msec)
#define PULSE_DURATION 10  // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100      // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300      // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.2     // EMA weight of new sample (range: 0 to 1)

//#define N_SAMPLES 3
//#define N_SAMPLES 10
#define N_SAMPLES 30

// global variables
unsigned long last_sampling_time; // unit: msec
float dist_ema = _DIST_MAX;

float samples[N_SAMPLES];
int sample_index = 0;

float get_median(float arr[], int size) {
  float sorted_arr[size];
  for (int i = 0; i < size; i++) {
    sorted_arr[i] = arr[i];
  }

  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (sorted_arr[j] > sorted_arr[j + 1]) {
        float temp = sorted_arr[j];
        sorted_arr[j] = sorted_arr[j + 1];
        sorted_arr[j + 1] = temp;
      }
    }
  }

  return sorted_arr[size / 2];
}

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // initialize serial port
  Serial.begin(57600);

  for (int i = 0; i < N_SAMPLES; i++) {
    samples[i] = _DIST_MAX;
  }
}

void loop() {
  float dist_raw, dist_median;
  
  // wait until next sampling time.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);

  if (dist_raw == 0.0) {
    dist_raw = _DIST_MAX + 100;
  }

  samples[sample_index] = dist_raw;
  sample_index = (sample_index + 1) % N_SAMPLES;

  dist_median = get_median(samples, N_SAMPLES);

  dist_ema = _EMA_ALPHA * dist_median + (1.0 - _EMA_ALPHA) * dist_ema;

  // output the read value to the serial port
  Serial.print("Min:");      Serial.print(_DIST_MIN);
  Serial.print(",raw:");      Serial.print(dist_raw);
  Serial.print(",ema:");      Serial.print(dist_ema);
  Serial.print(",median:");   Serial.print(dist_median);
  Serial.print(",Max:");      Serial.print(_DIST_MAX);
  Serial.println("");

  // do something here
  if ((dist_median < _DIST_MIN) || (dist_median > _DIST_MAX))
    digitalWrite(PIN_LED, 1);       // LED OFF
  else
    digitalWrite(PIN_LED, 0);       // LED ON

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm

  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - pulseIn(ECHO, HIGH, timeout) returns microseconds (음파의 왕복 시간)
  // - 편도 거리 = (pulseIn() / 1,000,000) * SND_VEL / 2 (미터 단위)
  //   mm 단위로 하려면 * 1,000이 필요 ==>  SCALE = 0.001 * 0.5 * SND_VEL
  //
  // - 예, pusseIn()이 100,000 이면 (= 0.1초, 왕복 거리 34.6m)
  //       = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //       = 100,000 * 0.001 * 0.5 * 346
  //       = 17,300 mm  ==> 17.3m
}
