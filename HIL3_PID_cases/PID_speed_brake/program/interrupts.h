float ADC_read[3] = {0,0,0};


ISR(TIMER1_COMPA_vect)
{
  static float t = 0.0, dt = 0.0, tp = 0.0;

  //t = micros()*1.0e-6 - t0;

  //dt = t - tp;
  //tp = t; // save previous time

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

ISR(ADC_vect)
{
  static double adc1;
  static int i = 0;
  int N = 15;
  float N_inv = 1.0/N;
  static int index = 0;
  
  float voltage;
  const float ADC_to_V = 1.0/1023.0*5;
  
  // read the ADC (10-bits) // 0 - 1023
  adc1 += ADC;
  
  i++;
  if(i > N)
  {
    adc1 = adc1*N_inv;
    voltage = adc1 * ADC_to_V;
    ADC_read[index] = voltage;
    i = 0;
    adc1 = 0;
  
    index++;
    ADMUX+=2;
  }
  
  if(index > 2)
  {
    index = 0;
    ADMUX = 0;
    ADMUX |= BIT(MUX0);
    ADMUX |= BIT(REFS0);
  }

  ADCSRA |= BIT(ADSC); // start new ADC conversion to avoid 
  /*
  Serial.print("ADC1 = ");
  Serial.print(ADC_read[0]);
  Serial.print("\tADC2 = ");
  Serial.print(ADC_read[1]);
  Serial.print("\tADC3 = ");
  Serial.println(ADC_read[2]);
  */
}
