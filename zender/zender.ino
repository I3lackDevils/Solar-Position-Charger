String serial = "4.587,50.25,12,250.36,";
//char mystr[5] = "datta"; //String data

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
}

void loop() {
  Serial.println(serial); //Write the serial data
  Serial.print('\r');
  //Serial.write(mystr,5);
  delay(1000);
}
