// Define membership functions for lux
float lux_Sgelap(float x) {
  if (x <= 0) return 1;
  if (x > 0 && x <= 5) return (5 - x) / 5.0;
  if (x > 5 && x <= 10) return (10 - x) / 5.0;
  return 0;
}

float lux_gelap(float x) {
  if (x <= 7) return 0;
  if (x > 7 && x <= 15) return (x - 7) / 8.0;
  if (x > 15 && x <= 20) return (20 - x) / 5.0;
  return 0;
}

float lux_Normal(float x) {
  if (x <= 17) return 0;
  if (x > 17 && x <= 25) return (x - 17) / 8.0;
  if (x > 25 && x <= 30) return (30 - x) / 5.0;
  return 0;
}

float lux_terang(float x) {
  if (x <= 27) return 0;
  if (x > 27 && x <= 35) return (x - 27) / 8.0;
  if (x > 35 && x <= 40) return (40 - x) / 5.0;
  return 0;
}

float lux_Sterang(float x) {
  if (x <= 37) return 0;
  if (x > 37 && x <= 45) return (x - 37) / 8.0;
  if (x > 45 && x <= 60) return 1;
  return 0;
}

// Define output levels for LED brightness
int ledOutput_off = 0;
int ledOutput_sedang = 127;
int ledOutput_cerah = 255;

// Menyimpan informasi aturan
struct Rule_lux {
  float (*membershipFunction)(float); // Pointer to membership function
  float ledOutput;
};

// Define the rules
  Rule_lux luxrules[] = {
  {lux_Sgelap, ledOutput_cerah},
  {lux_gelap, ledOutput_sedang},
  {lux_Normal, ledOutput_sedang},
  {lux_terang, ledOutput_off},
  {lux_Sterang, ledOutput_off}
};
