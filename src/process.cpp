#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
    long totalTime = LinuxParser::CpuUtilization(pid_);
    return totalTime / UpTime();
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
    return this->CpuUtilization() < a.CpuUtilization();
}