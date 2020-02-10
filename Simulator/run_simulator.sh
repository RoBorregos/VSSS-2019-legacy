#!/bin/bash

cleanUp() {
    # Forcefully kill all programs
    kill -9 $VIEWER_PID
    kill -9 $SAMPLE_PID
    kill -9 $SIMULATOR_PID

    echo ''
}

# Always re build the VSS-SampleCpp/src/main.cpp file
make -B -C VSS-SampleCpp/build/
# If make fail exit the script
if [[ $? -ne 0 ]] ; then
    exit 1
fi

./VSS-Viewer/build/vss-viewer --yellow_debug_port=5558 --state_port=5555 &
# Save the vss-viewer PID for killing it after
VIEWER_PID=$!
./VSS-SampleCpp/build/vss-sample &
SAMPLE_PID=$!
./VSS-Simulator/build/vss-simulator &
SIMULATOR_PID=$!

# Trap ctrl + c to kill all progarms
trap cleanUp SIGINT

# Wait until last run program end (or get killed)
wait $!