#include <Arduino.h>
#include <math.h>

#include <avr/io.h> // I/O and register definitions
#include <avr/interrupt.h> // interrupt functions and definitions

#define BIT(a) (1 << (a))

class A_pwm
{
  private:
  int pin;
  uint8_t bit_pos;
  uint8_t digital_pin_timer;
  int val;
  uint16_t* timer_cmp_out;
  TCB_t *timer_B;
  
  public:
  A_pwm() { ; };
  void setting(int pin);
  void set_val(int val);
};

void A_pwm::setting(int pin)
{
  this->pin = pin;
  val = 0;

  DDRD |= BIT(5);   //pinmode 5  output
  DDRD |= BIT(6);   //pinmode 6  output
  DDRB |= BIT(2);   //pinmode 10 output
  
  bit_pos = digitalPinToBitPosition(pin);
  digital_pin_timer =  digitalPinToTimer(pin);
  
  if(digital_pin_timer == TIMERA0)
  {
    timer_cmp_out = ((uint16_t*) (&TCA0.SINGLE.CMP0BUF)) + bit_pos;   //Find register address for timer pin
  } else {
    timer_B = ((TCB_t *)&TCB0 + (digital_pin_timer - TIMERB0));
  }
  
}

void A_pwm::set_val(int val)
{
  this->val = val;

  if(digital_pin_timer == TIMERA0)
  {
    *timer_cmp_out = val;
    TCA0.SINGLE.CTRLB |= BIT(TCA_SINGLE_CMP0EN_bp + bit_pos);         //Enable compare output
  } else 
  {
    timer_B->CCMPH = val;
    timer_B->CTRLB |= (TCB_CCMPEN_bm);
  }
  
}
