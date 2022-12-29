#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  // Return this process's ID
  int Pid();
  // Return the user (name) that generated this process
  std::string User();
  // Return the command that generated this process
  std::string Command();
  float CpuUtilization();                  // TODO: See src/process.cpp
  // Return this process's memory utilization
  std::string Ram();
  // Return the age of this process (in seconds)
  long int UpTime();
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_{0};
};

#endif