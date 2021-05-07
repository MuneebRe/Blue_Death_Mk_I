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
