void sensorsuhu () {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Suhu :"); Serial.println(tempC);
  lcd.setCursor(0,0);
  lcd.print("Suhu = ");
  lcd.print(tempC);
  lcd.print(" C");
  
  if (Firebase.RTDB.setFloat(&fbdo, "suhu",tempC)){
  }
  
  //int pwmValue = map(fanSpeedValue, 0, 1023, 0, 255);

   float fanSpeed = calculateFanSpeed(tempC);
   float heaterOutput = calculateHeaterOutput(tempC);
   int heaterPower = map(heaterOutput, 0, 255, 0, 80); 
   acd.setPower(heaterPower);
  if (Firebase.RTDB.setFloat(&fbdo, "fan",fanSpeed)){
  }
  if (Firebase.RTDB.setFloat(&fbdo, "heater",heaterOutput)){
  }
  ledcWrite(pwmChannel2, fanSpeed);
  Serial.print("Kecepatan Kipas: ");
  Serial.println(fanSpeed);
  Serial.print("Output Heater: ");
  Serial.println(heaterOutput);
  Serial.println("============================");
}
