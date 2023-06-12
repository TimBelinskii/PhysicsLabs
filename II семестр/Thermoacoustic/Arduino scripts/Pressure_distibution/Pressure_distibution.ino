#include "VolAnalyzer.h"
VolAnalyzer analyzer(A0);


void setup() {
  Serial.begin(9600);
  Serial.println("I");
}

void loop() {
  if (analyzer.tick()) {
    // Serial.print(analyzer.getVol());
    // Serial.print(',');
    // Serial.print(analyzer.getRaw());
    // Serial.print(',');
    // Serial.print(analyzer.getRawMax());
    // Serial.print(',');
    Serial.println(analyzer.getVol());  
  }
}