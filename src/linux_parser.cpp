#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string label, memTotal, memFree, unit;
  string line;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label >> memTotal >> unit;

    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> label >> memFree >> unit;
  }
  // Avoid division by zero
  if (std::stof(memTotal) == 0.0) {
    return 0.0;
  }
  return (std::stof(memTotal) - std::stof(memFree))/std::stof(memTotal); 
}

long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

vector<string> LinuxParser::CpuUtilization() {
  string label, user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  return {user, nice, system, idle, iowait, irq, softirq, steal};
}

long LinuxParser::CpuUtilization(int pid) {
  long utime, stime, cutime, cstime, starttime;
  string line;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    utime = std::stol(*std::next(std::istream_iterator<std::string>(linestream), 14));
    stime = std::stol(*std::next(std::istream_iterator<std::string>(linestream), 15));
    cutime = std::stol(*std::next(std::istream_iterator<std::string>(linestream), 16));
    cstime = std::stol(*std::next(std::istream_iterator<std::string>(linestream), 17));
    starttime = std::stol(*std::next(std::istream_iterator<std::string>(linestream), 22));
    return utime + stime + cutime + cstime;
  }
  return 0;
}

int LinuxParser::TotalProcesses() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}

string LinuxParser::Ram(int pid) {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          string unit;
          linestream >> unit;
          return value + " " + unit;
        }
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string name, temp, match_uid;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
      while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> temp >> match_uid) {
        if (match_uid == Uid(pid)) {
          return name;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string uptime;
    for (int i = 0; i < 22; i++) {
      linestream >> uptime;
    }
    long uptime_seconds = stol(uptime)/sysconf(_SC_CLK_TCK);
  }
  return 0;
}
