# VSSS Simulator

The documentation and the original repositories of the simulator can be found here: https://github.com/VSS-SDK.

For more information check the [VSS-SDK](https://vss-sdk.github.io/book/general.html).

## Installation

Just run the following commands from this directory and everything will be ready to go.

```
$ sudo .VSS-Core/configure.sh
$ sudo .VSS-Simulator/configure.sh
$ sudo .VSS-Viewer/configure.sh
$ sudo .VSS-Sample-Cpp/configure.sh
```

## Usage

To run the simulator just run the bash script

```
$ sudo ./run_simulator.sh
```

This will run the simulator using the VSS-SampleCpp/src/main.cpp file.
To stop the simulation just use ctrl + c, the bash script will kill all running processes.