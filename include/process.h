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
  int Pid() const;
  // Return the user (name) that generated this process
  std::string User();
  // Return the command that generated this process
  std::string Command();
  // Return this process's CPU utilization
  float CpuUtilization() const;
  // Return this process's memory utilization
  std::string Ram();
  // Return the age of this process (in seconds)
  long int UpTime() const;
  // Overload the "less than" comparison operator for Process objects
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid_{0};
};

#endif