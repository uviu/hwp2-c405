void setup() {
  // Serielle Kommunikation initialisieren
  Serial.begin(9600);
  
  // Die Pins 2 bis 5 als Eingänge konfigurieren
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  // Die Werte der digitalen Pins 2 bis 5 lesen
  int pin2Value = digitalRead(5);
  int pin3Value = digitalRead(4);
  int pin4Value = digitalRead(3);
  int pin5Value = digitalRead(2);
  
  // Die Werte über die serielle Verbindung an den Computer senden
  Serial.print(pin2Value);
  Serial.print(pin3Value);
  Serial.print(pin4Value);
  Serial.println(pin5Value);
}
