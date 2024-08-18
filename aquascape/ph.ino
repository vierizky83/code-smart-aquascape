
void sensorph(){  
	//voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
		

	//phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
	int readings[numReadings]; // Array untuk menyimpan pembacaan
  int total = 0; // Total dari pembacaan
  int minVal = 1023; // Inisialisasi dengan nilai maksimum ADC
  int maxVal = 0; // Inisialisasi dengan nilai minimum ADC

  // Membaca nilai dari sensor
  for (int i = 0; i < numReadings; i++) {
    readings[i] = analogRead(pinPH);
    total += readings[i];
    if (readings[i] < minVal) {
      minVal = readings[i];
    }
    if (readings[i] > maxVal) {
      maxVal = readings[i];
    }
    delay(50); // Delay antara pembacaan ( 0.9921 * readPH) + 0.3316;
  }

  // Menghitung rata-rata dari nilai minimum dan maksimum
  int average = (minVal + maxVal) / 2;
  float readPH = (-0.0204 * average) + 15.842; //rumus regresi linier ADC to PH -0.0212x + 15.962
  float readPH_FIX = ( 1.0291 * readPH) - 0.4811; //rumus regresi linier PH to PH  0.9921x + 0.3316
  
  //Serial.print("ADC:");  Serial.print(average);  //Serial.print(" PH:");  
  //Serial.print(readPH);
  Serial.print(" PH FIX:");  Serial.println(readPH_FIX);
  
	lcd.setCursor(0,1);
  lcd.print("PH = " + String(readPH_FIX,2 ));
  

  float pumpUpSum = 0;
  float pumpDownSum = 0;
  float weightSum = 0;
  for (int i = 0; i < sizeof(rules) / sizeof(Rule); i++) {
  float membershipValue = rules[i].membershipFunction(readPH_FIX);
  if (membershipValue > 0) {
      pumpUpSum += membershipValue * rules[i].pumpUpOutput;
      pumpDownSum += membershipValue * rules[i].pumpDownOutput;
      weightSum += membershipValue;
    }
  }
  float pumpUp = (weightSum > 0) ? (pumpUpSum / weightSum) : 0;
  float pumpDown = (weightSum > 0) ? (pumpDownSum / weightSum) : 0;  

  //control pump
  if (pumpUp > 0) {
      // Turn on pumpUp for the calculated duration
      Serial.println("Turning on pumpUp");
      digitalWrite(pumpUp_pin, LOW);
      delay(pumpUp * 1000); // delay for pumpUp duration in seconds
      digitalWrite(pumpUp_pin, HIGH);
      digitalWrite(pumpDown_pin, HIGH);
      //delay(60000);
      Serial.println("Turning off pumpUp");
      
    }

    if (pumpDown > 0) {
      // Turn on pumpDown for the calculated duration
      Serial.println("Turning on pumpDown");
      digitalWrite(pumpDown_pin, LOW);
      delay(pumpDown * 1000); // delay for pumpDown duration in seconds
      digitalWrite(pumpDown_pin, HIGH);
      digitalWrite(pumpUp_pin, HIGH);
      //delay(60000);
      Serial.println("Turning off pumpDown");
      
    }

  
  if (Firebase.ready() && signupOK ) {
     if (Firebase.RTDB.setFloat(&fbdo, "ph",readPH_FIX)){
     }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  if (Firebase.RTDB.setFloat(&fbdo, "pumpup",pumpUp)){
  }
   if (Firebase.RTDB.setFloat(&fbdo, "pumpdown",pumpDown)){
  }
    Serial.print("Pump Up Duration: ");
    Serial.println(pumpUp);
    Serial.print("Pump Down Duration: ");
    Serial.println(pumpDown);
    Serial.println("============================");
    //delay(60000);
}
