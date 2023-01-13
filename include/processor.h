#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  // Return the aggregate CPU utilization
  float Utilization();

 private:
  double prevTotal{0};
  double prevIdle{0};
};

#endif