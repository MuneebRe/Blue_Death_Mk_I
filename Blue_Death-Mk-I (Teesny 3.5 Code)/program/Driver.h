class Driver
{
  private:
  int in1_pin, in2_pin, enA_pin;
  double motor;

  public:
  Driver(int in1, int in2, int enA);
  void set(bool in1_val, bool in2_val, int enA_val);
  ~Driver();
};
