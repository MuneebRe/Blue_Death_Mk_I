class Hall_Sensor
{
  private:
  int pin;
  double duration;
  double rpm;
  bool state;

  public:
  Hall_Sensor(int pin);
  void calculate();
  void get();
  ~Hall_Sensor();
};
