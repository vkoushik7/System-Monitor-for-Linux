# System Monitor for Linux

System Monitor for Linux is a program viewer designed for Linux distributions. It has been developed entirely in C++ and utilizes the ncurses library to create a graphical representation of the System Monitor.

## Getting Started

Before you begin, please ensure you have the necessary dependencies installed:

### Prerequisites

Make sure you have the ncurses library installed. If it's not already installed, you can do so by running the following command:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Also ensure you have cmake installed as:

```bash
sudo apt install cmake
```

Build the application:
Clone:

```bash
git clone https://github.com/vkoushik7/System-Monitor-for-Linux.git
```

build:

```bash
make build
```

running the application

```bash
cd build
./monitor
```
