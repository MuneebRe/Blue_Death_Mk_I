class Ping
{
  private:
  int pin;
  double distance;
  long duration;

  public:
  Ping(int pin);
  void calculate();
  double get();
  ~Ping();
};
