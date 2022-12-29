#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  // Return the system's CPU
  Processor& Cpu();
  // Return a container composed of the system's processes
  std::vector<Process>& Processes();
  // Return the system's memory utilization
  float MemoryUtilization();
  // Return the number of seconds since the system started running
  long UpTime();
  // Return the total number of processes on the system
  int TotalProcesses();
  // Return the number of processes actively running on the system
  int RunningProcesses();
  // Return the system's kernel identifier
  std::string Kernel();
  // Return the operating system name
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif