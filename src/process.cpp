#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
  return LinuxParser::CpuUtilization(pid_);
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() {
  string ram = LinuxParser::Ram(pid_);
  if (ram == "") {
    return "";
  }
  long ram_kb = std::stol(ram);
  long ram_mb = ram_kb / 1000.0;
  return to_string(ram_mb);
}

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return (this->CpuUtilization() < a.CpuUtilization());
}

bool Process::operator>(Process const& a) const {
  return (this->CpuUtilization() > a.CpuUtilization());
}