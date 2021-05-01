#define BIT(a) (1 << (a))

#define z1_OUTPUT() DDRH |= BIT(4); //Pin 7 for Mega 2560
#define z1_HIGH() PORTH |= BIT(4);
#define z1_LOW()  PORTH &= ~BIT(4);

#define z2_OUTPUT() DDRH |= BIT(5); //Pin 8 for Mega 2560
#define z2_HIGH() PORTH |= BIT(5);
#define z2_LOW()  PORTH &= ~BIT(5);

//#define pin_A1()  ADMUX |= BIT(MUX0);
//#define pin_A3()  ADMUX |= BIT(MUX1) | BIT(MUX0);
//#define pin_A5()  ADMUX |= BIT(MUX2) | BIT(MUX0);

void pin_setup()
{
  z1_OUTPUT()
  z2_OUTPUT()
}

void timer1_setup()
{
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 5600;
  TIFR1 |= BIT(OCF1A);
  TIMSK1 = BIT(OCIE1A);
  TCCR1B |= BIT(WGM12);
  TCCR1B |= BIT(CS11) | BIT(CS10); //64 prescaler
  //TCCR1B |= BIT(CS10);  //no prescaler
  //TCCR1B |= BIT(CS11);
  TCNT1 = 0;
}

void ADC_setup()
{
  ADMUX = 0;
  //ADMUX |= BIT(MUX0) | BIT(MUX2);
  ADMUX |= BIT(MUX0);
  ADMUX |= BIT(REFS0);
 
  // set ADC control and status register A
  ADCSRA = 0;
  ADCSRA |= BIT(ADEN); // ADC enable
  ADCSRA |= BIT(ADIE); // ADC interrupt enable

  // ADPS0, ADPS1, ADPS2 - system clock to ADC input clock prescaler bits
  // smaller prescaler results in faster conversions, but too fast
  // reduces ADC resolution and accuracy
  //ADCSRA |= BIT(ADPS0) | BIT(ADPS1) | BIT(ADPS2); // 128 prescaler (slowest)
  // this gives a conversion theoretical time of 116 microseconds for one channel
  
  ADCSRA |= BIT(ADPS1) | BIT(ADPS2); // 64 prescaler
  //ADCSRA |= BIT(ADPS2); // 32 prescaler
  // this gives a conversion time of 60 microseconds for one channel


  ADCSRA |= BIT(ADSC); // start ADC conversion
}
