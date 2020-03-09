#!/bin/bash

cleanUp() {
    # Forcefully kill all programs
    kill -9 $VIEWER_PID
    kill -9 $SAMPLE_PID
    kill -9 $SIMULATOR_PID

    echo ''
}

# Always re build the VSS-SampleCpp/src/main.cpp file
rm -R build
mkdir -p build
cd build
cmake ..
make
# If make fail exit the script
if [[ $? -ne 0 ]] ; then
    exit 1
fi
cd ..

# Run simulator and save the PID's to kill them after
./VSS-Viewer/build/vss-viewer --yellow_debug_port=5558 --state_port=5555 &
VIEWER_PID=$!
./build/vss-sample &
SAMPLE_PID=$!
./VSS-Simulator/build/vss-simulator &
SIMULATOR_PID=$!

# Trap ctrl + c to kill all progarms
trap cleanUp SIGINT

# Wait until last run program end (or get killed)
wait $!