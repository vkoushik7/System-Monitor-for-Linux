#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>

using std::string;
class Process
{
public:
    Process(int pid);
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const &a) const;

private:
    int _pid{0};
    string _usr{}, _cmd{}, _ram{};
    float _seconds{0.0}, _total_time{0.0}, _cpu_util{0.0};
};

#endif