float pH_Sasam(float x) {
  if (x <= 0) return 1;
  if (x > 0 && x <= 2) return (2 - x) / 2.0;
  if (x > 2 && x <= 5) return (5 - x) / 3.0;
  return 0;
}

float pH_Asam(float x) {
  if (x <= 4) return 0;
  if (x > 4 && x <= 5.5) return (x - 4) / 1.5;
  if (x > 5.5 && x <= 6.7) return (6.7 - x) / 1.2;
  return 0;
}

float pH_Normal(float x) {
  if (x <= 6.5) return 0;
  if (x > 6.5 && x <= 7) return (x - 6.5) / 0.5;
  if (x > 7 && x <= 7.4) return (7.4 - x) / 0.4;
  return 0;
}
float pH_Basa(float x) {
  if (x <= 7.2) return 0;
  if (x > 7.2 && x <= 8) return (x - 7) / 0.8;
  if (x > 8 && x <= 9.5) return (9.5 - x) / 1.5;
  return 0;
}

float pH_SBasa(float x) {
  if (x <= 9) return 0;
  if (x > 9 && x <= 11) return (x - 9) / 2.0;
  if (x > 11 && x <= 14) return 1;
  return 0;
}
float pumpOutput_off = 0;
float pumpOutput_sedang = 0.5;
float pumpOutput_lama = 1;

struct Rule {
  float (*membershipFunction)(float); // Pointer to membership function
  float pumpUpOutput;
  float pumpDownOutput;
};
Rule rules[] = {
  {pH_Sasam, pumpOutput_lama, pumpOutput_off},
  {pH_Asam, pumpOutput_sedang, pumpOutput_off},
  {pH_Normal, pumpOutput_off, pumpOutput_off},
  {pH_Basa, pumpOutput_off, pumpOutput_sedang},
  {pH_SBasa, pumpOutput_off, pumpOutput_lama}
};
