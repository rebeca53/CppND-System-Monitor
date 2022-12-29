#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
 // Return the aggregate CPU utilization
  float Utilization();

 private:
   long prevTotal;
   long prevIdle;
};

#endif