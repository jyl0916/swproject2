// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 100      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;
  
  // wait until next sampling time. 
  // millis() returns the number of milliseconds since the program started.
  //    will overflow after 50 days.
  if (millis() < (last_sampling_time + INTERVAL)) //앞서서는 샘플링 간격을 제어하기 위해 delay(INTERVAL)를 사용하여 타이밍을 설정했지만 
                                                  //개선한 코드에서는 millis()를 활용하여 마지막 샘플링 시간을 추적하고 지정된 샘플링 간격을 보장.(더 정밀해짐)
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  int bright = 0;
  if ((distance >= 100) && (distance <= 200)) {
      bright = map(distance,100,200,255,0);
      analogWrite(PIN_LED, bright);       
  } else if (distance > 200 && distance <=300) {
      bright = map(distance,200,300,0,255);
      analogWrite(PIN_LED, bright);
  } else {
      analogWrite(PIN_LED,255);      
    }

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.println("");


  
  //delay(50) 삭제시 loop() 함수가 더 빨리 실행되어 각 반복이 더 빠르게 진행되는 것 같다. 
  //따라서 다음 거리 측정까지의 시간 간격이 단축될 것 같다. 
  //이는 거리 측정을 더 자주 수행하게 되어 측정 데이터의 발생 및 출력이 빈번 -> delay()시간하고 loop()시간 더해서 나옴
  
  // update last sampling time
  last_sampling_time += INTERVAL; //last_sampling_time 변수는 이전 거리 측정이 이루어진 시간을 기록하는 데 사용됨
                                  //INTERVAL 변수는 거리 측정 간격을 나타내며, 새로운 거리 측정을 얼마나 자주 수행할지를 결정함
                                  //last_sampling_time 변수는 현재 시간에서 INTERVAL을 더함으로써 업데이트됨.
                                  //이렇게 하면 다음 거리 측정을 위한 시간이 설정되고, 다음 루프 반복에서 이 시간을 비교하여 거리 측정을 수행할 시간인지를 결정. 
                                  //이를 통해 정확한 시간 간격으로 거리를 측정할 수 있습니다.

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
  //        = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //        = 100,000 * 0.001 * 0.5 * 346
  //        = 17,300 mm  ==> 17.3m
}
