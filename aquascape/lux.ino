void sensorlux() {
  unsigned int data0, data1;
  light.getData(data0,data1);
  double lux;    // Resulting lux value
  boolean good;  // True if neither sensor is saturated
  good = light.getLux(gain,ms,data0,data1,lux);
  if (lux < 20 && gain == 0) {
    // If lux is very low and gain is low, switch to high gain
    gain = 1;
    light.setTiming(gain, 2, ms);
  } else if (lux > 40000 && gain == 1) {
    // If lux is very high and gain is high, switch to low gain
    gain = 0;
    light.setTiming(gain, 2, ms);
  }
  float ledOutputSum = 0;
  float weightSum = 0;
  // Evaluate each rule and accumulate weighted sums
  for (int i = 0; i < sizeof(luxrules) / sizeof(Rule_lux); i++) {
    float membershipValue = luxrules[i].membershipFunction(lux);
    if (membershipValue > 0) {
      ledOutputSum += membershipValue * luxrules[i].ledOutput;
      weightSum += membershipValue;
    }
  }
  // Calculate weighted average for LED output
  float ledOutput = (weightSum > 0) ? (ledOutputSum / weightSum) : 0;

  // Print the values for debugging
  Serial.print("Lux Value: ");
  Serial.println(lux);
  Serial.print("LED Brightness: ");
  Serial.println(ledOutput);
  Serial.println("===============");
    lcd.setCursor(0,2);
    lcd.print("lux = ");
    lcd.print(lux, 1 ); 
    //lcd.setCursor(0,3 );
    //lcd.print("Nilai PWM = ");
    //lcd.print(ledOutput);
  ledcWrite(pwmChannel1, ledOutput);
    
     if (Firebase.RTDB.setFloat(&fbdo, "kecerahanled",ledOutput)){ 
    }
    if (Firebase.RTDB.setFloat(&fbdo, "lux",lux)){ 
    }
    //if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  //}
  else {
    byte error = light.getError();
  } 
}
