# exit when any command fails
set -e

DISTRO=``
RELEASE=``
RELEASE_DEBIAN=``
ARCHITECTURE=``
COMPILE_TYPE=$1

INSTALLED=0

CMAKE () {
    rm -R build
    mkdir -p build
    cd build
    cmake ..
    make
    cd ..
}

INSTALL_UBUNTU_18_04 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkg-config
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_UBUNTU_16_04 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkg-config
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_UBUNTU_16_10 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkgconf
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_UBUNTU_14_04 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkg-config
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_MINT_18_2 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkg-config
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_DEBIAN_9 () {
  apt-get update && apt-get upgrade
  apt-get -y install pkgconf
  apt-get -y install g++ cmake libxmu-dev libxi-dev protobuf-compiler libprotobuf-dev
  INSTALLED=1
}

INSTALL_BASE() {
    apt-get update && apt-get upgrade
    apt-get -y install lsb-release;

    DISTRO=`lsb_release -si`
    RELEASE=`lsb_release -sr`
    RELEASE_DEBIAN=`lsb_release -sr | cut -c1-1`
    ARCHITECTURE=`uname -m`
}

INIT_SUBMODULES() {
    git submodule init;
    git submodule update;
}

INSTALL () {
    INSTALL_BASE;

    # Ubuntu
    if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "18.04" ]]; then
        INSTALL_UBUNTU_18_04;
    fi

    if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "16.04" ]]; then
        INSTALL_UBUNTU_16_04;
    fi

    if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "16.10" ]]; then
        INSTALL_UBUNTU_16_10;
    fi

    if [[ "$DISTRO" == "Ubuntu" ]] && [[ "$RELEASE" == "14.04" ]]; then
        INSTALL_UBUNTU_14_04;
    fi

    # Debian
    if [[ "$DISTRO" == "Debian" ]] && [[ "$RELEASE_DEBIAN" == "9" ]]; then
        INSTALL_DEBIAN_9;
    fi

    # LinuxMint
    if [[ "$DISTRO" == "LinuxMint" ]] && [[ "$RELEASE" == "18.2" ]]; then
        INSTALL_MINT_18_2;
    fi

    if [[ $INSTALLED == 0 ]]; then
        echo "Sistema Operacional Incompatível";
    fi

    if [[ $INSTALLED == 1 ]]; then
        INIT_SUBMODULES;
        CMAKE;
    fi
}

INIT_SUBMODULES;

cd VSS-Core/
sudo ./configure.sh

cd ../VSS-Simulator/
sudo ./configure.sh

cd ../VSS-Viewer/
sudo ./configure.sh

cd ..
INSTALL;
