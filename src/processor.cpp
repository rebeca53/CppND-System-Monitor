#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  std::vector<std::string> values = LinuxParser::CpuUtilization();
  long user = std::stol(values[0]);
  long nice = std::stol(values[1]);
  long system = std::stol(values[2]);
  long idle = std::stol(values[3]);
  long iowait = std::stol(values[4]);
  long irq = std::stol(values[5]);
  long softirq = std::stol(values[6]);
  long steal = std::stol(values[7]);

  idle += iowait;
  long nonIdle = user + nice + system + irq + softirq + steal;
  long total = idle + nonIdle;
  double totalDelta = total - prevTotal;
  double idleDelta = idle - prevIdle;

  prevIdle = idle;
  prevTotal = total;
  if (totalDelta == 0.0) {
    return 0.0;
  }
  return (totalDelta - idleDelta) / totalDelta;
}