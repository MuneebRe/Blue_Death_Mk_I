#include "Blue_Death_Mk_I.h"

Blue_Death_Mk_I BD_Mk_I(0,0,0);

void setup() {
  Serial.begin(12000000);
}

void loop() {
  BD_Mk_I.update();
}
