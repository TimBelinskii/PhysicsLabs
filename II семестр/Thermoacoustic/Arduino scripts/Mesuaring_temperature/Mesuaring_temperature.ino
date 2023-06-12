#include <math.h>
#define COOL_SIDE A0
#define Ac 0.00153066809
#define Bc 0.00016198180
#define Cc 0.00000043614

#define HOT_SIDE A1
#define Ah 0.00135014696
#define Bh 0.00019596720
#define Ch 0.00000024485

#define refVoltage 3.222
#define numForAver 5
#define expAverCoef 0.1

float filtTc=0, filtTh=0;

float R_t[80]  {61.70, 58.32, 55.15, 52.16, 49.36, 46.72, 44.24, 41.90, 39.70, 37.63, 35.68, 33.80, 32.10, 30.47, 28.93, 27.47,
                26.10, 24.80, 23.58, 22.42, 21.33, 20.30, 19.32, 18.39, 17.52, 16.69, 15.91, 15.16, 14.46, 13.79, 13.16, 12.56,
                11.99, 11.46, 10.94, 10.46, 10.00, 9.561, 9.144, 8.747, 8.370, 8.011, 7.670, 7.345, 7.036, 6.741, 6.461, 6.193,
                5.938, 5.695, 5.464, 5.242, 5.031, 4.830, 4.638, 4.454, 4.279, 4.111, 3.951, 3.797, 3.651, 3.511, 3.377, 3.248,
                3.126, 3.008, 2.896, 2.788, 2.684, 2.585, 2.490, 2.400, 2.312, 2.229, 2.148, 2.071, 1.997, 1.927, 1.858, 1.793};

int binSearch(float R){
  int l = -1, r = 80, m;
  while (l < r - 1){
    m = (l + r) / 2;
    if (R_t[m] * 1000 > R)
      l = m;
    else
      r = m;
  }
  return l;
}


float Thermistor(float V, float R_pull, float A=0, float B=0, float C=0, bool use_tabel=true){
  float T, R = V / ((refVoltage - V) / R_pull);
  if (use_tabel == true){
    int t = binSearch(R);
    T = (float)((R_t[t] * 1000 - R) / (R_t[t] * 1000 - R_t[t + 1] * 1000)  + t - 11);
  }
  else{
    if (A == 0||B == 0||C == 0){
      Serial.println("A, B, C is not defined!!!");
    }
    T = (float)(1/(A + B * log(R) + C * pow(log(R), 3)) - 273.15);
  }
  return T;
}

void setup(){
  Serial.begin(9600);
  Serial.println("t, Tc, Th");
  analogReference(EXTERNAL);
}
void loop() 
{
  char dataStr[100] = "";
  char buffer[7];
  float Vc=0, Vh=0;

  dataStr[0] = 0;

  long t0 = millis();
  for (int i = 0; i < numForAver; i++){
    Vc += (float)(analogRead(COOL_SIDE) * refVoltage) / 1024;
    Vh += (float)(analogRead(HOT_SIDE) * refVoltage) / 1024;
  }
  long t1 = millis();

  Vc /= numForAver;
  Vh /= numForAver;
  filtTc += (Thermistor(Vc, 10000.0, Ac, Bc, Cc, false) - filtTc) * expAverCoef;
  filtTh += (Thermistor(Vh, 10000.0, Ah, Bh, Ch, false) - filtTh) * expAverCoef;

  Serial.print((t1 + t0)/2);
  Serial.print(", ");
  Serial.print(filtTc);
  Serial.print(", ");
  Serial.println(filtTh);
  delay(200);
}