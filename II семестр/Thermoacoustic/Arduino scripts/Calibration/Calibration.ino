#define refVoltage 3.2

#define resistor1 A0
#define pull_up1 10000
#define resistor2 A1
#define pull_up2 10000

#define numForAver 5
#define expAvercoef 0.1

float mesV1, mesV2, R1, R2, filtV1=0, filtV2=0;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop() {
  mesV1 = 0;
  mesV2 = 0;
  for (int i=0;i < numForAver;i++){
    mesV1 += (float)(refVoltage * analogRead(resistor1) / 1024);
    mesV2 += (float)(refVoltage * analogRead(resistor2) / 1024);
  }
  filtV1 += (mesV1 / numForAver - filtV1) * 0.1;
  filtV2 += (mesV2 / numForAver - filtV2) * 0.1;
  R1 = (float)(pull_up1 * filtV1 / (refVoltage - filtV1));
  R2 = (float)(pull_up2 * filtV2 / (refVoltage - filtV2));
  Serial.print(R1);
  Serial.print(", ");
  Serial.println(R2);
}
