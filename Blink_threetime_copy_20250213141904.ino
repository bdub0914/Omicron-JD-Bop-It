void setup() {
  pinMode(8, OUTPUT);                    
}

void loop() {
  for (int i = 0; i < 3; i++){
  digitalWrite(8, HIGH);  
  delay(1000);                      
  digitalWrite(8, LOW);    
  delay(1000); 
}
  while(true);
}
