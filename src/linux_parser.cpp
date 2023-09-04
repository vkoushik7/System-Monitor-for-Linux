#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel()
{
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return version + " " + kernel;
}

vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {

      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization()
{
  float MemTotal, MemFree;
  string key, value, kb, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb)
      {
        if (key == "MemTotal")
        {
          MemTotal = std::stof(value);
        }
        if (key == "MemFree")
        {
          MemFree = std::stof(value);
        }
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

long LinuxParser::UpTime()
{
  long uptime_long;
  string uptime, idletime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    uptime_long = std::stol(uptime);
  }
  return uptime_long;
}

long LinuxParser::ActiveJiffies(int pid)
{
  string value, line;
  long total_clk;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value)
    {
      stat_list.push_back(value);
    }
  }
  long int utime{std::stol(stat_list[13])};
  long int stime{std::stol(stat_list[14])};
  long int cutime{std::stol(stat_list[15])};
  long int cstime{std::stol(stat_list[16])};
  total_clk = (utime + stime + cutime + cstime);
  return total_clk;
}

vector<string> LinuxParser::CpuUtilization()
{
  string value, line, key;
  vector<string> jiffies_list;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key)
    {
      if (key != "cpu")
      {
        jiffies_list.push_back(key);
      }
    }
  }
  return jiffies_list;
}
int LinuxParser::TotalProcesses()
{
  int val;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
        {
          val = std::stoi(value);
          return val;
        }
      }
    }
  }
  return 0;
}
int LinuxParser::RunningProcesses()
{
  int val;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {

        if (key == "procs_running")
        {
          val = std::stoi(value);
          return val;
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid)
{
  string cmd, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      std::replace(line.begin(), line.end(), ' ', '*');
      while (linestream >> cmd)
      {
        std::replace(cmd.begin(), cmd.end(), '*', ' ');
        return cmd;
      }
    }
  }
  return string();
}

string LinuxParser::Ram(int pid)
{
  string key, value, kb, line;
  int vmsize;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb)
      {
        if (key == "VmSize:")
        {
          vmsize = std::stoi(value);
          vmsize = vmsize / 1000;
          return std::to_string(vmsize);
        }
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid)
{
  string key, value, uid, user, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid:")
        {
          uid = value;
        }
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid)
{
  string usr, passwd, uid, line;
  string uid_ = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> usr >> passwd >> uid)
      {
        if (uid == uid_)
        {
          return usr;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid)
{
  string value, line;
  long int starttime, uptime;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value)
    {
      stat_list.push_back(value);
    }
  }
  starttime = std::stol(stat_list[21]) / sysconf(_SC_CLK_TCK);
  uptime = LinuxParser::UpTime() - starttime;
  return uptime;
}