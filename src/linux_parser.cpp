#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace LinuxParser {
template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

}  // namespace LinuxParser

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
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
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
  return (std::stof(memTotal) - std::stof(memFree)) / std::stof(memTotal);
}

long LinuxParser::UpTime() { return getValueOfFile<long>(kUptimeFilename); }

vector<string> LinuxParser::CpuUtilization() {
  string label, user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;
  }
  return {user, nice, system, idle, iowait, irq, softirq, steal};
}

double LinuxParser::CpuUtilization(int pid) {
  long utime, stime, cutime, cstime;
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (i < 23) {
      linestream >> value;
      switch (i) {
        case 14:
          utime = std::stol(value);
          break;
        case 15:
          stime = std::stol(value);
          break;
        case 16:
          cutime = std::stol(value);
          break;
        case 17:
          cstime = std::stol(value);
          break;
        default:
          break;
      }
      i++;
    }

    long total_time = utime + stime + cutime + cstime;
    double seconds = UpTime(pid);
    return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
  return 0.0;
}

int LinuxParser::TotalProcesses() {
  return findValueByKey<int>(filterProcesses, kStatFilename);
}

int LinuxParser::RunningProcesses() {
  return findValueByKey<int>(filterRunningProcesses, kStatFilename);
}

string LinuxParser::Command(int pid) {
  return getValueOfFile<string>(to_string(pid) + kCmdlineFilename);
}

string LinuxParser::Ram(int pid) {
  // I`m using VmRSS instead of VmSize to get the physical memory being used.
  string value =
      findValueByKey<string>(filterProcMem, std::to_string(pid) + kStatusFilename);
  if (value == "") {
    value =
        findValueByKey<string>(filterProcMem, std::to_string(pid) + kStatMFilename);
  }
  return value;
}

string LinuxParser::Uid(int pid) {
  return findValueByKey<string>(filterUID, std::to_string(pid) + kStatusFilename);
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
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string uptime;
    for (int i = 0; i < 22; i++) {
      linestream >> uptime;
    }
    long uptime_seconds = stol(uptime) / sysconf(_SC_CLK_TCK);
    return UpTime() - uptime_seconds;
  }
  return 0;
}
