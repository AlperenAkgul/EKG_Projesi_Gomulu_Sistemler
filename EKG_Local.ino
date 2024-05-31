void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(4, INPUT);

}

void loop() {
  if((digitalRead(2) == 1) || (digitalRead(4) == 1)){
    Serial.println('!');
  }
  else{
    Serial.print(0);
    Serial.print(" ");
    Serial.print(5000);
    Serial.print(" ");
    Serial.println(analogRead(A0));

  }
  delay(50);

}
