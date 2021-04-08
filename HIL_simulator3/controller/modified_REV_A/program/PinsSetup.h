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
