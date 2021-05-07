#include <SoftwareSerial.h>

class HC05
{
  private:
  int txPin, rxPin;
  unsigned long previousMillis;
  long interval;
  String ledB;
  int ledState;
  float t, t0, y;
  char buffer_in[64];
  char *p, *p_buffer, *pc;
  int *pi;
  long *pl;
  double *pd;
  bool *pb;
  int n;
  int state;
  


  public:
  HC05();
  void update(int shared_mem[]);
  ~HC05();
};
