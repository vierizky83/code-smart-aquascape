// Fungsi Keanggotaan Suhu
float temp_SDingin(float x) {
  if (x <= 20) return 1;
  if (x > 20 && x <= 23) return (23 - x) / 3.0;
  return 0;
}

float temp_Dingin(float x) {
  if (x <= 24) return 0;
  if (x > 24 && x <= 25) return (x - 24);
  if (x > 25 && x <= 27) return (27 - x) / 2.0;
  return 0;
}

float temp_Normal(float x) {
  if (x <= 26) return 0;
  if (x > 26 && x <= 27) return (x - 26);
  if (x > 27 && x <= 28) return (28 - x);
  return 0;
}

float temp_Panas(float x) {
  if (x <= 27) return 0;
  if (x > 27 && x <= 29) return (x - 27) / 2.0;
  if (x > 29 && x <= 30) return (30 - x);
  return 0;
}

float temp_SPanas(float x) {
  if (x <= 29) return 0;
  if (x > 29 && x <= 31) return (x - 29) / 2.0;
  if (x > 31) return 1;
  return 0;
}
// Fungsi Output Sugeno untuk Kecepatan Kipas
float fanSpeed_Off(float temp) {
  return 0; // Output konstan
}

float fanSpeed_Sedang(float temp) {
  return 127; // Output konstan
}

float fanSpeed_Cepat(float temp) {
  return 255; // Output konstan
}

// Fungsi Output Sugeno untuk Heater
float heater_Off(float temp) {
  return 0; // Output konstan
}

float heater_Sedang(float temp) {
  return 127; // Output konstan
}

float heater_Tinggi(float temp) {
  return 255; // Output konstan
}
// Struct untuk aturan fuzzy
struct Rule_suhu {
  float (*tempMembershipFunction)(float);
  float (*fanOutputFunction)(float);
  float (*heaterOutputFunction)(float);
};

// Mendefinisikan aturan
Rule_suhu suhurules[] = {
  {temp_SDingin, fanSpeed_Off, heater_Tinggi},
  {temp_Dingin, fanSpeed_Off, heater_Sedang},
  {temp_Normal, fanSpeed_Off, heater_Off},
  {temp_Panas, fanSpeed_Sedang, heater_Off},
  {temp_SPanas, fanSpeed_Cepat, heater_Off}
};
// Deklarasi fungsi untuk menghitung output menggunakan metode Sugeno
float calculateFanSpeed(float temp) {
  float fanSpeedSum = 0;
  float weightSum = 0;

  // Evaluasi setiap aturan dan akumulasi jumlah berbobot
  for (int i = 0; i < sizeof(suhurules) / sizeof(Rule_suhu); i++) {
    float membershipValue = suhurules[i].tempMembershipFunction(temp);
    if (membershipValue > 0) {
      fanSpeedSum += membershipValue * suhurules[i].fanOutputFunction(temp);
      weightSum += membershipValue;
    }
  }

  // Menghitung rata-rata berbobot untuk kecepatan kipas
  return (weightSum > 0) ? (fanSpeedSum / weightSum) : 0;
}

float calculateHeaterOutput(float temp) {
  float heaterOutputSum = 0;
  float weightSum = 0;

  // Evaluasi setiap aturan dan akumulasi jumlah berbobot
  for (int i = 0; i < sizeof(suhurules) / sizeof(Rule_suhu); i++) {
    float membershipValue = suhurules[i].tempMembershipFunction(temp);
    if (membershipValue > 0) {
      heaterOutputSum += membershipValue * suhurules[i].heaterOutputFunction(temp);
      weightSum += membershipValue;
    }
  }

  // Menghitung rata-rata berbobot untuk output heater
  return (weightSum > 0) ? (heaterOutputSum / weightSum) : 0;
}
