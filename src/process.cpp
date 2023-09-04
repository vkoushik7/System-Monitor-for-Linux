#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)
{
    _pid = pid;
    _cpu_util = Process::CpuUtilization();
    _cmd = LinuxParser::Command(_pid);
}

int Process::Pid() { return _pid; }

float Process::CpuUtilization()
{
    long int Hz = sysconf(_SC_CLK_TCK);
    _total_time = LinuxParser::ActiveJiffies(_pid) / Hz;
    _seconds = LinuxParser::UpTime(_pid);
    return _total_time / _seconds;
}

string Process::Command() { return _cmd; }

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return LinuxParser::User(_pid); }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

bool Process::operator<(Process const &a) const
{
    return _cpu_util > a._cpu_util;
}