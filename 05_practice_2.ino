#define PIN_LED 7 

void setup() {
  pinMode(PIN_LED, OUTPUT); 
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  digitalWrite(PIN_LED, LOW);
  delay(1000); 
  digitalWrite(PIN_LED, HIGH);

  for(int i = 0; i<5; i++){  
     digitalWrite(PIN_LED, HIGH);
     delay(200); 
     digitalWrite(PIN_LED, LOW);
     delay(200);
    }
  digitalWrite(PIN_LED, HIGH); 
}

void loop() {
;
}
