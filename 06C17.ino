const int ledPin = 7;

unsigned int currentPeriodUs = 1000; // 기본값 
unsigned long onTimeUs = 0;
unsigned long offTimeUs = 0;

// 각 단계에 머무를 시간 1,000,000/200 = 5000
const unsigned long stepDurationUs = 5000;

// 함수 1
void set_period(int period) {
  currentPeriodUs = period; 
}

// 함수 2
void set_duty(int duty) {
  if (duty >= 0 && duty <= 100) {
    // 현재 주기에 맞춰 ON/OFF 시간 계산
    onTimeUs = currentPeriodUs * (duty / 100.0);
    offTimeUs = currentPeriodUs - onTimeUs;
  }
}

void customPwm() {
  // digitalWrite()와 delayMicroseconds() 사용
  if (onTimeUs > 0) {
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(onTimeUs);

    if (offTimeUs > 0) {
      digitalWrite(ledPin, LOW);
      delayMicroseconds(offTimeUs);
    }
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);

  set_period(10000); // 10ms 
  //set_period(1000);  // 1ms 
  //set_period(100);   // 0.1ms 
}

void loop() {
  
  // 밝기 증가 (0 -> 99) / 0.5초 
  for (int duty = 0; duty < 100; duty++) {
    set_duty(duty);
    unsigned long startTime = micros();
    while (micros() - startTime <= stepDurationUs) {
      customPwm();
    }
  }

  // 밝기 감소 (100 -> 1) / 0.5초 
  for (int duty = 100; duty > 0; duty--) {
    set_duty(duty);
    unsigned long startTime = micros();
    while (micros() - startTime <= stepDurationUs) {
      customPwm();
    }
  }
}
