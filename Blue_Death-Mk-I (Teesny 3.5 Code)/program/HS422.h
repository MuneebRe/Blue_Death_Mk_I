#include <Arduino.h>
#include <Servo.h>

class HS422
{
  protected:
  Servo* myservo;
  int offset, output, limit;

  public:
  HS422(int pin);
  update(int servo_write);
  ~HS422();
};
