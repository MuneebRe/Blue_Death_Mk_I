unsigned int z1, z2;

void servo_pulse(unsigned int _z1, unsigned int _z2)
{
  if(z1 < 0) _z1 = 0;
  if(z1 > 2500) _z1 = 2500;
  z1 = _z1;

  if(z2 < 0) _z2 = 0;
  if(z2 > 2500) _z2 = 2500;
  z2 = _z2;
}

ISR(TIMER1_COMPA_vect)
{
  static float t = 0.0, dt = 0.0, tp = 0.0;
  static bool state = 0;

  t = micros()*1.0e-6 - t0;

  dt = t - tp;
  tp = t; // save previous time

  /*
  Serial.print("\nt(arduino) = ");
  Serial.print(t);
  
  Serial.print("\tdt = ");
  Serial.print(dt,6);
  */

  z1_HIGH();
  delayMicroseconds(z1);
  z1_LOW();

  z2_HIGH();
  delayMicroseconds(z2);
  z2_LOW();
  
}
