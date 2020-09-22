# VSSS_2019

RoBorregos  
Tecnológico de Monterrey  
Monterrey, Nuevo León, México   
Latin American and Brazilian Robotics Competition (*LARC*), Very Small Size Soccer (***VSSS***)

Very Small Size Soccer @LARC 2019

-- Installation guide and docker setup --

I. For Mac OS  
  1. Install Docker  
    visit: https://docs.docker.com/docker-for-mac/install/  
  2. Download docker image  
    \> Open docker and sign in  
		$ docker pull osvaldoav/vsss 
  3. In its own terminal, create a bridge between a network socket & the X window server on OS X host  
    $ brew install socat  
	  $ socat TCP-LISTEN:6000,reuseaddr,fork UNIX-CLIENT:\"$DISPLAY\"  
  4. Install and configure XQuartZ  
    $ brew install xquartz  
    $ open -a Xquartz  
	  \> Open preferences and check on "Allow connections from network clients" in secutiy Tab  
  5. Add ip to control list  
    $ ip=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')  
    $ xhost + $ip  
  6. Enable X11 Forwarding  
    $ sudo nano /etc/ssh/sshd_config  
    \> X11Forwarding yes  
  7. Configure and run container  
    $ docker run -e DISPLAY=$ip:0 --name vsss -v <*RepoPath*\>:/root/src -it osvaldoav/vsss  
		**Note:**  change *RepoPath* for the absolute path to the repo in MacOS
  8. Inside docker instance terminal  
    defaults write org.macosforge.xquartz.X11 enable_iglx -bool true

  #### Common issues
  - Port already in use (kill socat process)  
	    $ lsof -i tcp:6000  
      $ kill -9 <PID> #where <PID> is the process id returned by lsof  
