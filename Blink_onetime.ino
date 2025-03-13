void setup() {
  pinMode(8, OUTPUT);  
  pinMode(9, INPUT);             
}

void loop() {
  int button = digitalRead(9);
  if (button == HIGH) { 
    digitalWrite(8, HIGH);  
    delay(1000);           
    digitalWrite(8, LOW);  
    delay(1000);            
}
}
